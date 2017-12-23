
#include "sqlTable.h"

QString PersonTable::getColumnNames( int index )
{
	switch(index)
	{
	case PersonColumn_Id:
		return QStringLiteral("ID");
	case PersonColumn_Name:
		return QStringLiteral("姓名");
	case PersonColumn_Sex:
		return QStringLiteral("性别");
	case PersonColumn_Birthday:
		return QStringLiteral("出生日期");
	default:
		return QStringLiteral("");
	}
}
