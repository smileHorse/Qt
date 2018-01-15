#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QList>
#include <QtWidgets/QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextCodec;
class QTextEdit;
QT_END_NAMESPACE
class PreviewForm;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

private slots:
	void open();
	void save();
	void about();
	void aboutToShowSaveAsMenu();

private:
	void findCodecs();
	void createActions();
	void createMenus();

	QTextEdit* textEdit;
	PreviewForm* previewForm;
	QList<QTextCodec*> codecs;

	QMenu* fileMenu;
	QMenu* helpMenu;
	QMenu* saveAsMenu;
	QAction* openAct;
	QList<QAction*> saveAsActs;
	QAction* exitAct;
	QAction* aboutAct;
	QAction* aboutQtAct;
};

#endif // MAINWINDOW_H
