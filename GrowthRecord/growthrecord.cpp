
#include <QtWidgets/QtWidgets>

#include "growthrecord.h"
#include "growthWidget.h"
#include "personManageDialog.h"

GrowthRecord::GrowthRecord(QWidget *parent)
	: QMainWindow(parent)
{
	createWidgets();
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
}

GrowthRecord::~GrowthRecord()
{

}

void GrowthRecord::createWidgets()
{
	growthWidget = new GrowthWidget;
	setCentralWidget(widget);

	setWindowTitle(QStringLiteral("成长记录"));
	setWindowIcon(QIcon(":/icon.png"));
	resize(1200, 800);
}

void GrowthRecord::createActions()
{
	exportAction = createActionImpl(QIcon(":/export.png"), QStringLiteral("导出"), 
		QStringLiteral("导出成长记录"), QKeySequence("Ctrl+E"), SLOT(exportGrowthRecord()));
	closeAction = createActionImpl(QIcon(":/close.png"), QStringLiteral("关闭"), 
		QStringLiteral("退出当前应用程序"), QKeySequence::Quit, SLOT(close()));

	personManageAction = createActionImpl(QIcon(":/personmanage.png"), QStringLiteral("人员信息管理"), 
		QStringLiteral("管理人员信息：增、删、改"), QKeySequence("Ctrl+P"), SLOT(personManage()));
	showAction = createActionImpl(QIcon(":/show.png"), QStringLiteral("显示成长记录"), 
		QStringLiteral("显示成长记录信息"), QKeySequence("F2"), SLOT(showGrowth()));
	updateAction = createActionImpl(QIcon(":/update.png"), QStringLiteral("刷新成长记录"), 
		QStringLiteral("刷新成长记录"), QKeySequence("F5"), SLOT(updateGrowth()));

	aboutAction = createActionImpl(QIcon(":/about.png"), QStringLiteral("关于"), 
		QStringLiteral("程序说明"), QKeySequence("F1"), SLOT(about()));
}

QAction* GrowthRecord::createActionImpl(const QIcon& icon, const QString& text, 
		const QString& statusTip, const QKeySequence& key, const char* slot )
{
	QAction* action = new QAction(icon, text, this);
	action->setStatusTip(statusTip);
	action->setShortcut(key);
	connect(action, SIGNAL(triggered()), this, slot);
	return action;
}

void GrowthRecord::createConnects()
{
}

void GrowthRecord::createMenus()
{
	fileMenu = menuBar()->addMenu(QStringLiteral("文件"));
	fileMenu->addAction(exportAction);
	fileMenu->addSeparator();
	fileMenu->addAction(closeAction);

	recordMenu = menuBar()->addMenu(QStringLiteral("记录"));
	recordMenu->addAction(personManageAction);
	recordMenu->addAction(showAction);
	recordMenu->addAction(updateAction);

	helpMenu = menuBar()->addMenu(QStringLiteral("帮助"));
	helpMenu->addAction(aboutAction);
}

void GrowthRecord::createToolBars()
{
	fileToolBar = addToolBar(QStringLiteral("文件"));
	fileToolBar->addAction(exportAction);
	fileToolBar->addSeparator();
	fileToolBar->addAction(closeAction);

	recordToolBar = addToolBar(QStringLiteral("记录"));
	recordToolBar->addAction(personManageAction);
	recordToolBar->addAction(showAction);
	recordToolBar->addAction(updateAction);
}

void GrowthRecord::createStatusBar()
{
	statusBar()->setStatusTip(QStringLiteral("准备就绪"));
}

void GrowthRecord::exportGrowthRecord()
{

}

void GrowthRecord::personManage()
{
	PresonManageDialog dialog;
	dialog.exec();
}

void GrowthRecord::showGrowth()
{

}

void GrowthRecord::updateGrowth()
{

}

void GrowthRecord::about()
{
	QMessageBox::about(this, QStringLiteral("成长记录"), 
		QStringLiteral("通过曲线和表格的形式展示成长变化记录"));
}
