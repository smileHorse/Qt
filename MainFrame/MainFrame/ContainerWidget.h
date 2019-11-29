/*!
 * \file ContainerWidget.h
 *
 * \author Masf
 * \date ʮһ�� 2019
 *
 * ���沼������
 * �����ж����������������ʱ��ȫ��Ԫ�ش�ֱ�������У�����ˮƽ��������
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

	// ִ�д��ڲ���
	void doLayout();

	// ������Ҫ��������
	void setMainWidget(CMainWidget* mainWidget);

	// ���ö�������
	void setHeaderWidget(CHeaderWidget* headerWidget);

	// ���õ�������
	void setFooterWidget(CFooterWidget* footerWidget);

	// ���ò��������
	void setAsideWidget(CAsideWidget* asideWidget);

private:
	CMainWidget* m_mainWidget;
	CHeaderWidget* m_headerWidget;
	CFooterWidget* m_footerWidget;
	CAsideWidget* m_asideWidget;
};
#endif
