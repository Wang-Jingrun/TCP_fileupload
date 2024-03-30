#include <task/fileupload_task.h>
#include <random>
#include <task/task_factory.h>
#include <unistd.h>
using namespace yazi::task;

#include <socket/socket_handler.h>
using namespace yazi::socket;

FileuploadTask::FileuploadTask(int sockfd) : Task(), m_sockfd(sockfd)
{
	reset();
}

void FileuploadTask::reset()
{
	m_total_len = 0;
	m_head_len = 0;
	m_file_len = 0;
}

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

void FileuploadTask::destroy()
{
	log_debug("fileupload task destroy");
	if (m_closed)
	{
		Singleton<TaskFactory>::instance()->remove(m_sockfd);
	}
	else
	{
		Singleton<SocketHandler>::instance()->attach(m_sockfd);
	}
}

void FileuploadTask::fail()
{
	reset();
	if (m_recv_file.is_open())
	{
		log_error("fileupload fail, file name: %s", m_filename.c_str());
		m_recv_file.close();
		if (unlink(m_filename.c_str()) != 0)
		{
			log_error("Error deleting file");
			log_error("error: errno=%d errmsg=%s", errno, strerror(errno));
		}
		log_info("delete file: %s", m_filename.c_str());
	}
}

string FileuploadTask::generateRandomString(int length)
{
	std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> distribution(0, charset.length() - 1);

	std::string result;
	for (int i = 0; i < length; ++i)
	{
		result += charset[distribution(generator)];
	}

	return result;
}