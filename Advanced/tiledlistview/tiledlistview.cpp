#include "tiledlistview.h"
#include <QApplication>
#include <QPaintEvent>
#include <QPainter>
#include <QScrollBar>
#include <QStyleOptionViewItem>

const int ExtraHeight = 3;

TiledListView::TiledListView(QWidget* parent /*= 0*/)
	: QAbstractItemView(parent), idealWidth(0), idealHeight(0),
	hashIsDirty(false)
{
	setFocusPolicy(Qt::WheelFocus);
	setFont(QApplication::font("QListView"));
	horizontalScrollBar()->setRange(0, 0);
	verticalScrollBar()->setRange(0, 0);
}

void TiledListView::setModel(QAbstractItemModel* model)
{
	QAbstractItemView::setModel(model);
	hashIsDirty = true;
}

QRect TiledListView::visualRect(const QModelIndex &index) const
{
	QRect rect;
	if (index.isValid())
	{
		rect = viewportRectForRow(index.row()).toRect();
	}
	return rect;
}

void TiledListView::scrollTo(const QModelIndex &index, ScrollHint hint /*= EnsureVisible*/)
{
	QRect viewRect = viewport()->rect();
	QRect itemRect = visualRect(index);

	if (itemRect.left() < viewRect.left())
	{
		horizontalScrollBar()->setValue(
			horizontalScrollBar()->value() + itemRect.left() - viewRect.left());
	}
	else if (itemRect.right() > viewRect.right())
	{
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() + 
			qMin(itemRect.right() - viewRect.right(), itemRect.left() - viewRect.left()));
	}
	if (itemRect.top() < viewRect.top())
	{
		verticalScrollBar()->setValue(verticalScrollBar()->value() + 
			itemRect.top() - viewRect.top());
	}
	else if (itemRect.bottom() > viewRect.bottom())
	{
		verticalScrollBar()->setValue(verticalScrollBar()->value() + 
			qMin(itemRect.bottom() - viewRect.bottom(), itemRect.top() - viewRect.top()));
	}
	viewport()->update();
}

QModelIndex TiledListView::indexAt(const QPoint &point_) const
{
	QPoint point(point_);
	point.rx() += horizontalScrollBar()->value();
	point.ry() += verticalScrollBar()->value();
	calculateRectsIfNecessary();
	QHashIterator<int, QRectF> i(rectForRow);
	while (i.hasNext()) {
		i.next();
		if (i.value().contains(point))
		{
			return model()->index(i.key(), 0, rootIndex());
		}
	}
	return QModelIndex();
}

void TiledListView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, 
	const QVector<int> &roles /*= QVector<int>()*/)
{
	hashIsDirty = true;
	QAbstractItemView::dataChanged(topLeft, bottomRight);
}

void TiledListView::rowsInserted(const QModelIndex &parent, int start, int end)
{
	hashIsDirty = true;
	QAbstractItemView::rowsInserted(parent, start, end);
}

void TiledListView::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
	hashIsDirty = true;
	QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

void TiledListView::updateGeometries()
{
	QFontMetrics fm(font());
	const int RowHeight = fm.height() + ExtraHeight;
	horizontalScrollBar()->setSingleStep(fm.width("n"));
	horizontalScrollBar()->setPageStep(viewport()->width());
	horizontalScrollBar()->setRange(0, qMax(0, idealWidth - viewport()->width()));
	verticalScrollBar()->setSingleStep(RowHeight);
	verticalScrollBar()->setPageStep(viewport()->height());
	verticalScrollBar()->setRange(0, qMax(0, idealHeight - viewport()->height()));
}

QModelIndex TiledListView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
	QModelIndex index = currentIndex();
	if (index.isValid())
	{
		if ((cursorAction == MoveLeft && index.row() > 0) || 
			(cursorAction == MoveRight && index.row() + 1 < model()->rowCount()))
		{
			const int offset = (cursorAction == MoveLeft ? -1 : 1);
			index = model()->index(index.row() + offset, index.column(), index.parent());
		}
		else if ((cursorAction == MoveUp && index.row() > 0) || 
			(cursorAction == MoveDown && index.row() + 1 < model()->rowCount()))
		{
			QFontMetrics fm(font());
			const int RowHeight = (fm.height() + ExtraHeight) * (cursorAction == MoveUp ? -1 : 1);
			QRect rect = viewportRectForRow(index.row()).toRect();
			QPoint point(rect.center().x(), rect.center().y() + RowHeight);
			while(point.x() >= 0) {
				index = indexAt(point);
				if (index.isValid())
				{
					break;
				}
				point.rx() -= fm.width("n");
			}
		}
	}
	return index;
}

int TiledListView::horizontalOffset() const
{
	return horizontalScrollBar()->value();
}

int TiledListView::verticalOffset() const
{
	return verticalScrollBar()->value();
}

