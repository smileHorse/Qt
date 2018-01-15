#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
QT_END_NAMESPACE

class ConfigDialog : public QDialog
{
	Q_OBJECT

public:
	ConfigDialog(QWidget *parent = 0);

public slots:
	void changePage(QListWidgetItem* current, QListWidgetItem* previous);

private:
	void createIcons();

	QListWidget* contentsWidget;
	QStackedWidget* pagesWidget;
};

#endif // CONFIGDIALOG_H
