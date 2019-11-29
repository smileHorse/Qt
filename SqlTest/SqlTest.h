/*!
 * \file SqlTest.h
 *
 * \author Masf
 * \date ���� 2019
 *
 * ���ݿ����
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

	// ������ݿ�����
	bool addConnection(const QString& connectionName);

	// �����ݿ�
	bool open();

	// �ر����ݿ�
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
