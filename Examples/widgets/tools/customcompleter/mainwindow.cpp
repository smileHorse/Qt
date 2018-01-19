
#include <QtWidgets/QtWidgets>

#include "mainwindow.h"
#include "textedit.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	completingTextEdit = new TextEdit;
	completer = new QCompleter(this);
	completer->setModel(modelFromFile(":/resources/wordlist.txt"));
	completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setWrapAround(false);
	completingTextEdit->setCompleter(completer);

	setCentralWidget(completingTextEdit);
	resize(500, 300);
	setWindowTitle(tr("Completer"));
}

QAbstractItemModel* MainWindow::modelFromFile(const QString& fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
	{
		return new QStringListModel(completer);
	}

#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
	QStringList words;

	while(!file.atEnd())
	{
		QByteArray line = file.readLine();
		if (!line.isEmpty())
		{
			words << line.trimmed();
		}
	}

#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif
	
	return new QStringListModel(words, completer);
}
