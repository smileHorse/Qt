/*!
 * \file ConvertImageTask.h
 *
 * \author Masf
 * \date ���� 2019
 *
 * 
 */

#ifndef CONVERTIMAGETASK_H
#define CONVERTIMAGETASK_H

#include <QtCore/QRunnable>
#include <QtCore/QStringList>

class QObject;

class ConvertImageTask : public QRunnable
{
public:
	explicit ConvertImageTask(QObject* receiver, volatile bool* stopped,
		const QStringList& sourceFiles, const QString& targetType)
		: m_receiver(receiver), m_stopped(stopped), m_sourceFiles(sourceFiles),
		m_targetType(targetType)
	{}

private:
	void run();

	QObject* m_receiver;
	volatile bool* m_stopped;
	const QStringList m_sourceFiles;
	const QString m_targetType;
};
#endif