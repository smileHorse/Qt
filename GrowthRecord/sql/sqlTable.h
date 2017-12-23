#ifndef SQLTABLE_H
#define SQLTABLE_H

#include <QtSql/QtSql>
#include <QtWidgets/QMessageBox>

// ��Ա��Ϣ��
class PersonTable
{
public:
	enum PersonColumn
	{
		PersonColumn_Id = 0,
		PersonColumn_Name,
		PersonColumn_Sex,
		PersonColumn_Birthday,
		PersonColumn_Count
	};

	static QString getColumnNames(int index);
};

static bool createConnection()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(":memory:");
	if (!db.open())
	{
		QMessageBox::critical(0, QStringLiteral("�޷������ݿ�"), 
			QStringLiteral("�޷������ݿ�"), QMessageBox::Cancel);
		return false;
	}

	QSqlQuery query;
	query.exec("create table person("
				"int varchar(30) primary key, "
				"name varchar(20), " 
				"sex int, "
				"birthday date"
				")");

	query.exec("create table growth("
				"int varchar(30) primary key, "
				"personId int, "
				"time date, "
				"height double, "
				"weight double"
				")");

	return true;
}
#endif