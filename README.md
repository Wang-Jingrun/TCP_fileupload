# TCP_fileupload

基于 TCP 实现的文件上传，支持多客户端连接以及上传大文件。[[视频演示]](视频演示.mp4)

## 文件传输数据格式

![image-20240330163500437](imgs/image-20240330163500437.png)

- `head`：数据头
  - `head size`：`size_t`，数据头的大小，防止与文件内容发生粘包
  - `filename`：`string`，文件名
  - `?`：`char`，分隔符
  - `filesize`：`uint64_t`，文件内容的字节数
- `file content`：文件内容

## 服务端

### 架构设计

采用了线程池、非阻塞 socket、IO 多路复用 epoll 以及 reactor 事件处理模型

![image-20240302111001698](imgs/image-20240302111001698.png)

### 代码结构

### 执行流程

### 接收文件核心逻辑

`./fileupload_server/task/fileupload_task.cpp`

```cpp
/*
private:
	// socket 信息
	int m_sockfd = 0;
	bool m_closed = false;

	// 文件信息
	std::ofstream m_recv_file;
	string m_filename;
	uint64_t m_total_len;
	size_t m_head_len;
	uint64_t m_file_len;
*/

void FileuploadTask::run()
{
    log_debug("fileupload task run: conn=%d", m_sockfd);

    // 接收客户端的数据
    char buf[recv_buff_size] = { 0 };
    Socket socket(m_sockfd);
    int len = socket.recv(buf, sizeof(buf));
    if (len < 0)
    {
       if (errno == EAGAIN || errno == EWOULDBLOCK)
       {
          log_debug("socket recv would block: conn=%d", m_sockfd);
          return;
       }
       else if (errno == EINTR)
       {
          log_debug("socket recv interrupted: conn=%d", m_sockfd);
          return;
       }
       log_debug("socket connection abort: conn=%d", m_sockfd);
       fail();
       m_closed = true;
       return;
    }
    else if (len == 0)
    {
       log_debug("socket closed by peer: conn=%d", m_sockfd);
       fail();
       m_closed = true;
       return;
    }

    log_debug("recv: conn=%d msg=\n%s", m_sockfd, buf);

    m_total_len += len;
    log_debug("recv: conn=%d total_len=%llu", m_sockfd, m_total_len);
    if (m_head_len == 0)
    {
       memcpy(&m_head_len, buf, sizeof(size_t));
       log_info("recv head size=%zu", m_head_len);

       string file_info(buf + sizeof(size_t), m_head_len - sizeof(size_t));
       size_t sep_pos = file_info.find('?');
       if (sep_pos != string::npos)
       {
          // 防止重名
          std::size_t point_pos = file_info.find_last_of('.');
          m_filename = file_info.substr(0, point_pos) + '-' + generateRandomString(8)
             + file_info.substr(point_pos, sep_pos - point_pos);

          std::string file_size_str = file_info.substr(sep_pos + 1);
          memcpy(&m_file_len, file_size_str.c_str(), sizeof(uint64_t));
       }
       else
       {
          log_error("Invalid file info: conn=%d", m_sockfd);
          m_closed = true;
          fail();
          return;
       }

       log_info("recv file head size=%zu, file-name=%s, file-size=%llu", m_head_len, m_filename.c_str(), m_file_len);
       if (m_file_len > 0)
       {
          m_recv_file = std::ofstream(m_filename.c_str(), std::ios::binary);
          m_recv_file.write(buf + m_head_len, len - m_head_len);
       }
    }
    else
    {
       m_recv_file.write(buf, len);
    }

    if (m_total_len - m_head_len >= m_file_len)
    {
       log_info("recv file complete: file-name=%s, file-size=%llu", m_filename.c_str(), m_file_len);
       m_recv_file.close();
       string msg = "recv file complete!\r\n";
       socket.send(msg.c_str(), msg.size());
       reset();
    }
}
```

## 客户端

### 代码结构

### 发送文件核心逻辑

`./fileupload_client/socket/client_socket.cpp`

```cpp
bool ClientSocket::send_file(const string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		log_error("open file error: errno=%d errmsg=%s", errno, strerror(errno));
		return false;
	}

	/* send file head */
	size_t index = 0;
	char head[256] = { 0 };

	// 获取文件大小
	file.seekg(0, std::ios::end);
	uint64_t fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// 分隔符
	char sep = '?';

	// 数据头长度
	size_t headSize = sizeof(size_t) + filename.size() + sizeof(char) + sizeof(uint64_t);

	// 构造数据头
	memcpy(head + index, (char*)(&headSize), sizeof(size_t));
	index += sizeof(size_t);
	memcpy(head + index, filename.c_str(), filename.size());
	index += filename.size();
	memcpy(head + index, (char*)(&sep), sizeof(char));
	index += sizeof(char);
	memcpy(head + index, (char*)(&fileSize), sizeof(uint64_t));
	index += sizeof(uint64_t);
	log_info("head size: %zu, file size: %llu", headSize, fileSize);
	send(head, index);

	// 发送文件内容
	Progress progress(fileSize); // 进度显示

	char buffer[send_buff_size] = { 0 };
	uint64_t len = 0;
	while (file.read(buffer, sizeof(buffer)))
	{
		size_t bytesRead = file.gcount(); // 获取上一次读取的字节
		len += bytesRead;
		send(buffer, bytesRead);
		memset(buffer, 0, sizeof(buffer));
		progress.updateProgress(len);
	}
	int bytesRead = file.gcount(); // 获取上一次读取的字节
	len += bytesRead;
	send(buffer, bytesRead);
	memset(buffer, 0, sizeof(buffer));
	progress.updateProgress(len);

	log_info("send file size: %lu", len);
	printf("\r\nsend file size: %lu\r\n", len);

	// 接收确认信息
	recv(buffer, sizeof(buffer));
	log_info("recv: %s", buffer);
	printf("recv: %s\r\n", buffer);

	file.close();
	return true;
}
```

## Todo

- [ ] 支持带路径的文件

- [ ] 使用 QT 给客户端增加 UI，效果大致如下：

  ![客户端UI](imgs/客户端UI.png)

- [ ] 使客户端和服务端都可以接收或者发送文件


## Reference

https://github.com/kaifamiao/yazi-web