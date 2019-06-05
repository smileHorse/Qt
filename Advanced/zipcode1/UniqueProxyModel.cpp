
#include "UniqueProxyModel.h"

void UniqueProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
	connect(sourceModel, SIGNAL(modelReset()), this, SLOT(clearCache()));
	QSortFilterProxyModel::setSourceModel(sourceModel);
}

bool UniqueProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
	QModelIndex index = sourceModel()->index(source_row, Column, source_parent);
	const QString& text = sourceModel()->data(index).toString();
	if (cache.contains(text))
	{
		return false;
	}
	cache << text;
	return true;
}
