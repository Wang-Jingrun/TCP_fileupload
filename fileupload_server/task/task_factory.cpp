#include <task/task_factory.h>
#include <task/fileupload_task.h>
using namespace yazi::task;

Task * TaskFactory::create(int sockfd)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    auto it = m_sock_task.find(sockfd);
    if (it != m_sock_task.end())
    {
        return it->second;
    }
    auto task = new FileuploadTask(sockfd);
    m_sock_task[sockfd] = task;
    return task;
}

void TaskFactory::remove(int sockfd)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    auto it = m_sock_task.find(sockfd);
    if (it != m_sock_task.end())
    {
        delete it->second;
        m_sock_task.erase(it);
    }
    ::close(sockfd);
}