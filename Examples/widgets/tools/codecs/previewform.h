#ifndef PREVIEWFORM_H
#define PREVIEWFORM_H

#include <QDialog>
#include <QtCore/QList>

QT_BEGIN_NAMESPACE
class QComboBox;
class QDialogButtonBox;
class QLabel;
class QTextCodec;
class QTextEdit;
QT_END_NAMESPACE

class PreviewForm : public QDialog
{
	Q_OBJECT
public:
	PreviewForm(QWidget* parent = 0);

	void setCodecList(const QList<QTextCodec*>& list);
	void setEncodedData(const QByteArray& data);
	QString decodedString() const { return decodedStr; }

private slots:
	void updateTextEdit();

private:
	QByteArray encodedData;
	QString decodedStr;

	QComboBox* encodingComboBox;
	QLabel* encodingLabel;
	QTextEdit* textEdit;
	QDialogButtonBox* buttonBox;
};
#endif