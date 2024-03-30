#pragma once

#include <socket/socket.h>

namespace yazi
{
    namespace socket
    {
		const size_t send_buff_size = 1024;

        class ClientSocket : public Socket
        {
        public:
            ClientSocket() = delete;
            ClientSocket(const string & ip, int port);
            ~ClientSocket();

			bool send_file(const string& filename);
        };
    }
}