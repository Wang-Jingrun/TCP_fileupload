#include <iostream>
using namespace std;

#include <frame/server.h>
using namespace yazi::frame;

int main()
{
    auto server = Singleton<Server>::instance();
    server->start();
    return 0;
}
