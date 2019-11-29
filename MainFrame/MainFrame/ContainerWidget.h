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

class CAsideWidget;
class CFooterWidget;
class CHeaderWidget;
class CMainWidget;

class CContainerWidget : public QWidget
{
	Q_OBJECT

public:
	CContainerWidget(QWidget* parent = nullptr);

	CContainerWidget(CMainWidget* mainWidget, QWidget* parent = nullptr);

	CContainerWidget(CMainWidget* mainWidget, CAsideWidget* asideWidget, 
		QWidget* parent = nullptr);

	CContainerWidget(CMainWidget* mainWidget, CHeaderWidget* headerWidget,
		QWidget* parent = nullptr);

	// 执行窗口布局
	void doLayout();

	// 设置主要区域容器
	void setMainWidget(CMainWidget* mainWidget);

	// 设置顶栏容器
	void setHeaderWidget(CHeaderWidget* headerWidget);

	// 设置底栏容器
	void setFooterWidget(CFooterWidget* footerWidget);

	// 设置侧边栏容器
	void setAsideWidget(CAsideWidget* asideWidget);

private:
	CMainWidget* m_mainWidget;
	CHeaderWidget* m_headerWidget;
	CFooterWidget* m_footerWidget;
	CAsideWidget* m_asideWidget;
};
#endif
