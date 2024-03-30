#pragma once

#include <map>
#include <socket/server_socket.h>
#include <socket/event_poller.h>

namespace yazi
{
    namespace socket
    {
        class SocketHandler
        {
            SINGLETON(SocketHandler);
        public:
            void listen(const string & ip, int port);
            void attach(int sockfd);
            void detach(int sockfd);
            void handle(int max_conn, int timeout);

        private:
            Socket * m_server = nullptr;
            EventPoller m_epoll;
        };
    }
}