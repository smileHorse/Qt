/*!
 * \file ZipcodeItem.h
 *
 * \author Masf
 * \date ÁùÔÂ 2019
 *
 * 
 */
#ifndef ZIPCODEITEM_H
#define ZIPCODEITEM_H

#include "global.h"
#include <QString>

class ZipcodeItem
{
public:
	explicit ZipcodeItem(int zipcode_=InvalidZipcode, 
		const QString& postOffice_=QString(),
		const QString& county_=QString(),
		const QString& state_=QString())
		: zipcode(zipcode_), postOffice(postOffice_), county(county_), state(state_)
	{}

	bool operator<(const ZipcodeItem& other) const
	{
		return zipcode != other.zipcode ? zipcode < other.zipcode 
			: postOffice < other.postOffice;
	}

	int zipcode;
	QString postOffice;
	QString county;
	QString state;
};
#endif