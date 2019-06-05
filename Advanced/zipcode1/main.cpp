#include "aqp.hpp"
#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("Zipcodes");

	MainWindow w;
	w.resize(800, 640);
	w.show();

	return a.exec();
}
