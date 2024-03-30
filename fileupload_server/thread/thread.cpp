#include <thread/thread.h>
using namespace yazi::thread;

Thread::Thread() : m_tid(0)
{
}

Thread::~Thread()
{
}

void Thread::start()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&m_tid, &attr, thread_func, (void *)this);
    pthread_attr_destroy(&attr);
}

void Thread::stop()
{
    pthread_exit(PTHREAD_CANCELED);
}

void * Thread::thread_func(void * ptr)
{
    auto thread = (Thread *)ptr;
    thread->run();
    return ptr;
}
