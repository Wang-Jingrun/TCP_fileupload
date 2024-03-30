#pragma once

#include <socket/socket.h>
using namespace yazi::socket;

#include <thread/task.h>
using namespace yazi::thread;

namespace yazi
{
	namespace task
	{
		const size_t recv_buff_size = 512 * 1024;

		class FileuploadTask : public Task
		{
		 public:
			FileuploadTask() = delete;
			FileuploadTask(int sockfd);
			~FileuploadTask() = default;

			void reset();
			virtual void run();
			virtual void destroy();
			void fail();

			static string generateRandomString(int length);

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
		};
	}
}