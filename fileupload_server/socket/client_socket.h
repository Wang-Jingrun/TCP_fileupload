#pragma once

#include <socket/socket.h>

namespace yazi
{
    namespace socket
    {
        class ClientSocket : public Socket
        {
        public:
            ClientSocket() = delete;
            ClientSocket(const string & ip, int port);
            ~ClientSocket() = default;
        };
    }
}