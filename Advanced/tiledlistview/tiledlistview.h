#ifndef TILEDLISTVIEW_H
#define TILEDLISTVIEW_H

#include <QAbstractItemView>
#include <QHash>
#include <QRectF>

class TiledListView : public QAbstractItemView
{
	Q_OBJECT

public:
	explicit TiledListView(QWidget* parent = 0);

	void setModel(QAbstractItemModel* model);
	QRect visualRect(const QModelIndex &index) const;
	void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
	QModelIndex indexAt(const QPoint &point) const;

protected slots:
	void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
	void rowsInserted(const QModelIndex &parent, int start, int end);
	void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
	void updateGeometries();

protected:
	QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, 
		Qt::KeyboardModifiers modifiers);
	bool isIndexHidden(const QModelIndex &index) const { return false; }
	int horizontalOffset() const;
	int verticalOffset() const;
	void scrollContentsBy(int dx, int dy);
	void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command);
	QRegion visualRegionForSelection(const QItemSelection &selection) const;

	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);

private:
	void calculateRectsIfNecessary() const;
	QRectF viewportRectForRow(int row) const;
	void paintOutline(QPainter* painter, const QRectF& rectangle);

	mutable int idealWidth;
	mutable int idealHeight;
	mutable QHash<int, QRectF> rectForRow;
	mutable bool hashIsDirty;
};

#endif // TILEDLISTVIEW_H
