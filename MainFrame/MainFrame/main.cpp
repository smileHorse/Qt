#include "MainFrame.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainFrame w;
	w.show();
	return a.exec();
}
