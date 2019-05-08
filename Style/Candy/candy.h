#ifndef CANDY_H
#define CANDY_H

#include <QtWidgets/QDialog>

class QCheckBox;
class QComboBox;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QListWidget;

class Candy : public QDialog
{
	Q_OBJECT

public:
	Candy(QWidget *parent = 0);
	~Candy();

private:
	QLabel* nameLabel;
	QLabel* passwordLabel;
	QLabel* countryLabel;
	QLabel* professionLabel;
	QComboBox* nameComboBox;
	QLineEdit* passwordLineEdit;
	QComboBox* countryComboBox;
	QListWidget* professionListWidget;
	QCheckBox* acceptCheckBox;
	QDialogButtonBox* buttonBox;
};

#endif // CANDY_H
