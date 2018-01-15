
#include <QtWidgets/QtWidgets>

#include "previewform.h"

PreviewForm::PreviewForm( QWidget* parent /*= 0*/ )
{
	encodingComboBox = new QComboBox;

	encodingLabel = new QLabel(tr("&Encoding:"));
	encodingLabel->setBuddy(encodingComboBox);

	textEdit = new QTextEdit;
	textEdit->setLineWrapMode(QTextEdit::NoWrap);
	textEdit->setReadOnly(true);

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(encodingComboBox, SIGNAL(activated(int)), this, SLOT(updateTextEdit()));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QGridLayout* mainLayout = new QGridLayout;
	mainLayout->addWidget(encodingLabel, 0, 0);
	mainLayout->addWidget(encodingComboBox, 0, 1);
	mainLayout->addWidget(textEdit, 1, 0, 1, 2);
	mainLayout->addWidget(buttonBox, 2, 0, 1, 2);
	setLayout(mainLayout);

	setWindowTitle(tr("Choose Encoding"));
	resize(400, 300);
}

void PreviewForm::setCodecList( const QList<QTextCodec*>& list )
{
	encodingComboBox->clear();
	foreach(QTextCodec* codec, list)
	{
		encodingComboBox->addItem(codec->name(), codec->mibEnum());
	}
}

void PreviewForm::setEncodedData( const QByteArray& data )
{
	encodedData = data;
	updateTextEdit();
}

void PreviewForm::updateTextEdit()
{
	int mib = encodingComboBox->itemData(encodingComboBox->currentIndex()).toInt();
	QTextCodec* codec = QTextCodec::codecForMib(mib);

	QTextStream in(&encodedData);
	in.setAutoDetectUnicode(false);
	in.setCodec(codec);
	decodedStr = in.readAll();

	textEdit->setPlainText(decodedStr);
}
