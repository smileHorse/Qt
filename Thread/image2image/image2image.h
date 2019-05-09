#ifndef IMAGE2IMAGE_H
#define IMAGE2IMAGE_H

#include <QtWidgets/QMainWindow>

class QCloseEvent;
class QComboBox;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QPushButton;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void convertOrCancel();
	void updateUi();
	void checkIfDone();
	void announceProgress(bool saved, const QString& message);
	void sourceTypeChanged(const QString& sourceType);

protected:
#ifdef USE_QTCONCURRENT
	bool event(QEvent* event);
#endif
	void closeEvent(QCloseEvent* event);

private:
	void createWidgets();
	void createLayout();
	void createConnections();
	void convertFiles(const QStringList& sourceFiles);

	QLabel *directoryLabel;
	QLineEdit *directoryEdit;
	QLabel *sourceTypeLabel;
	QComboBox *sourceTypeComboBox;
	QLabel *targetTypeLabel;
	QComboBox *targetTypeComboBox;
	QPlainTextEdit *logEdit;
	QPushButton *convertOrCancelButton;
	QPushButton *quitButton;

	int total;
	int done;
	volatile bool stopped;
};

#endif // IMAGE2IMAGE_H
