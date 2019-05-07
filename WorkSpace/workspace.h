#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QtWidgets/QMainWindow>


class QWidget;

class WorkSpace : public QMainWindow
{
	Q_OBJECT

public:
	WorkSpace(QWidget *parent = 0);
	~WorkSpace();

private:
	void createWidgets();
	QWidget* createHeaderWidget();
	QWidget* createBodyWidget();
	void createLayout();
	void createStatusBar();

private:
};

#endif // WORKSPACE_H
