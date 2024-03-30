#include <thread/thread_pool.h>
using namespace yazi::thread;

void ThreadPool::create(int threads)
{
    AutoLock lock(&m_mutex);
    m_threads = threads;
    for (int i = 0; i < threads; i++)
    {
        auto thread = new WorkerThread();
        m_pool.push_back(thread);
        thread->start();
    }
    log_debug("thread pool create worker threads: %d", threads);
}

WorkerThread * ThreadPool::get()
{
    AutoLock lock(&m_mutex);
    while (m_pool.empty())
        m_cond.wait(&m_mutex);
    auto thread = m_pool.front();
    m_pool.pop_front();
    return thread;
}

void ThreadPool::put(WorkerThread * thread)
{
    AutoLock lock(&m_mutex);
    m_pool.push_back(thread);
    m_cond.signal();
}

bool ThreadPool::empty()
{
    AutoLock lock(&m_mutex);
    return m_pool.empty();
}

void ThreadPool::assign(Task * task)
{
    auto thread = get();
    thread->assign(task);
}