void TiledListView::scrollContentsBy(int dx, int dy)
{
	scrollDirtyRegion(dx, dy);
	viewport()->scroll(dx, dy);
}

void TiledListView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags)
{
	QRect rectangle = rect.translated(horizontalScrollBar()->value(),
		verticalScrollBar()->value()).normalized();
	calculateRectsIfNecessary();
	QHashIterator<int, QRectF> i(rectForRow);
	int firstRow = model()->rowCount();
	int lastRow = -1;
	while (i.hasNext()) {
		i.next();
		if (i.value().intersects(rectangle))
		{
			firstRow = firstRow < i.key() ? firstRow : i.key();
			lastRow = lastRow > i.key() ? lastRow : i.key();
		}
	}
	if (firstRow != model()->rowCount() && lastRow != -1)
	{
		QItemSelection selection(model()->index(firstRow, 0, rootIndex()),
			model()->index(lastRow, 0, rootIndex()));
		selectionModel()->select(selection, flags);
	}
	else {
		QModelIndex invalid;
		QItemSelection selection(invalid, invalid);
		selectionModel()->select(selection, flags);
	}
}

QRegion TiledListView::visualRegionForSelection(const QItemSelection &selection) const
{
	QRegion region;
	foreach(const QItemSelectionRange& range, selection)
	{
		for (int row = range.top(); row <= range.bottom(); ++row)
		{
			for (int column = range.left(); column < range.right(); ++column)
			{
				QModelIndex index = model()->index(row, column, rootIndex());
				region += visualRect(index);
			}
		}
	}
	return region;
}

void TiledListView::paintEvent(QPaintEvent *event)
{
	QPainter painter(viewport());
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	for (int row = 0; row < model()->rowCount(rootIndex()); ++row)
	{
		QModelIndex index = model()->index(row, 0, rootIndex());
		QRectF rect = viewportRectForRow(row);
		if (!rect.isValid() || rect.bottom() < 0 || rect.y() > viewport()->height())
		{
			continue;
		}
		QStyleOptionViewItem option = viewOptions();
		option.rect = rect.toRect();
		if (selectionModel()->isSelected(index))
		{
			option.state |= QStyle::State_Selected;
		}
		if (currentIndex() == index)
		{
			option.state |= QStyle::State_HasFocus;
		}
		itemDelegate()->paint(&painter, option, index);
		paintOutline(&painter, rect);
	}
}

void TiledListView::resizeEvent(QResizeEvent *event)
{
	hashIsDirty = true;
	calculateRectsIfNecessary();
	updateGeometries();
}

void TiledListView::mousePressEvent(QMouseEvent *event)
{
	QAbstractItemView::mousePressEvent(event);
	setCurrentIndex(indexAt(event->pos()));
}

void TiledListView::calculateRectsIfNecessary() const
{
	if (!hashIsDirty)
	{
		return;
	}
	const int ExtraWidth = 10;
	QFontMetrics fm(font());
	const int RowHeight = fm.height() + ExtraHeight;
	const int MaxWidth = viewport()->width();
	int minimumWidth = 0;
	int x = 0;
	int y = 0;
	for (int row = 0; row < model()->rowCount(rootIndex()); ++row)
	{
		QModelIndex index = model()->index(row, 0, rootIndex());
		QString text = model()->data(index).toString();
		int textWidth = fm.width(text);
		if (!(x == 0 || x + textWidth + ExtraWidth < MaxWidth))
		{
			y += RowHeight;
			x = 0;
		}
		else if (x != 0)
		{
			x += ExtraWidth;
		}
		rectForRow[row] = QRectF(x, y, textWidth + ExtraWidth, RowHeight);
		if (textWidth > minimumWidth)
		{
			minimumWidth = textWidth;
		}
		x += textWidth;
	}
	idealWidth = minimumWidth + ExtraWidth;
	idealHeight = y + RowHeight;
	hashIsDirty = false;
	viewport()->update();
}

QRectF TiledListView::viewportRectForRow(int row) const
{
	calculateRectsIfNecessary();
	QRectF rect = rectForRow.value(row).toRect();
	if (!rect.isValid())
	{
		return rect;
	}
	return QRectF(rect.x() - horizontalScrollBar()->value(), 
		rect.y() - verticalScrollBar()->value(), rect.width(), rect.height());
}

void TiledListView::paintOutline(QPainter* painter, const QRectF& rectangle)
{
	const QRectF rect = rectangle.adjusted(0, 0, -1, -1);
	painter->save();
	painter->setPen(QPen(palette().dark().color(), 0.5));
	painter->drawRect(rect);
	painter->setPen(QPen(Qt::black, 0.5));
	painter->drawLine(rect.bottomLeft(), rect.bottomRight());
	painter->drawLine(rect.bottomRight(), rect.topRight());
	painter->restore();
}
