
#include "pages.h"

ConfigurationPage::ConfigurationPage( QWidget* parent /*= 0*/ )
	: QWidget(parent)
{
	QGroupBox* configGroup = new QGroupBox(tr("Server configuration"));

	QLabel* serverLabel = new QLabel(tr("Server:"));
	QComboBox* serverComboBox = new QComboBox;
	serverComboBox->addItem(tr("Qt (Australia)"));
	serverComboBox->addItem(tr("Qt (Germany)"));
	serverComboBox->addItem(tr("Qt (Norway)"));
	serverComboBox->addItem(tr("Qt (People's Republic of China)"));
	serverComboBox->addItem(tr("Qt (USA)"));

	QHBoxLayout* serverLayout = new QHBoxLayout;
	serverLayout->addWidget(serverLabel);
	serverLayout->addWidget(serverComboBox);

	QVBoxLayout* configLayout = new QVBoxLayout;
	configLayout->addLayout(serverLayout);
	configGroup->setLayout(configLayout);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(configGroup);
	mainLayout->addStretch(1);
	setLayout(mainLayout);
}

QueryPage::QueryPage( QWidget* parent /*= 0*/ )
	: QWidget(parent)
{
	QGroupBox* packageGroup = new QGroupBox(tr("Look for packages"));

	QLabel* nameLabel = new QLabel(tr("Name:"));
	QLineEdit* nameLineEdit = new QLineEdit;

	QLabel* dateLabel = new QLabel(tr("Released after:"));
	QDateTimeEdit* dateEdit = new QDateTimeEdit(QDate::currentDate());

	QCheckBox* releasesCheckBox = new QCheckBox(tr("Releases"));
	QCheckBox* upgradesCheckBox = new QCheckBox(tr("Upgrades"));

	QSpinBox* hitsSpinBox = new QSpinBox;
	hitsSpinBox->setPrefix(tr("Return up to "));
	hitsSpinBox->setSuffix(tr(" results"));
	hitsSpinBox->setSpecialValueText(tr("Return only the first result"));
	hitsSpinBox->setMinimum(1);
	hitsSpinBox->setMaximum(100);
	hitsSpinBox->setSingleStep(10);

	QPushButton* startQueryButton = new QPushButton(tr("Start Query"));

	QGridLayout* packagesLayout = new QGridLayout;
	packagesLayout->addWidget(nameLabel, 0, 0);
	packagesLayout->addWidget(nameLineEdit, 0, 1);
	packagesLayout->addWidget(dateLabel, 1, 0);
	packagesLayout->addWidget(dateEdit, 1, 1);
	packagesLayout->addWidget(releasesCheckBox, 2, 0);
	packagesLayout->addWidget(upgradesCheckBox, 3, 0);
	packagesLayout->addWidget(hitsSpinBox, 4, 0, 1, 2);
	packageGroup->setLayout(packagesLayout);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(packagesLayout);
	mainLayout->addSpacing(12);
	mainLayout->addWidget(startQueryButton);
	mainLayout->addStretch(1);
	setLayout(mainLayout);
}

UpdatePage::UpdatePage( QWidget* parent /*= 0*/ )
	: QWidget(parent)
{
	QGroupBox* updateGroup = new QGroupBox(tr("Package selection"));
	QCheckBox* systemCheckBox = new QCheckBox(tr("Update system"));
	QCheckBox* appsCheckBox = new QCheckBox(tr("Update applications"));
	QCheckBox* docsCheckBox = new QCheckBox(tr("Update documentation"));

	QGroupBox* packageGroup = new QGroupBox(tr("Existing packages"));

	QListWidget* packageList = new QListWidget;
	QListWidgetItem* qtItem = new QListWidgetItem(packageList);
	qtItem->setText(tr("Qt"));
	QListWidgetItem* qsaItem = new QListWidgetItem(packageList);
	qsaItem->setText(tr("QSA"));
	QListWidgetItem* teamBuildItem = new QListWidgetItem(packageList);
	teamBuildItem->setText(tr("Teambuilder"));

	QPushButton* startUpdateButton = new QPushButton(tr("Start update"));

	QVBoxLayout* updateLayout = new QVBoxLayout;
	updateLayout->addWidget(systemCheckBox);
	updateLayout->addWidget(appsCheckBox);
	updateLayout->addWidget(docsCheckBox);
	updateGroup->setLayout(updateLayout);

	QVBoxLayout* packageLayout = new QVBoxLayout;
	packageLayout->addWidget(packageList);
	packageGroup->setLayout(packageLayout);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(updateGroup);
	mainLayout->addWidget(packageGroup);
	mainLayout->addSpacing(12);
	mainLayout->addWidget(startUpdateButton);
	mainLayout->addStretch(1);
	setLayout(mainLayout);
}
