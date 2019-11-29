/*!
 * \file ContainerWidget.cpp
 *
 * \author Masf
 * \date 十一月 2019
 *
 * 界面布局容器
 * 当含有顶栏容器或底栏容器时，全部元素垂直上下排列，否则水平左右排列
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

// 执行窗口布局
void CContainerWidget::doLayout()
{
	// 检查主要区域容器是否存在，若不存在，则设置为一个默认Label
	if (!m_mainWidget)
	{
		m_mainWidget = new QLabel(QStringLiteral("主要区域容器"));
	}

	// 检查侧边栏容器是否存在，若存在，则将侧边栏容器和主要区域容器进行分隔器布局
	if (m_asideWidget)
	{
		QSplitter* splitter = new QSplitter(Qt::Horizontal);
		splitter->addWidget(m_asideWidget);
		splitter->addWidget(m_mainWidget);
	}
	else
	{

	}

	// 将顶栏容器、底栏容器与主要区域容器进行垂直布局
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

// 设置主要区域容器
void CContainerWidget::setMainWidget(QWidget* mainWidget)
{
	m_mainWidget = mainWidget;
}

// 设置顶栏容器
void CContainerWidget::setHeaderWidget(QWidget* headerWidget)
{
	m_headerWidget = headerWidget;
}

// 设置底栏容器
void CContainerWidget::setFooterWidget(QWidget* footerWidget)
{
	m_footerWidget = footerWidget;
}

// 设置侧边栏容器
void CContainerWidget::setAsideWidget(QWidget* asideWidget)
{
	m_asideWidget = asideWidget;
}
