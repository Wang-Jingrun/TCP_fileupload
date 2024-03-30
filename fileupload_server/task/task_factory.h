#pragma once

#include <map>
#include <mutex>

#include <utility/singleton.h>
using namespace yazi::utility;

#include <socket/socket.h>
using namespace yazi::socket;

#include <thread/task.h>
using namespace yazi::thread;

namespace yazi
{
    namespace task
    {
        class TaskFactory
        {
            SINGLETON(TaskFactory);
        public:
            Task * create(int sockfd);
            void remove(int sockfd);

        private:
            std::map<int, Task *> m_sock_task;
            std::mutex m_mutex;
        };
    }
}
