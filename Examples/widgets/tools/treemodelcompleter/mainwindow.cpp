
#include <QtWidgets/QtWidgets>

#include "mainwindow.h"
#include "treemodelcompleter.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	completer = new TreeModelCompleter(this);
	completer->setModel(modelFromFile(":/resources/treemodel.txt"));
	completer->setSeparator(QLatin1String("."));
	QObject::connect(completer, SIGNAL(highlighted(QModelIndex)),
		this, SLOT(highlight(QModelIndex)));

	QWidget *centralWidget = new QWidget;

	QLabel *modelLabel = new QLabel;
	modelLabel->setText(tr("Tree Model<br>(Double click items to edit)"));

	QLabel *modeLabel = new QLabel;
	modeLabel->setText(tr("Completion Mode"));
	modeComboBox = new QComboBox;
	modeComboBox->addItem(tr("Inline"));
	modeComboBox->addItem(tr("Filtered Popup"));
	modeComboBox->addItem(tr("Unfiltered Popup"));
	modeComboBox->setCurrentIndex(1);

	QLabel *caseLabel = new QLabel;
	caseLabel->setText(tr("Case Sensitivity"));
	caseComboBox = new QComboBox;
	caseComboBox->addItem(tr("Case Insensitive"));
	caseComboBox->addItem(tr("Case Sensitive"));
	caseComboBox->setCurrentIndex(0);
	//! [0]

	//! [1]
	QLabel *separatorLabel = new QLabel;
	separatorLabel->setText(tr("Tree Separator"));

	QLineEdit *separatorLineEdit = new QLineEdit;
	separatorLineEdit->setText(completer->separator());
	connect(separatorLineEdit, SIGNAL(textChanged(QString)),
		completer, SLOT(setSeparator(QString)));

	QCheckBox *wrapCheckBox = new QCheckBox;
	wrapCheckBox->setText(tr("Wrap around completions"));
	wrapCheckBox->setChecked(completer->wrapAround());
	connect(wrapCheckBox, SIGNAL(clicked(bool)), completer, SLOT(setWrapAround(bool)));

	contentsLabel = new QLabel;
	contentsLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(separatorLineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(updateContentsLabel(QString)));

	treeView = new QTreeView;
	treeView->setModel(completer->model());
	treeView->header()->hide();
	treeView->expandAll();
	//! [1]

	//! [2]
	connect(modeComboBox, SIGNAL(activated(int)), this, SLOT(changeMode(int)));
	connect(caseComboBox, SIGNAL(activated(int)), this, SLOT(changeCase(int)));

	lineEdit = new QLineEdit;
	lineEdit->setCompleter(completer);
	//! [2]

	//! [3]
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(modelLabel, 0, 0); layout->addWidget(treeView, 0, 1);
	layout->addWidget(modeLabel, 1, 0);  layout->addWidget(modeComboBox, 1, 1);
	layout->addWidget(caseLabel, 2, 0);  layout->addWidget(caseComboBox, 2, 1);
	layout->addWidget(separatorLabel, 3, 0); layout->addWidget(separatorLineEdit, 3, 1);
	layout->addWidget(wrapCheckBox, 4, 0);
	layout->addWidget(contentsLabel, 5, 0, 1, 2);
	layout->addWidget(lineEdit, 6, 0, 1, 2);
	centralWidget->setLayout(layout);
	setCentralWidget(centralWidget);

	changeCase(caseComboBox->currentIndex());
	changeMode(modeComboBox->currentIndex());

	setWindowTitle(tr("Tree Model Completer"));
	lineEdit->setFocus();
}

void MainWindow::changeCase(int cs)
{
	completer->setCaseSensitivity(cs ? Qt::CaseSensitive : Qt::CaseInsensitive);
}

void MainWindow::changeMode(int index)
{
	QCompleter::CompletionMode mode;
	if (index == 0)
	{
		mode = QCompleter::InlineCompletion;
	}
	else if (index == 1)
	{
		mode = QCompleter::PopupCompletion;
	}
	else
	{
		mode = QCompleter::UnfilteredPopupCompletion;
	}

	completer->setCompletionMode(mode);
}

void MainWindow::highlight( const QModelIndex& index )
{
	QAbstractItemModel* completionModel = completer->completionModel();
	QAbstractProxyModel* proxy = qobject_cast<QAbstractProxyModel*>(completionModel);
	if (!proxy)
	{
		return;
	}
	QModelIndex sourceIndex = proxy->mapToSource(index);
	treeView->selectionModel()->select(sourceIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
	treeView->scrollTo(index);
}

void MainWindow::updateContentsLabel( const QString& sep )
{
	contentsLabel->setText(tr("Type path from model above with items at each level separated by a '%1'").arg(sep));
}

QAbstractItemModel* MainWindow::modelFromFile(const QString& fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
	{
		return new QStringListModel(completer);
	}

#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
	QStringList words;

	QStandardItemModel* model = new QStandardItemModel(completer);
	QVector<QStandardItem*> parents(10);
	parents[0] = model->invisibleRootItem();

	while(!file.atEnd())
	{
		QString line = file.readLine();
		QString trimmedLine = line.trimmed();
		if (line.isEmpty() || trimmedLine.isEmpty())
		{
			continue;
		}

		QRegExp re("^\\s+");
		int nonws = re.indexIn(line);
		int level = 0;
		if (nonws == -1)
		{
			level = 0;
		}
		else
		{
			if (line.startsWith("\t"))
			{
				level = re.cap(0).length();
			}
			else
			{
				level = re.cap(0).length() / 4;
			}
		}

		if (level + 1 >= parents.size())
		{
			parents.resize(parents.size() * 2);
		}

		QStandardItem* item = new QStandardItem;
		item->setText(trimmedLine);
		parents[level]->appendRow(item);
		parents[level + 1] = item;
	}

#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

	return model;
}