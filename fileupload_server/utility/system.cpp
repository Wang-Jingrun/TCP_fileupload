#include <utility/system.h>
using namespace yazi::utility;

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/resource.h>

#include <utility/string.h>
#include <utility/logger.h>
#include <utility/ini_file.h>
using namespace yazi::utility;

void System::init()
{
	core_dump();
	m_root_path = get_root_path();

	const string& dir = m_root_path + "/log";
	DIR* dp = opendir(dir.c_str());
	if (dp == nullptr)
	{
		mkdir(dir.c_str(), 0755);
	}
	else
	{
		closedir(dp);
	}
}

void System::core_dump()
{
	struct rlimit x;
	x.rlim_cur = RLIM_INFINITY;
	x.rlim_max = RLIM_INFINITY;
	setrlimit(RLIMIT_CORE, &x);
}

//string System::get_root_path()
//{
//	if (!m_root_path.empty())
//	{
//		return m_root_path;
//	}
//	char path[1024] = { 0 };
//	int len = readlink("/proc/self/exe", path, 1024);
//	if (len < 0 || len >= 1024)
//	{
//		return "";
//	}
//
//	auto paths = String::split(path, '/');
//	paths.pop_back();
//	paths.pop_back();
//
//	return String::join(paths, '/');
//}

string System::get_root_path()
{
	if (!m_root_path.empty())
	{
		return m_root_path;
	}
	char path[1024] = {0};
	int len = readlink("/proc/self/exe", path, 1024);
	if (len < 0 || len >= 1024)
	{
		return "";
	}
	for (int i = len; i >= 0; --i)
	{
		if (path[i] == '/')
		{
			path[i] = '\0';
			break;
		}
	}
	return path;
}
