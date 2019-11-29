#pragma once

#include <QtWidgets/QMainWindow>

class CContainerWidget;

class MainFrame : public QMainWindow
{
	Q_OBJECT

public:
	MainFrame(QWidget *parent = Q_NULLPTR);

private:
	void initFrame();

private:
	CContainerWidget* m_containerWidget;
};
