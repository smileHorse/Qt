#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(mainwindow);

	QApplication app(argc, argv);
	MainWindow window;
	window.show();
	return app.exec();
}
