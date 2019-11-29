/*!
* \file FrameLine.h
*
* \author Masf
* \date ʮһ�� 2019
*
* �ָ���
*/

#include "FrameLine.h"

CFrameLine::CFrameLine(Qt::Orientation orientation, QWidget* parent /*= nullptr*/)
{
	this->setFrameShadow(QFrame::Raised);
	QFrame::Shape shape = orientation == Qt::Horizontal ? QFrame::HLine : QFrame::VLine;
	this->setFrameShape(shape);
}
