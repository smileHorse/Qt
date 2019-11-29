
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

		// ������������
		sqlTest.insertData();

		// ������ķ�ʽ������������
		sqlTest.insertDataInTransaction();

		// ����ķ�ʽ������������
		sqlTest.insertDelayedData();

		// ���������ķ���������������
		sqlTest.insertDelayedDataInTransaction();

		// ������������
		//sqlTest.batchInsertData();

		// ������ķ�ʽ������������
		//sqlTest.batchInsertDataInTransaction();

		cout << endl << endl;
		cout << "please input table data count(0 to exit): ";
		cin >> count;
	}
	
	return a.exec();
}
