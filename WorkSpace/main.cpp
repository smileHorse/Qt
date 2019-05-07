#include "workspace.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName(QStringLiteral("¹¤×÷Ì¨"));

	WorkSpace w;
	w.show();

	return a.exec();
}
