#ifndef FSMODEL_H
#define FSMODEL_H

#include <QtWidgets/QFileSystemModel>

class FileSystemModel : public QFileSystemModel
{
public:
	FileSystemModel(QObject* parent = 0);
	QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const Q_DECL_OVERRIDE;
};

#endif