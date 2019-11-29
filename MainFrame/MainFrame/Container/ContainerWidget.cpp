/*!
 * \file ContainerWidget.cpp
 *
 * \author Masf
 * \date ʮһ�� 2019
 *
 * ���沼������
 * �����ж����������������ʱ��ȫ��Ԫ�ش�ֱ�������У�����ˮƽ��������
 */

#include "ContainerWidget.h"
#include "FrameLine.h"

CContainerWidget::CContainerWidget(QWidget* mainWidget, QWidget* parent /*= nullptr*/)
	: QWidget(parent), m_mainWidget(mainWidget), m_headerWidget(0), m_footerWidget(0), 
	m_asideWidget(0)
{
	doLayout();
}

CContainerWidget::CContainerWidget(QWidget* mainWidget, QWidget* asideWidget, 
	QWidget* parent /*= nullptr*/)
	: QWidget(parent), m_mainWidget(mainWidget), m_headerWidget(0), m_footerWidget(0),
	m_asideWidget(asideWidget)
{
	doLayout();
}

CContainerWidget::CContainerWidget(QWidget* mainWidget, QWidget* headerWidget, 
	QWidget* footerWidget, QWidget* parent /*= nullptr*/)
	: QWidget(parent), m_mainWidget(mainWidget), m_headerWidget(headerWidget), 
	m_footerWidget(footerWidget), m_asideWidget(0)
{
	doLayout();
}

CContainerWidget::CContainerWidget(QWidget* mainWidget, QWidget* headerWidget, 
	QWidget* footerWidget, QWidget* asideWidget, QWidget* parent /*= nullptr*/)
	: QWidget(parent), m_mainWidget(mainWidget), m_headerWidget(headerWidget), 
	m_footerWidget(footerWidget), m_asideWidget(asideWidget)
{
	doLayout();
}

// ִ�д��ڲ���
void CContainerWidget::doLayout()
{
	// �����Ҫ���������Ƿ���ڣ��������ڣ�������Ϊһ��Ĭ��Label
	if (!m_mainWidget)
	{
		m_mainWidget = new QLabel(QStringLiteral("��Ҫ��������"));
	}

	// ������������Ƿ���ڣ������ڣ��򽫲������������Ҫ�����������зָ�������
	if (m_asideWidget)
	{
		QSplitter* splitter = new QSplitter(Qt::Horizontal);
		splitter->addWidget(m_asideWidget);
		splitter->addWidget(m_mainWidget);
	}
	else
	{

	}

	// ������������������������Ҫ�����������д�ֱ����
	QVBoxLayout* layout = new QVBoxLayout();
	if (m_headerWidget)
	{
		layout->addWidget(m_headerWidget);
		layout->addWidget(new CFrameLine(Qt::Horizontal));
	}

	layout->addWidget(m_mainWidget, 1);

	if (m_footerWidget)
	{
		layout->addWidget(new CFrameLine(Qt::Horizontal));
		layout->addWidget(m_footerWidget);
	}
	setLayout(layout);
}

// ������Ҫ��������
void CContainerWidget::setMainWidget(QWidget* mainWidget)
{
	m_mainWidget = mainWidget;
}

// ���ö�������
void CContainerWidget::setHeaderWidget(QWidget* headerWidget)
{
	m_headerWidget = headerWidget;
}

// ���õ�������
void CContainerWidget::setFooterWidget(QWidget* footerWidget)
{
	m_footerWidget = footerWidget;
}

// ���ò��������
void CContainerWidget::setAsideWidget(QWidget* asideWidget)
{
	m_asideWidget = asideWidget;
}
