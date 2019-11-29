/*!
 * \file FrameLine.h
 *
 * \author Masf
 * \date 十一月 2019
 *
 * 分隔线
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