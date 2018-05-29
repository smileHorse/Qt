#ifndef CONVERTFILEFRAME_H
#define CONVERTFILEFRAME_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QtWidgets>

class ConvertFileFrame : public QMainWindow
{
	Q_OBJECT

public:
	ConvertFileFrame(QWidget *parent = 0);
	~ConvertFileFrame();

private:
	void createWidgets();
	void createConnects();

private slots:
	void chooseSource();
	void chooseDest();
	void convert();
	void updateProcessList(const QString& text);
	void convertFinished();

private:
	QLineEdit* sourceLineEdit;
	QLineEdit* destLineEdit;
	QPushButton* sourcePushButton;
	QPushButton* destPushButton;
	QTextEdit*	processTextEdit;
	QPushButton*	runButton;
	QPushButton*	closeButton;

	QStringList	sourceFileNames;
	QString		destFolder;
};

#endif // CONVERTFILE_H
