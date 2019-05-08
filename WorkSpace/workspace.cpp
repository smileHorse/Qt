#include "workspace.h"

#include <QtWidgets/QtWidgets>

#include "LoginWidget.h"

WorkSpace::WorkSpace(QWidget *parent)
	: QMainWindow(parent)
{
	try 
	{
		createWidgets();
	}
	catch(...)
	{}
}

WorkSpace::~WorkSpace()
{

}

void WorkSpace::createWidgets()
{
	setWindowTitle(QApplication::applicationName());
	setWindowIcon(QIcon(":/images/main.png"));

	// 创建主界面

	m_loginWidget = new MLoginWidget();
	setCentralWidget(m_loginWidget);

	createStatusBar();
	
	// 默认最大化显示
	showMaximized();
}

void WorkSpace::createStatusBar()
{
	//statusBar()->showMessage(QStringLiteral("准备就绪"));
}
