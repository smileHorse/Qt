
#include "fsmodel.h"

FileSystemModel::FileSystemModel( QObject* parent /*= 0*/ )
	: QFileSystemModel(parent)
{

}

QVariant FileSystemModel::data( const QModelIndex &index, int role /* = Qt::DisplayRole */ ) const
{
	if (role == Qt::DisplayRole && index.column() == 0)
	{
		QString path = QDir::toNativeSeparators(filePath(index));
		if (path.endsWith(QDir::separator()))
		{
			path.chop(1);
		}
		return path;
	}
	return QFileSystemModel::data(index, role);
}
