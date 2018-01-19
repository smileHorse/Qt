#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QModelIndex>

class TreeModelCompleter;
QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QComboBox;
class QLabel;
class QLineEdit;
class QProgressBar;
class QCheckBox;
class QTreeView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);

private slots:
	void changeCase(int);
	void changeMode(int);
	void highlight(const QModelIndex&);
	void updateContentsLabel(const QString&);

private:
	QAbstractItemModel* modelFromFile(const QString& fileName);

	QTreeView* treeView;
	QComboBox* caseComboBox;
	QComboBox* modeComboBox;
	QLabel*	contentsLabel;
	TreeModelCompleter* completer;
	QLineEdit* lineEdit;
};

#endif // MAINWINDOW_H
