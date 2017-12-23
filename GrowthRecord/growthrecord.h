#ifndef GROWTHRECORD_H
#define GROWTHRECORD_H

#include <QtWidgets/QMainWindow>

class QAction;
class QMenu;
class QToolBar;
class GrowthWidget;

class GrowthRecord : public QMainWindow
{
	Q_OBJECT

public:
	GrowthRecord(QWidget *parent = 0);
	~GrowthRecord();

private:
	void createWidgets();
	void createActions();
	QAction* createActionImpl(const QIcon& icon, const QString& text, 
		const QString& statusTip, const QKeySequence& key, const char* slot);
	void createConnects();
	void createMenus();
	void createToolBars();
	void createStatusBar();


private slots:
	void exportGrowthRecord();
	void personManage();
	void showGrowth();
	void updateGrowth();
	void about();

private:
	QAction* exportAction;
	QAction* closeAction;
	QAction* personManageAction;
	QAction* showAction;
	QAction* updateAction;
	QAction* aboutAction;

	QMenu*	fileMenu;
	QMenu*	recordMenu;
	QMenu*	helpMenu;

	QToolBar*	fileToolBar;
	QToolBar*	recordToolBar;

	GrowthWidget*	growthWidget;
};

#endif // GROWTHRECORD_H
