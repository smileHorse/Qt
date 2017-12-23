
#include "sqlTable.h"

QString PersonTable::getColumnNames( int index )
{
	switch(index)
	{
	case PersonColumn_Id:
		return QStringLiteral("ID");
	case PersonColumn_Name:
		return QStringLiteral("����");
	case PersonColumn_Sex:
		return QStringLiteral("�Ա�");
	case PersonColumn_Birthday:
		return QStringLiteral("��������");
	default:
		return QStringLiteral("");
	}
}
