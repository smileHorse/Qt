
#include <QtWidgets/QApplication>
#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>

#include "candydialog.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setStyle(QStyleFactory::create("Fushion"));

	QFile file(":/qss/candy.qss");
	file.open(QFile::ReadOnly);
	a.setStyleSheet(file.readAll());

	CandyDialog w;
	w.show();
	return a.exec();
}
