/*!
* \file HeaderWidget.h
*
* \author Masf
* \date Ê®Ò»ÔÂ 2019
*
* µ×À¸ÈÝÆ÷
*/

#ifndef HEADERWIDGET_H
#define HEADERWIDGET_H

#include <QtWidgets/QtWidgets>
#include <QtCore/QList>

class CHeaderWidget : public QWidget
{
	Q_OBJECT

public:
	CHeaderWidget(QWidget* parent = nullptr);
	CHeaderWidget(const QList<QWidget*>& childs, QWidget* parent = nullptr);

protected:
	virtual void paintEvent(QPaintEvent * event);

private:
	void initWidget();

private:
	QList<QWidget*> m_childs;
};

#endif
