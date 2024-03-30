#include <socket/event_poller.h>
using namespace yazi::socket;

EventPoller::EventPoller() : m_epfd(0), m_max_conn(0), m_events(nullptr)
{
}

EventPoller::~EventPoller()
{
    if (m_epfd > 0)
    {
        ::close(m_epfd);
        m_epfd = 0;
    }

    if (m_events != nullptr)
    {
        delete [] m_events;
        m_events = nullptr;
    }
}

bool EventPoller::create(int max_conn)
{
    m_max_conn = max_conn;
    m_epfd = ::epoll_create(m_max_conn);
    if (m_epfd < 0)
    {
        return false;
    }
    m_events = new epoll_event[m_max_conn];
    return true;
}

void EventPoller::add(int fd, uint32_t events)
{
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    ::epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev);
}

void EventPoller::mod(int fd, uint32_t events)
{
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    ::epoll_ctl(m_epfd, EPOLL_CTL_MOD, fd, &ev);
}

void EventPoller::del(int fd)
{
    ::epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, nullptr);
}

int EventPoller::wait(int milliseconds)
{
    return ::epoll_wait(m_epfd, m_events, m_max_conn, milliseconds);
}

int EventPoller::get_fd(int idx)
{
    return m_events[idx].data.fd;
}

bool EventPoller::is_set(int idx, uint32_t events)
{
    return m_events[idx].events & events;
}