#include <thread/task_dispatcher.h>
using namespace yazi::thread;

void TaskDispatcher::init(int threads)
{
    Singleton<ThreadPool>::instance()->create(threads);
    start();
}

void TaskDispatcher::assign(Task * task)
{
    log_debug("task dispatcher assign task: %x", task);
    AutoLock lock(&m_mutex);
    m_queue.push_back(task);
    m_cond.signal();
}

void TaskDispatcher::handle(Task * task)
{
    ThreadPool * pool = Singleton<ThreadPool>::instance();
    if (!pool->empty())
    {
        pool->assign(task);
    }
    else
    {
        AutoLock lock(&m_mutex);
        m_queue.push_front(task);
        log_warn("all threads are busy!");
    }
}

bool TaskDispatcher::empty()
{
    AutoLock lock(&m_mutex);
    return m_queue.empty();
}

void TaskDispatcher::run()
{
    sigset_t mask;
    if (0 != sigfillset(&mask))
    {
        log_error("task dispatcher sigfillset error!");
        return;
    }
    if (0 != pthread_sigmask(SIG_SETMASK, &mask, nullptr))
    {
        log_error("task dispatcher pthread_sigmask error!");
        return;
    }
    while (true)
    {
        m_mutex.lock();
        while (m_queue.empty())
            m_cond.wait(&m_mutex);
        Task * task = m_queue.front();
        m_queue.pop_front();
        m_mutex.unlock();
        handle(task);
    }
}
