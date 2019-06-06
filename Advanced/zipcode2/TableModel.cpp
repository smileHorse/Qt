
#include "aqp.hpp"
#include "global.h"
#include "TableModel.h"
#include <QApplication>
#include <QDataStream>
#include <QFile>
#include <QFontMetrics>
#include <QStyleOptionComboBox>

const qint32 MagicNumber = 0x5A697043;
const qint16 FormatNumber = 100;
const int MaxColumns = 4;

QDataStream& operator<<(QDataStream& out, const ZipcodeItem& item)
{
	out << static_cast<quint16>(item.zipcode) << item.postOffice <<
		item.county << item.state;
	return out;
}

QDataStream& operator>>(QDataStream& in, ZipcodeItem& item)
{
	quint16 zipcode;
	in >> zipcode >> item.postOffice >> item.county >> item.state;
	item.zipcode = static_cast<int>(zipcode);
	return in;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
	if (index.isValid())
	{
		theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
	}
	return theFlags;
}

QVariant TableModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
	if (!index.isValid() || 
		index.row() < 0 || index.row() > zipcodes.count() ||
		index.column() < 0 || index.column() > MaxColumns)
	{
		return QVariant();
	}

	const ZipcodeItem& item = zipcodes.at(index.row());
	if (role == Qt::SizeHintRole)
	{
		QStyleOptionComboBox option;
		switch (index.column())
		{
		case Zipcode:
			{
				option.currentText = QString::number(MaxZipcode);
				const QString header = headerData(Zipcode, Qt::Horizontal, Qt::DisplayRole).toString();
				if (header.length() > option.currentText.length())
				{
					option.currentText = header;
				}
				break;
			}
		case PostOffice:
			option.currentText = item.postOffice;
			break;
		case County:
			option.currentText = item.county;
			break;
		case State:
			option.currentText = item.state;
			break;
		default:
			Q_ASSERT(false);
		}
		QFontMetrics fontMetrics(data(index, Qt::FontRole).value<QFont>());
		option.fontMetrics = fontMetrics;
		QSize size(fontMetrics.width(option.currentText), fontMetrics.height());
		return qApp->style()->sizeFromContents(QStyle::CT_ComboBox, &option, size);
	}
	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		switch (index.column())
		{
		case Zipcode: return item.zipcode;
		case PostOffice: return item.postOffice;
		case County: return item.county;
		case State: return item.state;
		default: Q_ASSERT(false);
		}
	}
	return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (role != Qt::DisplayRole)
	{
		return QVariant();
	}
	if (orientation == Qt::Horizontal)
	{
		switch (section)
		{
		case Zipcode: return "Zipcode";
		case PostOffice: return "PostOffice";
		case County: return "County";
		case State: return "State";
		default: Q_ASSERT(false);
		}
	}
	return section + 1;
}

int TableModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return parent.isValid() ? 0 : zipcodes.count();
}

int TableModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return parent.isValid() ? 0 : MaxColumns;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole*/)
{
	if (!index.isValid() || 
		index.row() < 0 || index.row() > zipcodes.count() ||
		index.column() < 0 || index.column() > MaxColumns)
	{
		return false;
	}

	ZipcodeItem& item = zipcodes[index.row()];
	switch (index.column())
	{
	case Zipcode:
		{
			bool ok;
			int zipcode = value.toInt(&ok);
			if (!ok || zipcode < MinZipcode || zipcode > MaxZipcode)
			{
				return false;
			}
			item.zipcode = zipcode;
			break;
		}
	case PostOffice:
		item.postOffice = value.toString();
		break;
	case County:
		item.county = value.toString();
		break;
	case State:
		item.state = value.toString();
		break;
	default:
		Q_ASSERT(false);
	}
	emit dataChanged(index, index);
	return true;
}

bool TableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role /*= Qt::EditRole*/)
{
	return false;
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent /*= QModelIndex()*/)
{
	beginInsertRows(parent, row, row + count - 1);
	for (int i = 0; i < count; ++i)
	{
		zipcodes.insert(row, ZipcodeItem());
	}
	endInsertRows();
	return true;
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent /*= QModelIndex()*/)
{
	beginRemoveRows(parent, row, row + count - 1);
	for (int i = 0; i < count; ++i)
	{
		zipcodes.removeAt(row);
	}
	endRemoveRows();
	return true;
}

void TableModel::load(const QString& filename /*= QString()*/)
{
	if (!filename.isEmpty())
	{
		m_filename = filename;
	}
	if (m_filename.isEmpty())
	{
		throw AQP::Error("no filename specified");
	}
	QFile file(m_filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		throw AQP::Error(file.errorString());
	}

	QDataStream in(&file);
	qint32 magicNumber;
	in >> magicNumber;
	if (magicNumber != MagicNumber)
	{
		throw AQP::Error("unrecognized file type");
	}
	qint16 formatVersionNumber;
	in >> formatVersionNumber;
	if (formatVersionNumber > FormatNumber)
	{
		throw AQP::Error("file format version is too new");
	}
	in.setVersion(QDataStream::Qt_4_5);
	beginResetModel();
	zipcodes.clear();

	ZipcodeItem item;
	while(!in.atEnd()) 
	{
		in >> item;
		zipcodes << item;
	}
	qSort(zipcodes);
	endResetModel();
}

void TableModel::save(const QString& filename /*= QString()*/)
{
	if (!filename.isEmpty())
		m_filename = filename;
	if (m_filename.isEmpty())
		throw AQP::Error(tr("no filename specified"));
	QFile file(m_filename);
	if (!file.open(QIODevice::WriteOnly))
		throw AQP::Error(file.errorString());

	QDataStream out(&file);
	out << MagicNumber << FormatNumber;
	out.setVersion(QDataStream::Qt_4_5);
	QListIterator<ZipcodeItem> i(zipcodes);
	while (i.hasNext())
		out << i.next();
}
