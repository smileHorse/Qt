
#include "convertFileThread.h"

#include <QtCore/QtCore>

ConvertFileThread::ConvertFileThread( const QStringList& sourceFiles, const QString& destFolder )
{
	this->sourceFileNames = sourceFiles;
	this->destFolder = destFolder;
}

void ConvertFileThread::run()
{
	Q_FOREACH (QString sourceFile, sourceFileNames)
	{
		convertFile(sourceFile);
	}

	emit process(QStringLiteral("ȫ���ļ��������!!!"));
}

void ConvertFileThread::convertFile( const QString& sourceFile )
{
	QFile file(sourceFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		emit process(QStringLiteral("�ļ�: ") + sourceFile + QStringLiteral("���ɶ����޷�����!"));
		return;
	}

	QFileInfo fileInfo(sourceFile);
	QString outFileName = destFolder + "/" + fileInfo.fileName();
	QFile outFile(outFileName);
	if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		emit process(QStringLiteral("�ļ�: ") + outFileName + QStringLiteral("����ʧ��!"));
		return;
	}

	while(!file.atEnd()) {
		QByteArray line = file.readLine();

		QByteArray outLine = convertLine(line);

		outFile.write(outLine);
	}

	emit process(QStringLiteral("�ļ�: ") + sourceFile + QStringLiteral(" �������!"));
}

QByteArray ConvertFileThread::convertLine( const QByteArray& line )
{
	static QMap<QString, QString> directory;
	if (directory.isEmpty())
	{
		//directory.insert("6$", "3$");
		//directory.insert("3$", "4$");
		//directory.insert("4$", "5$");
		//directory.insert("5$", "6$");
		//directory.insert("7$", "10$");
		//directory.insert("9$", "7$");
		//directory.insert("10$", "11$");

		directory.insert("6$", "5$");
	}

	for (QMap<QString, QString>::iterator iter = directory.begin(); iter != directory.end(); ++iter)
	{
		QByteArray key = iter.key().toLocal8Bit();
		QByteArray value = iter.value().toLocal8Bit();
		if (line.startsWith(key))
		{
			return value + line.mid(line.indexOf(key) + key.length());
		}
	}

	return line;
}


