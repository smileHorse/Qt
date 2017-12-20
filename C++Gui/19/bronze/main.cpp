
#include <QtWidgets/QtWidgets>

#include "bronzedialog.h"
#include "bronzestyle.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setStyle(new BronzeStyle);

	BronzeDialog w;
	w.layout()->setSpacing(7);
	w.layout()->setMargin(7);
	w.show();
	
	return a.exec();
}
