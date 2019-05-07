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

	// ����������
	createLayout();
	createStatusBar();

	// ���������汳��
	this->setStyleSheet("background-image: url(./images/bk-ground.png);");

	// Ĭ�������ʾ
	showMaximized();
}

QWidget* WorkSpace::createHeaderWidget()
{
	QWidget* widget = new QWidget();
	widget->setMinimumSize(QSize(100, 100));
	widget->setAutoFillBackground(true);	
	widget->setStyleSheet("background-image: url(./images/bk-header.png);");

	QLabel* label = new QLabel(QStringLiteral("..."));
	QPushButton* loginButton = new QPushButton(QStringLiteral("��¼"), widget);
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
	statusBar()->showMessage(QStringLiteral("׼������"));
}
