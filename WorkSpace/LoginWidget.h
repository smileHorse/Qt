/*!
 * \file LoginWidget.h
 *
 * \author Masf
 * \date ÎåÔÂ 2019
 *
 * µÇÂ¼´°¿Ú
 */

#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QtWidgets/QtWidgets>

class MLoginWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MLoginWidget(QWidget* parent = 0);
	virtual ~MLoginWidget();
	
protected:
	virtual void paintEvent(QPaintEvent* event);

private:
	void createWidgets();
	void createLayout();
	void createConnections();

private slots:
	void doLogin();

private:
	QLabel* m_iconLabel;

	QLineEdit* m_userLineEdit;
	QLineEdit* m_passwordLineEdit;
	QLabel* m_tipLabel;
	QPushButton* m_loginButton;

	QLabel* m_rightLabel;
};
#endif
