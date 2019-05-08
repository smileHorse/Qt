#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QtWidgets/QMainWindow>


class QWidget;
class MLoginWidget;

class WorkSpace : public QMainWindow
{
	Q_OBJECT

public:
	WorkSpace(QWidget *parent = 0);
	~WorkSpace();

private:
	void createWidgets();
	void createStatusBar();

private:
	MLoginWidget* m_loginWidget;
};

#endif // WORKSPACE_H
