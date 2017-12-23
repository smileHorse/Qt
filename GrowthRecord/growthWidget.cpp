
#include "growthWidget.h"
#include "growthCurveWidget.h"
#include "growthTableWidget.h"

GrowthWidget::GrowthWidget( QWidget* parent /*= 0*/ )
{
	initializeModel();
	createWidget();
	createLayout();
}

void GrowthWidget::initializeModel()
{
	personModel = new QSqlTableModel();
	personModel->setTable("person");
}

void GrowthWidget::createWidget()
{
	personComboBox = new QComboBox();
	personComboBox->setModel(personModel);

	personLabel = new QLabel(QStringLiteral("人员信息"));
	personLabel->setBuddy(personComboBox);
	

	growthTabWidget = new QTabWidget;

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void GrowthWidget::createLayout()
{
	QHBoxLayout* topLayout = new QHBoxLayout;
	topLayout->addWidget(personLabel);
	topLayout->addWidget(personComboBox);
	topLayout->addStretch(3);
}
