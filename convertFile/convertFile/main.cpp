#include "convertFileFrame.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ConvertFileFrame w;
	w.show();
	return a.exec();
}
