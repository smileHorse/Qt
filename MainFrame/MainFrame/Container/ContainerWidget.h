/*!
 * \file ContainerWidget.h
 *
 * \author Masf
 * \date 十一月 2019
 *
 * 界面布局容器
 * 当含有顶栏容器或底栏容器时，全部元素垂直上下排列，否则水平左右排列
 */
#ifndef CONTAINERWIDGET_H
#define CONTAINERWIDGET_H

#include <QtWidgets/QtWidgets>

class CContainerWidget : public QWidget
{
	Q_OBJECT

public:
	CContainerWidget(QWidget* mainWidget, QWidget* parent = nullptr);

	CContainerWidget(QWidget* mainWidget, QWidget* asideWidget,
		QWidget* parent = nullptr);

	CContainerWidget(QWidget* mainWidget, QWidget* headerWidget,
		QWidget* footerWidget, QWidget* parent = nullptr);

	CContainerWidget(QWidget* mainWidget, QWidget* headerWidget,
		QWidget* footerWidget, QWidget* asideWidget,
		QWidget* parent = nullptr);

private:
	// 执行窗口布局
	void doLayout();

	// 设置主要区域容器
	void setMainWidget(QWidget* mainWidget);

	// 设置顶栏容器
	void setHeaderWidget(QWidget* headerWidget);

	// 设置底栏容器
	void setFooterWidget(QWidget* footerWidget);

	// 设置侧边栏容器
	void setAsideWidget(QWidget* asideWidget);

private:
	QWidget* m_mainWidget;
	QWidget* m_headerWidget;
	QWidget* m_footerWidget;
	QWidget* m_asideWidget;
};
#endif
