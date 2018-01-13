#include "configdialog.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(configdialog);

	QApplication a(argc, argv);
	a.setApplicationDisplayName("Qt Example");
	ConfigDialog dialog;
	return dialog.exec();
}
