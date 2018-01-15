#ifndef DIALOG_H
#define DIALOG_H

#include <QtWidgets/QDialog>

class Dialog : public QDialog
{
	Q_OBJECT

public:
	Dialog(QWidget *parent = 0);
	void reactToSIP();

private:
	QRect desktopGeometry;

public slots:
	void desktopResized(int screen);
};

#endif // DIALOG_H
