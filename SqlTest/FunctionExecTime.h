/*!
 * \file FunctionExecTime.h
 *
 * \author Masf
 * \date 四月 2019
 *
 * 统计函数执行时间
 */

#ifndef FUNCTIONEXECTIME_H
#define FUNCTIONEXECTIME_H

#include <ctime>
#include <string>
using namespace std;

#define FUNCTION_EXEC_TIME	CFunctionExecTime exec(__FUNCTION__)

class CFunctionExecTime
{
public:
	explicit CFunctionExecTime(const string& funName, bool show = true);
	~CFunctionExecTime();

	void outputExecTime(const string& funName, const string& section, bool show = true);

	bool isShow() const { return m_show; }

	void setShow(bool val) { m_show = val; }

private:
	string m_funName;
	clock_t m_start;
	clock_t m_end;
	bool m_show;
};
#endif
