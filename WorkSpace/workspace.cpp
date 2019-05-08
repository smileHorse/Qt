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

	// ����������

	m_loginWidget = new MLoginWidget();
	setCentralWidget(m_loginWidget);

	createStatusBar();
	
	// Ĭ�������ʾ
	showMaximized();
}

void WorkSpace::createStatusBar()
{
	//statusBar()->showMessage(QStringLiteral("׼������"));
}
