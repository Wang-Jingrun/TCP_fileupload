#include <thread/thread_pool.h>
using namespace yazi::thread;

// 初始化
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

// 获取一个空闲的工作线程，如果没有则等待条件变量
WorkerThread* ThreadPool::get()
{
	AutoLock lock(&m_mutex);
	while (m_pool.empty())
		m_cond.wait(&m_mutex);
	auto thread = m_pool.front();
	m_pool.pop_front();
	return thread;
}

// 工作线程执行完成任务后，将自己放入空闲队列
void ThreadPool::put(WorkerThread* thread)
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

// 将任务交由线程池中的空闲线程进行处理
void ThreadPool::assign(Task* task)
{
	auto thread = get();
	thread->assign(task);
}