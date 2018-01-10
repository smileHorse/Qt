#include "licensewizard.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	LicenseWizard w;
	w.show();
	return a.exec();
}
