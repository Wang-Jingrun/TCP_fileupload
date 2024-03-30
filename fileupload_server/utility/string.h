#pragma once

#include <cctype>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <cstdarg>

using std::string;

namespace yazi
{
	namespace utility
	{
		class String
		{
		 public:
			String() = default;
			~String() = default;

			// 大小写转换
			static string to_lower(const string& input);    // 转成小写
			static string to_upper(const string& input);    // 转成大写
			static string capitalize(const string & input);		// 首字符大写

			// 裁剪左边或者右边指定字符串集合，trims字符可任意顺序
			static string ltrim(const string& input, const string& trims);
			static string rtrim(const string& input, const string& trims);
			static string trim(const string& input, const string& trims);

			// 字符串分割
			static std::vector<string> split(const string& input, const string& separator);
			static std::vector<string> split(const string& input, char separator);

			// 字符串合并
			static string join(const std::vector<string>& input, const string& separator);
			static string join(const std::vector<string>& input, char separator);

			// 判断字符串是否有前缀或者后缀
			static bool has_prefix(const string& input, const string& prefix);
			static bool has_suffix(const string& input, const string& suffix);

			static string format(const char * format, ...);
		};
	} // utility
} // yazi
