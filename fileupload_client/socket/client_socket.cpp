#include <socket/client_socket.h>
using namespace yazi::socket;

#include <utility/progress.h>
using namespace yazi::utility;

ClientSocket::ClientSocket(const string& ip, int port) : Socket()
{
	Socket::connect(ip, port);
}

ClientSocket::~ClientSocket()
{
	close();
}

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
	Progress progress(fileSize);

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