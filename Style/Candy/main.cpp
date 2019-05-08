#include "candy.h"
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCommonStyle>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QString styleFileName = QApplication::applicationDirPath() + 
		QDir::separator() + "qss/candy.qss";
	QFile file(styleFileName);
	if(file.open(QFile::ReadOnly))
	{
		QString styleSheet = a.styleSheet();
		QString fileContent = QLatin1String(file.readAll());
		a.setStyleSheet(styleSheet + fileContent);
	}

	Candy w;
	w.show();

	return a.exec();
}
