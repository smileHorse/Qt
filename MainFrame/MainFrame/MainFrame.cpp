#include "MainFrame.h"
#include "Container/ContainerWidget.h"
#include "Container/HeaderWidget.h"

MainFrame::MainFrame(QWidget *parent)
	: QMainWindow(parent)
{
	initFrame();
	this->setWindowTitle(QStringLiteral("������"));
	setWindowIcon(QIcon(":/images/ams.png"));
}

void MainFrame::initFrame()
{
	// ��������
	QPushButton* loginButton = new QPushButton(QStringLiteral("��¼"));
	QPushButton* logoutButton = new QPushButton(QStringLiteral("�˳�"));
	QPushButton* passwordButton = new QPushButton(QStringLiteral("�޸�����"));
	QList<QWidget*> headerChilds;
	headerChilds << loginButton << logoutButton << passwordButton;

	CHeaderWidget* headerWidget = new CHeaderWidget(headerChilds, this);
	headerWidget->setFixedHeight(100);
	headerWidget->setStyleSheet("background-image: url(:/images/bk-header.png)");

	// ��Ҫ��������
	QWidget* mainWidget = new QWidget(this);
	mainWidget->setStyleSheet("background-image: url(:/images/bk-longin.png)");
	mainWidget->setMinimumHeight(300);
	
	m_containerWidget = new CContainerWidget(mainWidget, headerWidget, 0, 0, this);
	setCentralWidget(m_containerWidget);
}
