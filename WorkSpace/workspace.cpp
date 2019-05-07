#include "workspace.h"

#include <QtWidgets/QtWidgets>

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
	createLayout();
	createStatusBar();

	// 设置主界面背景
	this->setStyleSheet("background-image: url(./images/bk-ground.png);");

	// 默认最大化显示
	showMaximized();
}

QWidget* WorkSpace::createHeaderWidget()
{
	QWidget* widget = new QWidget();
	widget->setMinimumSize(QSize(100, 100));
	widget->setAutoFillBackground(true);	
	widget->setStyleSheet("background-image: url(./images/bk-header.png);");

	QLabel* label = new QLabel(QStringLiteral("..."));
	QPushButton* loginButton = new QPushButton(QStringLiteral("登录"), widget);
	loginButton->setFlat(true);
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addStretch();
	hLayout->addWidget(label);
	hLayout->addWidget(loginButton);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addStretch();
	layout->addLayout(hLayout);
	widget->setLayout(layout);
	return widget;
}

QWidget* WorkSpace::createBodyWidget()
{
	QWidget* widget = new QWidget();
	widget->setMinimumSize(QSize(100, 100));
	widget->setAutoFillBackground(true);	
	widget->setStyleSheet("border-image: url(./images/bk-body.png);");

	return widget;
}

void WorkSpace::createLayout()
{
	QWidget* widget = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout();
	QWidget* headerWidget = createHeaderWidget();
	if (headerWidget)
	{
		layout->addWidget(headerWidget);
	}
	
	QWidget* bodyWidget = createBodyWidget();
	if (bodyWidget)
	{
		layout->addWidget(bodyWidget, 1);
	}
	widget->setLayout(layout);
	setCentralWidget(widget);
}

void WorkSpace::createStatusBar()
{
	statusBar();
	statusBar()->showMessage(QStringLiteral("准备就绪"));
}
