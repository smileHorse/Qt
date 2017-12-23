#ifndef GROWTHWIDGET_H
#define GROWTHWIDGET_H

#include <QtSql/QtSql>
#include <QtWidgets/QtWidgets>

class GrowthCurveWidget;
class GrowthTableWidget;
class QComboBox;
class QLabel;
class QTabWidget;

class GrowthWidget : public QWidget
{
	Q_OBJECT

public:
	GrowthWidget(QWidget* parent = 0);

private:
	void initializeModel();
	void createWidget();
	void createLayout();

private:
	QSqlTableModel*	personModel;

	QLabel*	personLabel;
	QComboBox*	personComboBox;
	QTabWidget*	growthTabWidget;
	GrowthCurveWidget*	growthCurveWidget;
	GrowthTableWidget*	growthTableWidget;
};

#endif