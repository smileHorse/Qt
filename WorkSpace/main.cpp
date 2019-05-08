#include "workspace.h"
#include <QtWidgets/QtWidgets>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName(QStringLiteral("����̨"));
	
	// ����Ӧ�ó�����ʽ
	QString sheetFilePath = QApplication::applicationDirPath() + QDir::separator() +
		"qss/WorkSpace.qss";
	QFile file(sheetFilePath);
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		QString styleSheet = a.styleSheet();
		styleSheet += QLatin1String(file.readAll());
		a.setStyleSheet(styleSheet);
	}

	WorkSpace w;
	w.show();

	return a.exec();
}
