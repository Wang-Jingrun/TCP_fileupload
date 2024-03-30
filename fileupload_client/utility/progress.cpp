#include <utility/progress.h>
#include <iostream>
#include<iomanip>
using namespace yazi::utility;

Progress::Progress(uint64_t total) : Progress(total, 50)
{
}

Progress::Progress(uint64_t total, int barWidth) : m_total(total), m_barWidth(barWidth)
{
	m_begin_t = Time();
}

void Progress::updateProgress(uint64_t current)
{
	double progress = (double)(current) / m_total;
	int pos = (int)(m_barWidth * progress);

	// 进度条
	std::cout << "[";
	for (int i = 0; i < m_barWidth; ++i)
	{
		if (i < pos)
			std::cout << "=";
		else if (i == pos)
			std::cout << ">";
		else
			std::cout << " ";
	}
	std::cout << "] ";

	// 百分比
	std::cout << int(progress * 100.0) << "%  ";

	// 速度
	Time cur_t;
	std::cout << std::fixed << std::setprecision(2) << double(current / (cur_t - m_begin_t)) << "MB/s\r";

	if (current >= m_total) std::cout << "\r\n";
	std::cout.flush();

}