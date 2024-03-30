#include <utility/logger.h>
using namespace yazi::utility;

const char * Logger::s_level[LOG_COUNT] =
{
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

void Logger::open(const string & filename)
{
    m_filename = filename;
    m_ofs.open(filename.c_str(), std::ios::app);
    if (m_ofs.fail())
    {
        throw std::logic_error("open log file failed: " + filename);
    }
    m_ofs.seekp(0, std::ios::end);
    m_len = (int)m_ofs.tellp();
}

void Logger::close()
{
    m_ofs.close();
}

void Logger::log(Level level, const char* file, int line, const char* format, ...)
{
    if (m_level > level)
    {
        return;
    }

    if (m_ofs.fail())
    {
        return;
    }

    std::ostringstream oss;
    time_t ticks = time(nullptr);
    struct tm time_info = {0};
    localtime(&time_info, &ticks);
    char timestamp[32] = {0};
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &time_info);

    int len = 0;
    const char * fmt = "%s %s %s:%d ";
    len = snprintf(nullptr, 0, fmt, timestamp, s_level[level], file, line);
    if (len > 0)
    {
        char * buffer = new char[len + 1];
        snprintf(buffer, len + 1, fmt, timestamp, s_level[level], file, line);
        buffer[len] = 0;
        oss << buffer;
        m_len += len;
        delete [] buffer;
    }

    va_list arg_ptr;
    va_start(arg_ptr, format);
    len = vsnprintf(nullptr, 0, format, arg_ptr);
    va_end(arg_ptr);
    if (len > 0)
    {
        char * content = new char[len + 1];
        va_start(arg_ptr, format);
        vsnprintf(content, len + 1, format, arg_ptr);
        va_end(arg_ptr);
        content[len] = 0;
        oss << content;
        m_len += len;
        delete [] content;
    }

    oss << "\n";
    const string & str = oss.str();

    std::unique_lock<std::mutex> lck(m_mutex);
    m_ofs << str;
    m_ofs.flush();

    if (m_console)
    {
        std::cout << str;
    }

    if (m_max > 0 && m_len >= m_max)
    {
        rotate();
    }
}

void Logger::set_level(int level)
{
    m_level = level;
}

void Logger::set_console(bool console)
{
    m_console = console;
}

void Logger::set_max_size(int size)
{
    m_max = size;
}

void Logger::rotate()
{
    close();
    sleep(1000);
    time_t ticks = time(nullptr);
    struct tm time_info = {0};
    localtime(&time_info, &ticks);
    char timestamp[32] = {0};
    strftime(timestamp, sizeof(timestamp), ".%Y-%m-%d_%H-%M-%S", &time_info);
    string filename = m_filename + timestamp;
    if (rename(m_filename.c_str(), filename.c_str()) != 0)
    {
        return;
    }
    open(m_filename);
}

void Logger::sleep(int milliseconds)
{
#ifdef WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

void Logger::localtime(struct tm * time_info, const time_t * ticks)
{
#ifdef WIN32
    localtime_s(time_info, ticks);
#else
    localtime_r(ticks, time_info);
#endif
}
