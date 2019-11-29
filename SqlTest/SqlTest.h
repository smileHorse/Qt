/*!
 * \file SqlTest.h
 *
 * \author Masf
 * \date 六月 2019
 *
 * 数据库测试
 */

#ifndef SQLTEST_H
#define SQLTEST_H

#include <QtSql/QtSql>

#include "TableData.h"

class CSqlTest
{
public:
	CSqlTest();
	~CSqlTest();

	// 添加数据库连接
	bool addConnection(const QString& connectionName);

	// 打开数据库
	bool open();

	// 关闭数据库
	void close();

	void insertData();
	void insertDataInTransaction();
	void insertDelayedData();
	void insertDelayedDataInTransaction();
	void batchInsertData();
	void batchInsertDataInTransaction();

	void setCount(int _count) { m_count = _count; }

private:
	void insertDataImpl();
	void insertDelayedDataImpl();
	void batchInsertDataImpl();

	QList<TableData> getTableDatas();

	void outputDbError(const QString& desc);

	void outputMessage(const QString& message);

private:
	QSqlDatabase m_database;
	int m_count;
};
#endif
