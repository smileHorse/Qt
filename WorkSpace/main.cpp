#include "workspace.h"
#include <QtWidgets/QtWidgets>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName(QStringLiteral("工作台"));
	
	// 设置应用程序样式
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
