/*!
 * \file FrameLine.h
 *
 * \author Masf
 * \date ʮһ�� 2019
 *
 * �ָ���
 */

#ifndef FRAMELINE_H
#define FRAMELINE_H

#include <QtWidgets/QFrame>


class CFrameLine : public QFrame
{
	Q_OBJECT

public:
	CFrameLine(Qt::Orientation orientation, QWidget* parent = nullptr);
};
#endif