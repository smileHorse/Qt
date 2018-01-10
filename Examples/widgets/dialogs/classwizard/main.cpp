
#include <QtWidgets/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtCore/QLibraryInfo>

#include "classwizard.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(classwizard);

	QApplication a(argc, argv);
	
#ifndef QT_NO_TRANSLATION
	QString translatorFileName = QLatin1String("qt_");
	translatorFileName += QLocale::system().name();
	QTranslator* translator = new QTranslator(&a);
	if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
	{
		a.installTranslator(translator);
	}
#endif

	ClassWizard w;
	w.show();
	
	return a.exec();
}
