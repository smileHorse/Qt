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
	// ִ�д��ڲ���
	void doLayout();

	// ������Ҫ��������
	void setMainWidget(QWidget* mainWidget);

	// ���ö�������
	void setHeaderWidget(QWidget* headerWidget);

	// ���õ�������
	void setFooterWidget(QWidget* footerWidget);

	// ���ò��������
	void setAsideWidget(QWidget* asideWidget);

private:
	QWidget* m_mainWidget;
	QWidget* m_headerWidget;
	QWidget* m_footerWidget;
	QWidget* m_asideWidget;
};
#endif
