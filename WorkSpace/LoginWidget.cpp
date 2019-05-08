/*!
 * \file LoginWidget.h
 *
 * \author Masf
 * \date 五月 2019
 *
 * 登录窗口
 */

#include "LoginWidget.h"

MLoginWidget::MLoginWidget(QWidget* parent /*= 0*/)
{
	try
	{
		createWidgets();
		createLayout();
		createConnections();
	}
	catch (...)
	{
	}
}

MLoginWidget::~MLoginWidget()
{

}

void MLoginWidget::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}

void MLoginWidget::createWidgets()
{
	m_iconLabel = new QLabel();
	m_iconLabel->setPixmap(QPixmap("./images/main.png"));

	m_userLineEdit = new QLineEdit();
	m_userLineEdit->setFocus();
	m_passwordLineEdit = new QLineEdit();
	m_passwordLineEdit->setEchoMode(QLineEdit::Password);
	m_tipLabel = new QLabel();
	m_tipLabel->setVisible(false);
	m_loginButton = new QPushButton(QIcon(":/images/login.png"), QStringLiteral("登录"));

	m_rightLabel = new QLabel(QStringLiteral("程序权利归属于..."));

	setAutoFillBackground(true);
	QPalette pale = this->palette();
	pale.setBrush(backgroundRole(), QPixmap(":/images/bk-body.png"));
	//setPalette(pale);
}

void MLoginWidget::createLayout()
{
	QVBoxLayout* loginLayout = new QVBoxLayout();
	loginLayout->addWidget(new QLabel(QStringLiteral("用户名")));
	loginLayout->addWidget(m_userLineEdit);
	loginLayout->addWidget(new QLabel(QStringLiteral("密码")));
	loginLayout->addWidget(m_passwordLineEdit);
	loginLayout->addWidget(m_loginButton, 0, Qt::AlignCenter);
	QGroupBox* groupBox = new QGroupBox();
	groupBox->setLayout(loginLayout);

	QVBoxLayout* centerLayout = new QVBoxLayout();
	centerLayout->addStretch();
	centerLayout->addWidget(m_iconLabel, 0, Qt::AlignCenter);
	centerLayout->addWidget(groupBox);
	centerLayout->addSpacing(50);
	centerLayout->addWidget(m_rightLabel, 0, Qt::AlignCenter);
	centerLayout->addStretch();

	QHBoxLayout* layout = new QHBoxLayout();
	layout->addStretch();
	layout->addLayout(centerLayout);
	layout->addStretch();
	setLayout(layout);
}

void MLoginWidget::createConnections()
{
	connect(m_loginButton, SIGNAL(clicked()), this, SLOT(doLogin()));
}

void MLoginWidget::doLogin()
{

}
