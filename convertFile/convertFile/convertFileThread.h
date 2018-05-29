#ifndef CONVERTFILETHREAD_H
#define CONVERTFILETHREAD_H

#include <QtCore/QThread>

class ConvertFileThread : public QThread
{
	Q_OBJECT

public:
	ConvertFileThread(const QStringList& sourceFiles, const QString& destFolder);

protected:
	virtual void run();

private:
	void convertFile(const QString& file);
	QByteArray convertLine(const QByteArray& line);

Q_SIGNALS:
	void process(const QString& msg);

private:
	QStringList sourceFileNames;
	QString destFolder;
};
#endif