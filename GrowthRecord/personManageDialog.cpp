
#include "personManageDialog.h"
#include "sql/sqlTable.h"

PresonManageDialog::PresonManageDialog( QWidget* parent /*= 0*/ )
{
	createDialog();
}

void PresonManageDialog::createDialog()
{
	initializeModel();
	createView();
	createButtons();
	createLayout();
	
	setWindowIcon(QIcon(":/personmanage.png"));
	setWindowTitle(QStringLiteral("人员信息"));
	resize(600, 400);
}

void PresonManageDialog::initializeModel()
{
	personModel.setTable("person");
	personModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
	personModel.select();

	for (int i = PersonTable::PersonColumn_Id; i != PersonTable::PersonColumn_Count; ++i)
	{
		personModel.setHeaderData(i, Qt::Horizontal, PersonTable::getColumnNames(i));
	}
}

void PresonManageDialog::createView()
{
	personView.setModel(&personModel);
	personView.setSelectionMode(QAbstractItemView::SingleSelection);
	personView.setSelectionBehavior(QAbstractItemView::SelectRows);
	personView.setColumnHidden(PersonTable::PersonColumn_Id, true);
	personView.resizeColumnsToContents();
	personView.setAlternatingRowColors(true);
	personView.setSortingEnabled(true);
	personView.setShowGrid(true);
}

void PresonManageDialog::createButtons()
{
	addButton = new QPushButton(QIcon(":/add.png"), QStringLiteral("增加"));
	connect(addButton, SIGNAL(clicked()), this, SLOT(addPerson()));

	deleteButton = new QPushButton(QIcon(":/delete.png"), QStringLiteral("删除"));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(deletePerson()));

	saveButton = new QPushButton(QIcon(":/save.png"), QStringLiteral("保存"));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(savePerson()));
}

void PresonManageDialog::createLayout()
{
	QHBoxLayout* buttonLayout = new QHBoxLayout();
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(addButton);
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(deleteButton);
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(saveButton);
	buttonLayout->addStretch(1);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(&personView);
	layout->addLayout(buttonLayout);
	setLayout(layout);
}

void PresonManageDialog::addPerson()
{
	int rows = personModel.rowCount();
	personModel.insertRow(rows);
	personModel.setData(personModel.index(rows, PersonTable::PersonColumn_Id), QUuid::createUuid().toString());
	personModel.submitAll();
	QModelIndex index = personModel.index(rows, PersonTable::PersonColumn_Name);
	personView.setCurrentIndex(index);
	personView.edit(index);
}

void PresonManageDialog::deletePerson()
{
	QModelIndex index = personView.currentIndex();
	if (!index.isValid())
	{
		return;
	}

	QSqlDatabase::database().transaction();
	QSqlRecord record = personModel.record(index.row());
	QString id = record.value(PersonTable::PersonColumn_Id).toString();
	int numGrowthRecords = 0;

	QSqlQuery query(QString("select count(*) from growth where personId = %1").arg(id));

	if (query.next())
	{
		numGrowthRecords = query.value(PersonTable::PersonColumn_Id).toInt();
	}
	if (numGrowthRecords > 0)
	{
		int r = QMessageBox::warning(this, QStringLiteral("删除人员信息"), 
				QStringLiteral("确认需要删除人员 %1 及其成长记录吗？").
				arg(record.value(PersonTable::PersonColumn_Name).toString()), 
				QMessageBox::Yes | QMessageBox::No);
		if (r == QMessageBox::No)
		{
			QSqlDatabase::database().rollback();
			return;
		}
		query.exec(QString("delete from growth where personId = %1").arg(id));
	}

	personModel.removeRow(index.row());
	personModel.submitAll();
	QSqlDatabase::database().commit();

	personView.setFocus();
}

void PresonManageDialog::savePerson()
{
	personModel.submitAll();
	personView.resizeColumnsToContents();
}
