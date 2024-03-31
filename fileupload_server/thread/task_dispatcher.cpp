#include <thread/task_dispatcher.h>
using namespace yazi::thread;

// 初始化
void TaskDispatcher::init(int threads)
{
	Singleton<ThreadPool>::instance()->create(threads);
	start();
}

// 主线程指派任务
void TaskDispatcher::assign(Task* task)
{
	log_debug("task dispatcher assign task: %x", task);
	AutoLock lock(&m_mutex);
	m_queue.push_back(task);
	m_cond.signal();
}

// 任务分发处理逻辑
void TaskDispatcher::handle(Task* task)
{
	ThreadPool* pool = Singleton<ThreadPool>::instance();
	if (!pool->empty())
	{
		// 将任务交由线程池中的空闲线程进行处理
		pool->assign(task);
	}
	else
	{
		// 当线程池没有空闲线程时，将任务加入任务队列中
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

// 任务分发线程主函数
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
		while (m_queue.empty()) // 任务队列为空，等待条件变量触发
			m_cond.wait(&m_mutex);
		Task* task = m_queue.front();
		m_queue.pop_front();
		m_mutex.unlock();
		handle(task);
	}
}