/*!
 * \file SqlTest.cpp
 *
 * \author Masf
 * \date 六月 2019
 *
 * 数据库测试
 */

#include "SqlTest.h"
#include "FunctionExecTime.h"

const QString g_connectionName = "SqlTest";
const QString g_sql = "INSERT INTO t_alarm_yxtb_1907"
	" VALUES (?,?,?,?,?,?,?,?,?,?)";
const QString g_delayedSql = "INSERT DELAYED INTO t_alarm_yxtb_1907"
	" VALUES (?,?,?,?,?,?,?,?,?,?)";

CSqlTest::CSqlTest()
{

}

CSqlTest::~CSqlTest()
{
	close();
}

// 添加数据库连接
bool CSqlTest::addConnection(const QString& connectionName)
{
	if (QSqlDatabase::contains(connectionName))
	{
		m_database = QSqlDatabase::database(connectionName);
	}
	else
	{
		m_database = QSqlDatabase::addDatabase("QMYSQL", connectionName);
		m_database.setConnectOptions("MYSQL_OPT_RECONNECT=1;MYSQL_OPT_CONNECT_TIMEOUT=2880000");
	}

	if (m_database.isValid())
	{
		return true;
	}
	else
	{
		outputDbError("addConnection failed");
		return false;
	}
}

bool CSqlTest::open()
{
	if (!addConnection(g_connectionName))
	{
		return false;
	}

	if (m_database.isOpen())
	{
		outputMessage("database is opened...");
		return true;
	}

	m_database.setHostName("192.168.3.25");
	m_database.setPort(3306);
	m_database.setUserName("root");
	m_database.setPassword("root");
	m_database.setDatabaseName("kh8100t");

	if (m_database.open())
	{
		outputMessage("database open successed...");
		return true;
	}
	else
	{
		outputDbError("database open failed");
		return false;
	}
}

// 关闭数据库
void CSqlTest::close()
{
	QSqlDatabase db = QSqlDatabase::database(g_connectionName);
	if (db.isOpen())
	{
		db.close();
		QSqlDatabase::removeDatabase(g_connectionName);
	}
}

void CSqlTest::insertData()
{
	FUNCTION_EXEC_TIME;

	insertDataImpl();

	outputMessage("insertData finished");
}

void CSqlTest::insertDataInTransaction()
{
	FUNCTION_EXEC_TIME;

	m_database.exec("START TRANSACTION");

	insertDataImpl();

	m_database.exec("COMMIT");

	outputMessage("insertDataInTransaction finished");
}

void CSqlTest::insertDelayedData()
{
	FUNCTION_EXEC_TIME;

	insertDelayedDataImpl();

	outputMessage("insertDelayedData finished");
}

void CSqlTest::insertDelayedDataInTransaction()
{
	FUNCTION_EXEC_TIME;

	m_database.exec("START TRANSACTION");

	insertDelayedDataImpl();

	m_database.exec("COMMIT");

	outputMessage("insertDelayedDataInTransaction finished");
}

void CSqlTest::batchInsertData()
{
	FUNCTION_EXEC_TIME;

	batchInsertDataImpl();

	outputMessage("batchInsertData finished");
}

void CSqlTest::batchInsertDataInTransaction()
{
	FUNCTION_EXEC_TIME;

	m_database.exec("START TRANSACTION");

	batchInsertDataImpl();

	m_database.exec("COMMIT");

	outputMessage("batchInsertDataInTransaction finished");
}

void CSqlTest::insertDataImpl()
{
	QSqlQuery query(m_database);
	query.prepare(g_sql);

	QList<TableData> list = getTableDatas();
	Q_FOREACH(TableData data, list)
	{
		query.addBindValue(data.id);
		query.addBindValue("data.datet");
		query.addBindValue("data.station");
		query.addBindValue("data.equipName");
		query.addBindValue("data.line");
		query.addBindValue(data.dataName);
		query.addBindValue(data.type);
		query.addBindValue(data.state);
		query.addBindValue(data.warnSource);
		query.addBindValue(data.warnLevel);

		if(!query.exec())
		{
			qDebug() << "exec error: " << query.lastError().text();
		}
	}
}

void CSqlTest::insertDelayedDataImpl()
{
	QSqlQuery query(m_database);
	query.prepare(g_delayedSql);

	QList<TableData> list = getTableDatas();
	Q_FOREACH(TableData data, list)
	{
		query.addBindValue(data.id);
		query.addBindValue("data.datet");
		query.addBindValue("data.station");
		query.addBindValue("data.equipName");
		query.addBindValue("data.line");
		query.addBindValue(data.dataName);
		query.addBindValue(data.type);
		query.addBindValue(data.state);
		query.addBindValue(data.warnSource);
		query.addBindValue(data.warnLevel);

		if(!query.exec())
		{
			qDebug() << "exec error: " << query.lastError().text();
		}
	}
}

void CSqlTest::batchInsertDataImpl()
{
	QSqlQuery query(m_database);
	query.prepare(g_sql);

	QList<TableData> list = getTableDatas();
	QVariantList idList;
	QVariantList datetList;
	QVariantList stationList;
	QVariantList equipNameList;
	QVariantList lineList;
	QVariantList dataNameList;
	QVariantList typeList;
	QVariantList stateList;
	QVariantList warnSourceList;
	QVariantList warnLevelList;
	Q_FOREACH(TableData data, list)
	{
		idList << data.id;
		datetList << "data.datet";
		stationList << "data.station";
		equipNameList << "data.equipName";
		lineList << "data.line";
		dataNameList << data.dataName;
		typeList << data.type;
		stateList << data.state;
		warnSourceList << data.warnSource;
		warnLevelList << data.warnLevel;		
	}

	query.addBindValue(idList);
	query.addBindValue(datetList);
	query.addBindValue(stationList);
	query.addBindValue(equipNameList);
	query.addBindValue(lineList);
	query.addBindValue(dataNameList);
	query.addBindValue(typeList);
	query.addBindValue(stateList);
	query.addBindValue(warnSourceList);
	query.addBindValue(warnLevelList);

	if(!query.execBatch())
	{
		qDebug() << "execBatch error: " << query.lastError().text();
	}
}

QList<TableData> CSqlTest::getTableDatas()
{
	QList<TableData> list;
	for (int i = 0; i < m_count; ++i)
	{
		TableData data;
		data.id = QUuid::createUuid().toString().left(36);

		list.push_back(data);
	}
	return list;
}

void CSqlTest::outputDbError(const QString& desc)
{
	qDebug() << desc << ": \t" << m_database.lastError().text();
}

void CSqlTest::outputMessage(const QString& message)
{
	qDebug() << message;
}
