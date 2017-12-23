
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

	setWindowTitle(QStringLiteral("�ɳ���¼"));
	setWindowIcon(QIcon(":/icon.png"));
	resize(1200, 800);
}

void GrowthRecord::createActions()
{
	exportAction = createActionImpl(QIcon(":/export.png"), QStringLiteral("����"), 
		QStringLiteral("�����ɳ���¼"), QKeySequence("Ctrl+E"), SLOT(exportGrowthRecord()));
	closeAction = createActionImpl(QIcon(":/close.png"), QStringLiteral("�ر�"), 
		QStringLiteral("�˳���ǰӦ�ó���"), QKeySequence::Quit, SLOT(close()));

	personManageAction = createActionImpl(QIcon(":/personmanage.png"), QStringLiteral("��Ա��Ϣ����"), 
		QStringLiteral("������Ա��Ϣ������ɾ����"), QKeySequence("Ctrl+P"), SLOT(personManage()));
	showAction = createActionImpl(QIcon(":/show.png"), QStringLiteral("��ʾ�ɳ���¼"), 
		QStringLiteral("��ʾ�ɳ���¼��Ϣ"), QKeySequence("F2"), SLOT(showGrowth()));
	updateAction = createActionImpl(QIcon(":/update.png"), QStringLiteral("ˢ�³ɳ���¼"), 
		QStringLiteral("ˢ�³ɳ���¼"), QKeySequence("F5"), SLOT(updateGrowth()));

	aboutAction = createActionImpl(QIcon(":/about.png"), QStringLiteral("����"), 
		QStringLiteral("����˵��"), QKeySequence("F1"), SLOT(about()));
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
	fileMenu = menuBar()->addMenu(QStringLiteral("�ļ�"));
	fileMenu->addAction(exportAction);
	fileMenu->addSeparator();
	fileMenu->addAction(closeAction);

	recordMenu = menuBar()->addMenu(QStringLiteral("��¼"));
	recordMenu->addAction(personManageAction);
	recordMenu->addAction(showAction);
	recordMenu->addAction(updateAction);

	helpMenu = menuBar()->addMenu(QStringLiteral("����"));
	helpMenu->addAction(aboutAction);
}

void GrowthRecord::createToolBars()
{
	fileToolBar = addToolBar(QStringLiteral("�ļ�"));
	fileToolBar->addAction(exportAction);
	fileToolBar->addSeparator();
	fileToolBar->addAction(closeAction);

	recordToolBar = addToolBar(QStringLiteral("��¼"));
	recordToolBar->addAction(personManageAction);
	recordToolBar->addAction(showAction);
	recordToolBar->addAction(updateAction);
}

void GrowthRecord::createStatusBar()
{
	statusBar()->setStatusTip(QStringLiteral("׼������"));
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
	QMessageBox::about(this, QStringLiteral("�ɳ���¼"), 
		QStringLiteral("ͨ�����ߺͱ�����ʽչʾ�ɳ��仯��¼"));
}
