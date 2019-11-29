/*!
 * \file TableData.h
 *
 * \author Masf
 * \date ÁùÔÂ 2019
 *
 * 
 */
#ifndef TABLEDATA_H
#define TABLEDATA_H

#include <QtCore/QString>

struct TableData
{
	QString id;
	QString datet;
	QString station;
	QString equipName;
	QString line;
	QString dataName;
	QString type;
	int state;
	QString warnSource;
	QString warnLevel;

	TableData()
		: state(0)
	{}
};
#endif