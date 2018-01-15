
#include <QtWidgets/QtWidgets>

#include "dialog.h"

Dialog::Dialog(QWidget *parent)
	: QDialog(parent)
{
	desktopGeometry = QApplication::desktop()->availableGeometry(0);

	setWindowTitle(tr("SIP Dialog Example"));
	QScrollArea* scrollArea = new QScrollArea(this);
	QGroupBox* groupBox = new QGroupBox(scrollArea);
	groupBox->setTitle(tr("SIP Dialog Example"));
	QGridLayout* gridLayout = new QGridLayout(groupBox);
	groupBox->setLayout(gridLayout);

	QLineEdit* lineEdit = new QLineEdit(groupBox);
	lineEdit->setText(tr("Open and close the SIP"));
	lineEdit->setMinimumWidth(220);

	QLabel* label = new QLabel(groupBox);
	label->setText(tr("This dialog resize if the SIP is opened"));
	label->setMinimumWidth(220);

	QPushButton* button = new QPushButton(groupBox);
	button->setText(tr("Close Dialog"));
	button->setMinimumWidth(220);

	if (desktopGeometry.height() < 400)
	{
		gridLayout->setVerticalSpacing(80);
	}
	else
	{
		gridLayout->setVerticalSpacing(150);
	}

	gridLayout->addWidget(label);
	gridLayout->addWidget(lineEdit);
	gridLayout->addWidget(button);

	scrollArea->setWidget(groupBox);
	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(scrollArea);
	setLayout(layout);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	connect(button, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
	connect(QApplication::desktop(), SIGNAL(workAreaResized(int)), 
			this, SLOT(desktopResized(int)));
}

void Dialog::reactToSIP()
{
	QRect availableGeometry = QApplication::desktop()->availableGeometry(0);

	if (desktopGeometry != availableGeometry)
	{
		if (windowState() | Qt::WindowMaximized)
		{
			setWindowState(windowState() & ~Qt::WindowMaximized);
		}

		setGeometry(availableGeometry);
	}

	desktopGeometry = availableGeometry;
}

void Dialog::desktopResized( int screen )
{
	if (screen != 0)
	{
		return;
	}
	reactToSIP();
}

