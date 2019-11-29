/*!
 * \file FunctionExecTime.h
 *
 * \author Masf
 * \date 四月 2019
 *
 * 统计函数执行时间
 */

#include "FunctionExecTime.h"
#include <QDebug>

CFunctionExecTime::CFunctionExecTime(const string& funName, bool show)
	: m_funName(funName), m_show(show)
{
	m_start = clock();
	m_end = clock();
}

CFunctionExecTime::~CFunctionExecTime()
{
	m_end = clock();
	long ellapsed = m_end - m_start;
	if (m_show)
	{
		qDebug() << "CFunctionExecTime -- " << m_funName.c_str() << " ellapsed " << ellapsed << " ms";
	}
}

void CFunctionExecTime::outputExecTime(const string& funName, const string& section, bool show /*= false*/)
{
	clock_t end = clock();
	long ellapsed = end - m_end;
	m_end = end;
	if (show || ellapsed >= 1000)
	{
		qDebug() << "CFunctionExecTime -- " << m_funName.c_str()
			<< " -- " << section.c_str() << " -- ellapsed " << ellapsed << " ms";
	}
}
