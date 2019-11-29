/*!
* \file HeaderWidget.cpp
*
* \author Masf
* \date Ê®Ò»ÔÂ 2019
*
* ¶¥À¸ÈÝÆ÷
*/

#include "HeaderWidget.h"

CHeaderWidget::CHeaderWidget(QWidget* parent /*= nullptr*/)
	: QWidget(parent)
{

}

CHeaderWidget::CHeaderWidget(const QList<QWidget*>& childs, QWidget* parent /*= nullptr*/)
	: QWidget(parent)
{
	m_childs = childs;
	initWidget();
}

void CHeaderWidget::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CHeaderWidget::initWidget()
{
	QHBoxLayout* childLayout = new QHBoxLayout();
	childLayout->addStretch(1);
	for (auto child : m_childs)
	{
		if (child)
		{
			childLayout->addWidget(child);
		}
	}

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addStretch(1);
	layout->addLayout(childLayout);
	setLayout(layout);
}
