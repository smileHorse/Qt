
#include <QtWidgets/QtWidgets>

#include "fsmodel.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), completer(0), lineEdit(0)
{
	createMenu();

	QWidget* centralWidget = new QWidget;

	QLabel* modelLabel = new QLabel;
	modelLabel->setText(tr("Model"));

	modelCombo = new QComboBox;
	modelCombo->addItem(tr("QFileSystemModel"));
	modelCombo->addItem(tr("QFileSystemModel that shows full path"));
	modelCombo->addItem(tr("Country list"));
	modelCombo->addItem(tr("Word list"));
	modelCombo->setCurrentIndex(0);

	QLabel* modeLabel = new QLabel;
	modeLabel->setText(tr("Completion mode"));
	modeCombo = new QComboBox;
	modeCombo->addItem(tr("Inline"));
	modeCombo->addItem(tr("Filtered Popup"));
	modeCombo->addItem(tr("Unfiltered Popup"));
	modeCombo->setCurrentIndex(1);

	QLabel* caseLabel = new QLabel;
	caseLabel->setText(tr("Case Sensitivity"));
	caseCombo = new QComboBox;
	caseCombo->addItem(tr("Case Insensitive"));
	caseCombo->addItem(tr("Case Sensitive"));
	caseCombo->setCurrentIndex(0);

	QLabel* maxVisibleLable = new QLabel;
	maxVisibleLable->setText(tr("Max Visible Items"));
	maxVisibleSpinBox = new QSpinBox;
	maxVisibleSpinBox->setRange(3, 25);
	maxVisibleSpinBox->setValue(10);

	wrapCheckBox = new QCheckBox;
	wrapCheckBox->setText(tr("Wrap around completions"));
	wrapCheckBox->setChecked(true);

	contentsLabel = new QLabel;
	contentsLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	connect(modelCombo, SIGNAL(activated(int)), this, SLOT(changeModel()));
	connect(modeCombo, SIGNAL(activated(int)), this, SLOT(changeMode(int)));
	connect(caseCombo, SIGNAL(activated(int)), this, SLOT(changeCase(int)));
	connect(maxVisibleSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeMaxVisible(int)));

	lineEdit = new QLineEdit;

	QGridLayout* layout = new QGridLayout;
	layout->addWidget(modelLabel, 0, 0);
	layout->addWidget(modelCombo, 0, 1);
	layout->addWidget(modeLabel, 1, 0);
	layout->addWidget(modeCombo, 1, 1);
	layout->addWidget(caseLabel, 2, 0);
	layout->addWidget(caseCombo, 2, 1);
	layout->addWidget(maxVisibleLable, 3, 0);
	layout->addWidget(maxVisibleSpinBox, 3, 1);
	layout->addWidget(wrapCheckBox, 4, 0);
	layout->addWidget(contentsLabel, 5, 0, 1, 2);
	layout->addWidget(lineEdit, 6, 0, 1, 2);
	centralWidget->setLayout(layout);
	setCentralWidget(centralWidget);

	changeModel();

	setWindowTitle(tr("Completer"));
	lineEdit->setFocus();
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About"), tr("This example demonstrates the "
		"different features of the QCompleter class."));
}

void MainWindow::changeCase( int cs )
{
	completer->setCaseSensitivity(cs ? Qt::CaseSensitive : Qt::CaseInsensitive);
}

void MainWindow::changeMode( int index )
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

void MainWindow::changeModel()
{
	delete completer;
	completer = new QCompleter(this);
	completer->setMaxVisibleItems(maxVisibleSpinBox->value());

	switch(modelCombo->currentIndex())
	{
	default:
	case 0:
		{
			QFileSystemModel* fsModel = new QFileSystemModel(completer);
			fsModel->setRootPath("");
			completer->setModel(fsModel);
			contentsLabel->setText(tr("Enter file path"));
		}
		break;
	case 1:
		{
			FileSystemModel* fsModel = new FileSystemModel(completer);
			completer->setModel(fsModel);
			fsModel->setRootPath("");
			contentsLabel->setText(tr("Enter file path"));
		}
		break;
	case 2:
		{
			completer->setModel(modelFromFile(":/resources/countries.txt"));
			QTreeView *treeView = new QTreeView;
			completer->setPopup(treeView);
			treeView->setRootIsDecorated(false);
			treeView->header()->hide();
			treeView->header()->setStretchLastSection(false);
			treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
			treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
			contentsLabel->setText(tr("Enter name of your country"));
		}
		break;
	case 3:
		{
			completer->setModel(modelFromFile(":/resources/wordlist.txt"));
			completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
			contentsLabel->setText(tr("Enter a word"));
		}
		break;
	}

	changeMode(modeCombo->currentIndex());
	changeCase(caseCombo->currentIndex());
	completer->setWrapAround(wrapCheckBox->isChecked());
	lineEdit->setCompleter(completer);
	connect(wrapCheckBox, SIGNAL(clicked(bool)), completer, SLOT(setWrapAround(bool)));
}

void MainWindow::changeMaxVisible( int max )
{
	completer->setMaxVisibleItems(max);
}

void MainWindow::createMenu()
{
	QAction* exitAction = new QAction(tr("&Exit"), this);
	QAction* aboutAction = new QAction(tr("About"), this);
	QAction* aboutQtAction = new QAction(tr("About Qt"), this);

	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
	connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	QMenu* fileMenu = menuBar()->addMenu(tr("File"));
	fileMenu->addAction(exitAction);

	QMenu* helpMenu = menuBar()->addMenu(tr("Help"));
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
}

QAbstractItemModel* MainWindow::modelFromFile( const QString& fileName )
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
	{
		return new QStringListModel(completer);
	}

#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
	QStringList words;

	while(!file.atEnd())
	{
		QByteArray line = file.readLine();
		if (!line.isEmpty())
		{
			words << line.trimmed();
		}
	}

#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

	if (!fileName.contains(QLatin1String("countries.txt")))
	{
		return new QStringListModel(words, completer);
	}

	QStandardItemModel* m = new QStandardItemModel(words.count(), 2, completer);
	for (int i = 0; i < words.count(); ++i)
	{
		QModelIndex countryIdx = m->index(i, 0);
		QModelIndex symbolIdx = m->index(i, 1);
		QString country = words[i].mid(0, words[i].length() - 2).trimmed();
		QString symbol = words[i].right(2);
		m->setData(countryIdx, country);
		m->setData(symbolIdx, symbol);
	}

	return m;
}
