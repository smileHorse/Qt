
#include <QStringList>

#include "treemodelcompleter.h"

TreeModelCompleter::TreeModelCompleter( QObject* parent /*= 0*/ )
	: QCompleter(parent)
{

}

TreeModelCompleter::TreeModelCompleter( QAbstractItemModel* model, QObject* parent /*= 0*/ )
	: QCompleter(model, parent)
{

}

QString TreeModelCompleter::separator() const
{
	return sep;
}

void TreeModelCompleter::setSeparator( const QString& separator )
{
	sep = separator;
}

QStringList TreeModelCompleter::splitPath( const QString &path ) const 
{
	if (sep.isNull())
	{
		return QCompleter::splitPath(path);
	}
	return path.split(sep);
}

QString TreeModelCompleter::pathFromIndex( const QModelIndex& index ) const 
{
	if (sep.isNull())
	{
		return QCompleter::pathFromIndex(index);
	}

	QStringList dataList;
	for (QModelIndex i = index; i.isValid(); i = i.parent())
	{
		dataList.prepend(model()->data(i, completionRole()).toString());
	}
	return dataList.join(sep);
}
