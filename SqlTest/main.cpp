
#include <QtCore/QCoreApplication>
#include <iostream>
using namespace std;

#include "SqlTest.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	CSqlTest sqlTest;
	bool result = sqlTest.open();

	int count;
	cout << "please input table data count(0 to exit): ";
	cin >> count;
	while(count > 0) 
	{
		sqlTest.setCount(count);

		// 逐条插入数据
		sqlTest.insertData();

		// 以事务的方式逐条插入数据
		sqlTest.insertDataInTransaction();

		// 延误的方式逐条插入数据
		sqlTest.insertDelayedData();

		// 延误加事务的放置逐条插入数据
		sqlTest.insertDelayedDataInTransaction();

		// 批量插入数据
		//sqlTest.batchInsertData();

		// 以事务的方式逐条插入数据
		//sqlTest.batchInsertDataInTransaction();

		cout << endl << endl;
		cout << "please input table data count(0 to exit): ";
		cin >> count;
	}
	
	return a.exec();
}
