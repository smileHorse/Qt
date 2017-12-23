#ifndef PERSONMANAGE_DIALOG_H
#define PERSONMANAGE_DIALOG_H

#include <QtSql/QtSql>
#include <QtWidgets/QtWidgets>

class PresonManageDialog : public QDialog
{
	Q_OBJECT

public:
	PresonManageDialog(QWidget* parent = 0);

private:
	void createDialog();
	void initializeModel();
	void createView();
	void createButtons();
	void createLayout();

private slots:
	void addPerson();
	void deletePerson();
	void savePerson();

private:
	QSqlTableModel	personModel;
	QTableView		personView;
	QPushButton*		addButton;
	QPushButton*		deleteButton;
	QPushButton*		saveButton;
};

#endif