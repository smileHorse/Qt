#include "image2image.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("Image2Image");
#ifdef Q_WS_MAC
	a.setCursorFlashTime(0);
#endif

	MainWindow w;
	w.show();

	return a.exec();
}
