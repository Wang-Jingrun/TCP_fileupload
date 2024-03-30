#include <frame/server.h>
using namespace yazi::frame;

#include <thread/task_dispatcher.h>
using namespace yazi::thread;

#include <socket/socket_handler.h>
using namespace yazi::socket;

void Server::start()
{
    auto sys = Singleton<System>::instance();
    sys->init();
    string root_path = sys->get_root_path();

    // ini config
    auto ini = Singleton<IniFile>::instance();
    ini->load(root_path + "/config/server.ini");

    m_ip = (string)(*ini)["server"]["ip"];
    m_port = (*ini)["server"]["port"];
    m_threads = (*ini)["server"]["threads"];
    m_max_conns = (*ini)["server"]["max_conns"];
    m_wait_time = (*ini)["server"]["wait_time"];
    m_log_level = (*ini)["server"]["log_level"];

    // ini logger
    auto logger = Singleton<Logger>::instance();
    logger->open(root_path + "/log/server.log");
    logger->set_level(m_log_level);
    logger->set_console(false);

    // init the thread pool and task queue
    auto dispatcher = Singleton<TaskDispatcher>::instance();
    dispatcher->init(m_threads);

    // init the socket handler
    auto handler = Singleton<SocketHandler>::instance();
    handler->listen(m_ip, m_port);
    handler->handle(m_max_conns, m_wait_time);
}
