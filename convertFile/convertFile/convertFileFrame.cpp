#include "convertFileFrame.h"
#include "convertFileThread.h"

ConvertFileFrame::ConvertFileFrame(QWidget *parent)
	: QMainWindow(parent)
{
	createWidgets();
	createConnects();
}

ConvertFileFrame::~ConvertFileFrame()
{

}

void ConvertFileFrame::createWidgets()
{
	QLabel* sourceLable = new QLabel(QStringLiteral("源:"));
	sourceLineEdit = new QLineEdit;
	sourcePushButton = new QPushButton("...");
	QHBoxLayout* sourceLayout = new QHBoxLayout;
	sourceLayout->addWidget(sourceLable);
	sourceLayout->addWidget(sourceLineEdit);
	sourceLayout->addWidget(sourcePushButton);

	QLabel* destLable = new QLabel(QStringLiteral("目的:"));
	destLineEdit = new QLineEdit;
	destPushButton = new QPushButton("...");
	QHBoxLayout* destLayout = new QHBoxLayout;
	destLayout->addWidget(destLable);
	destLayout->addWidget(destLineEdit);
	destLayout->addWidget(destPushButton);

	processTextEdit = new QTextEdit;
	QVBoxLayout* upLayout = new QVBoxLayout;
	upLayout->addLayout(sourceLayout);
	upLayout->addLayout(destLayout);
	upLayout->addWidget(processTextEdit);

	runButton = new QPushButton(QStringLiteral("开始"));
	closeButton = new QPushButton(QStringLiteral("关闭"));
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(runButton);
	buttonLayout->addWidget(closeButton);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addLayout(upLayout);
	layout->addLayout(buttonLayout);

	QWidget* widget = new QWidget;
	widget->setLayout(layout);
	setCentralWidget(widget);
}

void ConvertFileFrame::createConnects()
{
	connect(sourcePushButton, SIGNAL(clicked()), this, SLOT(chooseSource()));
	connect(destPushButton, SIGNAL(clicked()), this, SLOT(chooseDest()));
	connect(runButton, SIGNAL(clicked()), this, SLOT(convert()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

void ConvertFileFrame::chooseSource()
{
	QFileDialog dialog(this);
	dialog.setDirectory("D:/KH-8100T/project/localFile/warnData");
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setViewMode(QFileDialog::Detail);
	QStringList fileNames;
	if (dialog.exec())
	{
		fileNames = dialog.selectedFiles();
	}
	if (fileNames.isEmpty())
	{
		return;
	}
	
	QString filePath = fileNames.at(0);
	filePath.replace("\\", "/");
	sourceLineEdit->setText(filePath.left(filePath.lastIndexOf("/")));
	sourceFileNames = fileNames;
}

void ConvertFileFrame::chooseDest()
{
	QString destDir = QFileDialog::getExistingDirectory(this, QStringLiteral("选择目的文件夹"));
	if (destDir.isEmpty())
	{
		return;
	}

	destLineEdit->setText(destDir);
	destFolder = destDir;
}

void ConvertFileFrame::convert()
{
	ConvertFileThread* thread = new ConvertFileThread(sourceFileNames, destFolder);
	connect(thread, SIGNAL(process(const QString&)), this, SLOT(updateProcessList(const QString&)));
	connect(thread, SIGNAL(finished()), this, SLOT(convertFinished()));
	runButton->setEnabled(false);
	thread->start();
}

void ConvertFileFrame::updateProcessList( const QString& text )
{
	processTextEdit->moveCursor(QTextCursor::End);
	processTextEdit->insertPlainText("\n");
	processTextEdit->insertPlainText(text);
}

void ConvertFileFrame::convertFinished()
{
	runButton->setEnabled(true);
}
