#pragma once

#include <cstdint>
#include <utility/time.h>
using namespace yazi::utility;

namespace yazi
{
	namespace utility
	{

		class Progress
		{
		 public:
			Progress() = delete;
			Progress(uint64_t total);
			Progress(uint64_t total, int barWidth); // 主构造
			~Progress() = default;

			void updateProgress(uint64_t current);

		 private:
			uint64_t m_total;
			int m_barWidth;
			Time m_begin_t;
		};

	} // utility
} // yazi

