
#include <QtWidgets/QApplication>

#include "growthrecord.h"
#include "sql/sqlTable.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	if (!createConnection())
	{
		return 1;
	}

	GrowthRecord w;
	w.show();
	return a.exec();
}
