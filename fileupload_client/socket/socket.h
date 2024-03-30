#pragma once

#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
using std::string;

#include <utility/logger.h>
using namespace yazi::utility;

namespace yazi
{
    namespace socket
    {
        class Socket
        {
        public:
            Socket();
            Socket(int sockfd);
            virtual ~Socket();

            int fd() const;
            bool bind(const string & ip, int port);
            bool listen(int backlog);
            bool connect(const string & ip, int port);
            int accept();
            int send(const char * buf, int len);
            int recv(char * buf, int len);
            void close();

            bool set_non_blocking();
            bool set_send_buffer(int size);
            bool set_recv_buffer(int size);
            bool set_linger(bool active, int seconds);
            bool set_keepalive();
            bool set_reuseaddr();

        protected:
            string m_ip;
            int m_port;
            int m_sockfd;
        };
    }
}