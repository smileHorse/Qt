# QT的模型/视图架构

​	QT提供了几个窗口部件用于显示存储在模型中的那些数据项。

​	有几种纯粹的**视图部件**：`QListView`、`QTableView`、`QColumnView`(能够以列表的横向序列方式来显示树型层次结构的视图)和`QTreeView`。所有这些视图都必须提供一个模型(自定义的，或是QT提供的)来与之配合。

​	QT提供了一些便利**窗口部件**(它们提供了自己的内置模型并能直接使用)，如`QListWidget`、`QTableWidget`和`QTreeWidget`。还有`QComboBox`，它既是一个便利窗口部件也是一个视图部件，我们能直接使用它(因为它提供了内置模型)，也能把它当成一个模型的视图部件(这种情况下，可以提供一个合适的模型给它)。

​	所有的标准视图都提供了一个默认的`QStyledItemDelegate`委托 -- 该类显示视图的各个项，并为可编辑的项提供一个合适的编辑器。我们也能创建自己的委托，来控制视图的各个项的显示和编辑。

​	关于模型、视图、委托以及其背后的数据之间的关系，如下图所示：

​	![1559699099552](.\images\1559699099552.png)

​	*在一些环境中，特别是对于那些数据集比较小的应用程序，假如它们使用的数据在任何时候仅仅显示在一个部件中，那么选择便利窗口部件比较合适。*

​	尽管有统一的`API`，仍然有两种不同类型的模型：**有行列特性的表格模型**，**有父子关系的树模型**。

​	除了考虑表格模型和树模型之外，还有三类模型可能用到：

- QT预定义的一些模型，如`QStringListModel`、`QDirModel`、还有`QDirModel`的后继者`QFileSystemModel`。这些模型可以直接使用并且只需做很少的工作。

- `QStandardItemModel`,它是一个可被当做列表模型、表格模型或树模型来使用的通用模型。它提供了一个基于项的`API`。对于那些能够直接把数据恰好地填充入列表、表格或树模型的各个数据项中，并能够直接使用或仅需做微小改动即可的情况，使用`QStandardItemModel`是理想的选择。

- 最后一种是从`QAbstractItemModel`或`QAbstractListModel` 、`QAbstractTableModel`派生而来的自定义模型。如果想达到最佳的性能，或者该模型不符合基于项的特征，那就需要使用自定义模型。

  一些QT模型的层次结构如下图所示：

  ![1559699874831](.\images\1559699874831.png)

  使用`QStandardItemModel`的话，就不需要再创建自定义的模型了。但它有两个潜在的缺点：

- 首先，在加载大数据集时它明显比自定义模型慢。

- 其次，对于树模型来说，它提供的API不像自定义模型那样能有那么多的功能。

  大部分情况下，可以先使用`QStandardItemModel`，等到必要的时候再实现一个自定义模型来代替它。

  ## 1、 表格模型

  ​	表格模型按照行和列进行工作，每一项的父项都是一个无效的`QModelIndex`对象。列表模型和表格模型没有本质的不同：列表模型仅仅是只有一列的表格模型而已。

  ### 1.1 用于表格的`QStandardItemModel`

  #### 使用模型的`QComboBox`

  ~~~c++
  void MainWindow::createComboBoxModel(QComboBox *comboBox, int column)
  {
  	delete comboBox->model();
  	UniqueProxyModel *uniqueProxyModel = new UniqueProxyModel(column,
  		this);
  	uniqueProxyModel->setSourceModel(model);
  	uniqueProxyModel->sort(column, Qt::AscendingOrder);
  	comboBox->setModel(uniqueProxyModel);
  	comboBox->setModelColumn(column);
  }
  ~~~

  在创建一个新的组合框模型前应该先删除其内置的旧模型。

  自定义模型`UniqueProxyModel`与底层模型(`model`)使用了同样的列，在创建代理(`proxy`)时必须明确要对哪一列进行唯一值过滤。设置代理的源模型(`Source Model`)为底层模型`model`，设置组合框的模型为`proxy`。组合框的下拉列表框内应显示通过`QComboBox::setModelColumn()`方法设置指定列。

  #### 视图的选集模型

  视图的选集模型通过`QAbstractItemView::selectionModel()`方法来获取，它的类型是`QItemSelectionModel`(它是`QObject`的直接子类，而不是`QAbstractItemModel`的子类)。

  #### 从视图中选择对象

  ~~~c++
  QItemSelection selection;
  int firstSelectedRow = -1;
  for (int row = 0; row < proxyModel->rowCount(); ++row) {
      QModelIndex index = proxyModel->index(row, Zipcode);
      int zipcode = proxyModel->data(index).toInt();
      if (zipcode < minimumZipcode || zipcode > maximumZipcode)
          continue;
      if (!matchingColumn(county, row, County))
          continue;
      if (!matchingColumn(state, row, State))
          continue;
      if (firstSelectedRow == -1)
          firstSelectedRow = row;
      QItemSelection rowSelection(index, index);
      selection.merge(rowSelection, QItemSelectionModel::Select);
  }
  QItemSelectionModel *selectionModel = tableView->selectionModel();
  selectionModel->clearSelection();
  selectionModel->select(selection, QItemSelectionModel::Rows|
                         QItemSelectionModel::Select);
  if (firstSelectedRow != -1)
      tableView->scrollTo(proxyModel->index(firstSelectedRow, 0));
  ~~~

  ​	选集对象必须一行一行的构造。首先声明一个空的`QItemSelection`对象，然后遍历代理模型中的每一行，以构造`QItemSelection`对象。当有一行符合用户所设置的条件时，则创建一个包含该行的`QItemSelection`对象，然后把它并入我们创建的打算包含所有选择行的`QItemSelection`对象中。

  ​	一旦处理完所有行，就清除现存的选集模型，然后选中符合用户所选条件的行。最后，滚动到所选行的第一条。

  #### 添加模型数据

  ~~~c++
  void MainWindow::addZipcode()
  {
  	dontFilterOrSelectRadioButton->click();
  #ifdef CUSTOM_MODEL
  	if (!model->insertRow(model->rowCount()))
  		return;
  #else
  	QList<QStandardItem*> items;
  	QStandardItem *zipItem = new QStandardItem;
  	zipItem->setData(MinZipcode, Qt::EditRole);
  	items << zipItem;
  	for (int i = 0; i < model->columnCount() - 1; ++i)
  		items << new QStandardItem(tr("(Unknown)"));
  	model->appendRow(items);
  #endif
  	tableView->scrollToBottom();
  	tableView->setFocus();
  	QModelIndex index = proxyModel->index(proxyModel->rowCount() - 1,
  		Zipcode);
  	tableView->setCurrentIndex(index);
  	tableView->edit(index);
  }
  ~~~

  ​	因为使用了简单的`QStandardItemModel`派生类以及将数据保存于`QStandardItem`对象，添加一条数据的操作只要适当地初始化一行的几个`QStandardItem`并把新行添加到模型中即可。新数据被添加到底层模型中，视图使用的代理模型能够自动探测这种情形并与之适应。

  ​	对于非字符串数据，比如邮编，明智的做法是明确地指定数据的存储角色为`Qt::EditRole`，这样在`Qt::DisplayRole`被请求的时候令Qt自动产生一个相应的字符串来显示数据。

  #### 删除模型数据

  ~~~c++
  void MainWindow::deleteZipcode()
  {
  	QItemSelectionModel *selectionModel = tableView->selectionModel();
  	if (!selectionModel->hasSelection())
  		return;
  	QModelIndex index = proxyModel->mapToSource(
  		selectionModel->currentIndex());
  	if (!index.isValid())
  		return;
  	int zipcode = model->data(model->index(index.row(),
  		Zipcode)).toInt();
  	if (!AQP::okToDelete(this, tr("Delete Zipcode"),
  		tr("Delete Zipcode %1?").arg(zipcode, 5, 10, QChar('0'))))
  		return;
  	bool filtered = filterRadioButton->isChecked();
  	bool selected = selectByCriteriaRadioButton->isChecked();
  	QString county = countyGroupBox->isChecked()
  		? countyComboBox->currentText() : QString();
  	QString state = stateGroupBox->isChecked()
  		? stateComboBox->currentText() : QString();
  	dontFilterOrSelectRadioButton->click();
  
  	model->removeRow(index.row(), index.parent());
  
  	createComboBoxModels();
  	if (!county.isEmpty())
  		countyComboBox->setCurrentIndex(
  		countyComboBox->findText(county));
  	if (!state.isEmpty())
  		stateComboBox->setCurrentIndex(
  		stateComboBox->findText(state));
  	if (filtered)
  		filterRadioButton->click();
  	else if (selected)
  		selectByCriteriaRadioButton->click();
  }
  ~~~

  #### 对话框的使用

  ​	理论上可以通过调用`QWidget::setAttribute(Qt::WA_DeleteOnClose)`方法来使得对话框在关闭后自动销毁，但在实践中建议把这个工作留给智能指针去处理。这就意味着消息框在点击按钮时，仅仅是被关闭了而没有被销毁，因此在`QMessageBox::exec()`方法调用返回后，信息框仍然存在。

  ​	对于大型工程而言，使用`QDialog::exec()`方法已经过时了。问题在于，尽管从用户的角度来看它是一个阻塞式调用(也就是说，它阻止了用户与应用程序中其他窗体的交互)，但它并没有阻塞事件处理。这意味着程序的状态可以再调用`exec()`方法和用户接受或拒绝对话框的动作之间发生重大改变，甚至连对话框本身被意外删除也是有可能的。考虑到这个问题，使用`QDialog::open()`，或者对于非模态对话框使用`QDialog::show()`，并且使用信号-槽连接来处理用户对于对话框的响应是更安全的办法。

  #### `QStandardItemModel`派生类

  ​	`QStandardItemModel`类提供了操作表格数据及与视图交互的所有功能。这个例子中派生类只增加了加载数据和保存数据的功能。

  ​	**加载数据：**

  ~~~c++
  void StandardTableModel::load(const QString& filename /*= QString()*/)
  {
  	if (!filename.isEmpty())
  	{
  		m_filename = filename;
  	}
  	if (m_filename.isEmpty())
  	{
  		throw AQP::Error(tr("no filename specified"));
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
  		throw AQP::Error(tr("unrecognized file type"));
  	}
  	qint16 formatVersionNumber;
  	in >> formatVersionNumber;
  	if (formatVersionNumber > FormatNumber)
  	{
  		throw AQP::Error(tr("file format version is too new"));
  	}
  	in.setVersion(QDataStream::Qt_4_5);
  	clear();
  
  	quint16 zipcode;
  	QString postOffice;
  	QString county;
  	QString state;
  	QMap<quint16, QList<QStandardItem*> > itemForZipcode;
  
  	while (!in.atEnd())
  	{
  		in >> zipcode >> postOffice >> county >> state;
  		QList<QStandardItem*> items;
  		QStandardItem* item = new QStandardItem;
  		item->setData(zipcode, Qt::EditRole);
  		items << item;
  		foreach(const QString& text, QStringList() << postOffice << county << state)
  		{
  			items << new QStandardItem(text);
  		}
  		itemForZipcode[zipcode] = items;
  	}
  
  	QMapIterator<quint16, QList<QStandardItem*> > i(itemForZipcode);
  	while(i.hasNext())
  	{
  		appendRow(i.next().value());
  	}
  }
  ~~~

  ​	**保存数据：**

  ~~~c++
  void StandardTableModel::save(const QString& filename /*= QString()*/)
  {
  	if (!filename.isEmpty())
  	{
  		m_filename = filename;
  	}
  	if (m_filename.isEmpty())
  	{
  		throw AQP::Error(tr("no filename specified"));
  	}
  	QFile file(m_filename);
  	if (!file.open(QIODevice::WriteOnly))
  	{
  		throw AQP::Error(file.errorString());
  	}
  
  	QDataStream out(&file);
  	out << MagicNumber << FormatNumber;
  	out.setVersion(QDataStream::Qt_4_5);
  	for (int row = 0; row < rowCount(); ++row)
  	{
  		out << static_cast<quint16>(item(row, Zipcode)->data(Qt::EditRole).toUInt())
  			<< item(row, PostOffice)->text()
  			<< item(row, County)->text()
  			<< item(row, State)->text();
  	}
  }
  ~~~

  ​	这里使用一个唯一的幻数来标志邮编文件格式，并使用一个数字来标志我们使用的文件版本号。使用文件格式标志，使得在之后如想变更文件格式的时候，会变得容易些。这是因为我们可以根据版本号载入文件并按版本做相应调整。在写完这些信息后，设置`QDataStream`的版本为`Qt_4_5`，最后写入邮编记录。由于数据是由数字和字符串组成的，我们可以很容易的在较老的Qt版本中使用这个文件。使用最新的`QDataStream`版本号的唯一优点是新版本可以比老版本支持更多的Qt数据类型，或者在读、写时更快，或者存储的数据更紧凑。

  ​	基础数据类型如整型等无论`QDataStream`对象所使用的Qt版本是什么，都拥有相同的数据格式，所以不设置版本号，读取这些类型也总是安全的。然而，Qt特有的类型以及浮点数据类型的数据格式可能在不同的`QDataStream`版本中是不同的，因此，我们必须总是确保所读写的Qt特有类型以及浮点数据类型应使用相同的`QDataStream`版本。需要注意，永远不要使用`qreal`类型来进行浮点数据类型的读写，因为`qreal`的长度是平台相关的，必须明确地使用`float`、`double`类型。

  ​	写整型数据时使用正确的数据类型是必要的，Qt提供了所有的类型，从`qint8`和`quint8`到`qint64`和`quint64`。

  ​	保存和加载`QStandardItem`对象数据的另一种方法就是使用流操作，因为它支持将`QDataStream`对象作为Qt的全局操作符`operator>>()`和`operator<<()`的第一个参数。可以通过创建`QStandardItem`的派生类并重新实现流操作符要调用的`QStandardItem::read()`和`QStandardItem::write()`方法来获得更细致的读、写控制。

  #### `QSortFilterProxyModel`派生类

  ​	要进行过滤操作，需要实现保护方法`filterAcceptRow()`来过滤行，或者实现`filterAcceptColumn()`来过滤列。对于排序操作，要实现保护方法`lessThan()`。

  ~~~c++
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
  ~~~

  ~~~c++
  bool ProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
  {
  	if (m_minimumZipcode != InvalidZipcode || m_maximumZipcode != InvalidZipcode)
  	{
  		QModelIndex index = sourceModel()->index(sourceRow, Zipcode, sourceParent);
  		if (m_minimumZipcode != InvalidZipcode && 
  			sourceModel()->data(index).toInt() < m_minimumZipcode)
  		{
  			return false;
  		}
  		if (m_maximumZipcode != InvalidZipcode && 
  			sourceModel()->data(index).toInt() > m_maximumZipcode)
  		{
  			return false;
  		}
  	}
  	if (!m_county.isEmpty())
  	{
  		QModelIndex index = sourceModel()->index(sourceRow, County, sourceParent);
  		if (m_county != sourceModel()->data(index).toString())
  		{
  			return false;
  		}
  	}
  	if (!m_state.isEmpty())
  	{
  		QModelIndex index = sourceModel()->index(sourceRow, State, sourceParent);
  		if (m_state != sourceModel()->data(index).toString())
  		{
  			return false;
  		}
  	}
  	return true;
  }
  ~~~

  ​	子类化并不是使用`QSortFilterProxyModel`的唯一方法。可以直接实例化`QSortFilterProxyModel`类，并使用`setFilterKeyColumn()`方法来选择过滤的列，使用`setFilterRegExp()`方法来设置一个正则表达式来进行过滤，指定列的内容与正则表达式不匹配的行，将被过滤出去。也可以使用普通字符串和通配符模式。

  ​	对于排序可以采用多种方法。当视图接收到排序请求时，幕后的`QAbstractItemModel::sort()`方法将被调用，因此一种排序方法就是重新实现`QAbstractItemModel::sort()`，因为基类的实现没有做任何事情。对于本例，视图使用`QSortFilterProxyModel::sort()`方法进行排序，它有一个默认的能进行整型及一些基本的Qt类型的排序，比如`QString`和`QDateTime`。

  ​	还可以通过调用`setSortCaseSensitivity()`和`setSortLocaleAware()`方法对代理模型的排序实施更精细的控制；或者可以通过子类化`QSortFilterProxyModel`并重新实现`lessThan()`方法来处理排序。

  ​	对于`QStandardItemModel`派生类来说，另一个排序方法是使用`QStandardItemModel::setSortRole()`方法。例如，设置`Qt::UserRole`对应的数据为排序用的数据。要想这种方式能够正常工作，必须调用`setSortingEnabled(true)`方法来告知视图以支持排序，并且必须确保对于每一项item，除了供显示、编辑角色使用的数据之外，还要有用户角色的数据用来排序。

  ### 1.2 自定义表格模型

  ​	创建一个`QAbstractTableModel`派生类，就像在Qt的模型/视图架构中所有的`QAbstractItemModel`派生类一样，必须实现一组特定的函数，以使得我们的模型派生类的`API`与架构兼容，而且可以应用在任何需要这种模型的地方。

  ​	`QAbstractItemModel`派生类在几种不同情况下必须实现的方法，如下表所示：

  

  | 方法                                    | 说明                                                         |
| --------------------------------------- | :----------------------------------------------------------- |
  | **所有模型**                            |                                                              |
| `data(index,role)`                      | 返回所属模型的指定项(索引为`index`)的角色(`role`, `Qt::ItemDataRole`类型)所对应的的`QVariant`类型数据值 |
  | `flags(index)`                          | 返回表示指定项(索引为`index`)是否可用、是否有复选框、是否可编辑、是否可选择等状态的一个或多个`Qt::ItemFlag`类型值的按位或 |
| `headerData(sect,orient,role)`          | 返回由参数`sect`所指行(当`orient`为`Qt::Vertial`时)或列(当`orient`为`Qt::Horizontal`时)处参数`role`所指角色的`QVariant`类型的表头值 |
  | `rowCount(index)`                       | 返回以参数`index`为父项的那些项的行数                        |
| **表格和树模型**                        |                                                              |
  | `columnCount(index)`                    | 返回以参数`index`为父项的那些项的列数，通常对于整个模型来说是常量 |
| **所有可编辑的模型**                    |                                                              |
  | `setData(index,value,role)`             | 设置指定项(索引为`index`)的指定角色(`role`)对应的数据值为`value`，如果成功则返回`true`并且发出`dataChanged()`信号 |
| `setHeaderData(sect,orient,value,role)` | 设置由参数`sect`所指行(当`orient`为`Qt::Vertial`时)或列(当`orient`为`Qt::Horizontal`时)处参数`role`所指角色对应的表头值为`value`，如果设置成功，则返回true并且发出`headerDataChanged()`信号 |
  | **所有可添加/删除行的模型**             |                                                              |
| `insertRows(row,count,index)`           | 在指定项(索引为`index`)的子项中，在索引为`row`的行之前插入`count`行；重新实现时必须调用`beginInsertRows()`和`endInsertRows()` |
  | `removeRows(row,count,index)`           | 在指定项(索引为`index`)的子项中，从索引为`row`的行开始删除`count`行；如果成功，则返回`true`；重新实现时必须调用`beginRemoveRows()`和`endRemoveRows()` |
| **可添加、删除列的表格和树模型**        |                                                              |
  | `insertColumns(column,count,index)`     | 在指定项(索引为`index`)的子项中，在索引为`column`的列之前插入`count`列；如果成功，则返回`true`；重新实现时必须调用`beginInsertColumns()`和`endInsertColumns()` |
| `removeColumns(column,count,index)`     | 在指定项(索引为`index`)的子项中，从索引为`column`的列开始删除`count`列；如果成功，则返回`true`；重新实现时必须调用`beginRemoveColumns()`和`endRemoveColumns()` |
  | **树模型**                              |                                                              |
  | `index(row,column,index)`               | 返回指定项(索引为`index`)的子项中，行号为`row`、列号为`column`的项的索引(`QModelIndex`类型) |
  | `parent(index)`                         | 返回参数`index`所指项的父项索引(`QModelIndex`类型)           |
  
  ​	*`QList`在通常情况下能提供比`QVector`更好的性能，而对在中间进行插入和删除操作更是如此。对于真正的大列表，如果需要在中间进行插入和删除操作，`QLinkedList`是个更好的选择。*
  
  ​	`Qt::ItemDataRole`枚举类型，如下表所示：
  
  | 枚举值                          | 说明                                                         |
  | ------------------------------- | ------------------------------------------------------------ |
  | `Qt::AccessibleDescriptionRole` | 关于辅助特性支持的描述内容                                   |
  | `Qt::AccessibleTextRole`        | 辅助工具(比如屏幕阅读器)所使用的文本内容                     |
  | `Qt::BackgroundRole`            | 渲染数据时所使用的的背景画刷                                 |
  | `Qt::CheckStateRole`            | 数据项的复选状态                                             |
  | `Qt::DecorationRole`            | 数据的代表图标                                               |
  | `Qt::DisplayRole`               | 将要表达的数据显示成文本                                     |
  | `Qt::EditRole`                  | 宜于编辑格式的数据                                           |
  | `Qt::FontRole`                  | 渲染数据为文本时所使用的字体                                 |
  | `Qt::ForegroundRole`            | 渲染数据时所使用的的前景画刷                                 |
  | `Qt::SizeHintRole`              | 数据的尺寸大小提示                                           |
  | `Qt::StatusTipRole`             | 数据的状态栏提示内容                                         |
  | `Qt::TextAlignmentRole`         | 渲染数据为文本时文本使用的对齐方式                           |
  | `Qt::ToolTipRole`               | 数据的工具提示                                               |
  | `Qt::UserRole`                  | 可存储自定义数据的角色；更多的数据可存储在`Qt::UserRole + 1`等角色中 |
  | `Qt::WhatsThisRole`             | 数据的"这是什么?"文本内容                                    |
  
  `Qt::ItemFlag`枚举类型，如下表所示：
  
  | 枚举值                    | 说明                                                         |
  | ------------------------- | ------------------------------------------------------------ |
  | `Qt::ItemIsDragEnabled`   | 可拖动标志                                                   |
  | `Qt::ItemIsDropEnabled`   | 可放置标志                                                   |
  | `Qt::ItemIsEditable`      | 可编辑标志                                                   |
  | `Qt::ItemIsEnabled`       | 可与用户交互                                                 |
  | `Qt::ItemIsSelectable`    | 可选择标志                                                   |
  | `Qt::ItemIsTristate`      | 具有三种(而非两种)复选状态标志：选中、未选中、未改变         |
  | `Qt::ItemIsUserCheckable` | 具有用户可操作的复选框标志                                   |
  | `Qt::NoItemFlags`         | 无任何标志。如果这是唯一的标志，则该项不可复选、不可选择、不可编辑等 |
  
  #### `flags()`
  
  ~~~c++
  Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
  {
  	Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);
  	if (index.isValid())
  	{
  		theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
  	}
  	return theFlags;
  }
  ~~~
  
  ​	事实上，基类的实现返回了`Qt::ItemIsSelectable | Qt::ItemIsEnabled`，因此这里只需加上`Qt::ItemIsEditable`即可，但我们更倾向于明确的表达出我们的意图。
  
  #### `data()`
  
  ~~~c++
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
  ~~~
  
  ​	`data()`方法是所有关注数据和元数据的项被访问的途径。这个方法与`headerData()`方法不需要调用对应的基类的方法来处理未想到的情况，我们必须为那些没有亲自处理的情况返回一个无效的`QVariant`变量。为没有处理的情况返回任一无效的`QVariant`变量以外的值，将导致程序无法工作。
  
  ​	Qt的模型/视图架构认为`data()`和`headerData()`方法或返回一个可使用的有效的`QVariant`变量，或返回一个无效的`QVariant`变量。在这种情况下，Qt尽可能的提供一个它需要的变量。如果返回一个并不想要的有效的`QVariant`变量，例如，一个空字符串或0，Qt就会使用这个返回值进而出现混乱。
  
  #### `headerData()`
  
  ~~~c++
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
  ~~~
  
  #### rowCount()
  
  ~~~c++
  int TableModel::rowCount(const QModelIndex &index /*= QModelIndex()*/) const
  {
  	return index.isValid() ? 0 : zipcodes.count();
  }
  ~~~
  
  ​	表格和列表模型项有一个无效的模型索引作为它们的父项，因此如果参数`index`是无效的，就返回表格或列表中所有的行数。如果参数`index`是有效的，即要返回指定项的子项行数(只对树模型有意义)，因此在这种情况，对表格或列表模型都必须返回0。
  
  #### `columnCount()`
  
  ~~~c++
  int TableModel::columnCount(const QModelIndex &index /*= QModelIndex()*/) const
  {
  	return index.isValid() ? 0 : MaxColumns;
  }
  ~~~
  
  ​	如果参数`index`是无效的，就返回表格中所有的列数。对于列表模型，应该从`QAbstractListModel`派生，不需要实现这个方法，因为基类提供的就足够了。如果`index`是有效的，则是指返回指定的某项的子项列数，因此这种情况必须返回0。
  
  #### `setData()`
  
  ~~~c++
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
  ~~~
  
  ​	我们可能想要编辑过程有一个关联效果，并且模型/视图架构在一定程度通过发送指定矩形区域的左上和右下索引的`dataChanged()`信号来支持这种效果。
  
  ​	`setHeaderData()`通过返回`false`来阻止用户编辑行和列表头。如果要允许对表头进行编辑则必须发射带有方向和受影响的部分(行或列)的起始点和结束点的`headerDataChanged()`信号来实现，并返回`true`。
  
  #### `insertRows()`
  
  ~~~c++
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
  ~~~
  
  ​	一个可删除行的模型必须实现`insertRows()`和`removeRows()`，可删除列的模型必须实现`insertColumns()`和`removeColumns()`。如果参数`row`为0，那么新行将插入到所有已有行之前，如果`row == rowCount()`，那么新行将被追加到所有行之后。
  
  ​	在结构上，所有的`insertRows()`的重新实现都遵循相同模式：在对模型进行任何变更之前先调用`beginInsertRows()`，然后是执行插入操作的代码，最后在所有对模型的变更都处理完后调用`endInsertRows()`。如果有任何更改，这个方法必须返回`true`。
  
  ​	这里的`beginInsertRows()`和`endInsertRows()`可用于任何表格或列表模型的派生类。树模型要稍微复杂些。
  
  #### `removeRows()`
  
  ~~~c++
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
  ~~~
  
  ​	这个方法和`insertRows()`具有相同的结构，只是它调用的是`beginRemoveRows()`和`endRemoveRows()`。
  
  
  
  模型中数据发生变化时，需要调用`beginResetModel()`和`endResetModel()`。
  
  ### 1.3 `QStandardItemModel`和自定义模型的比较
  
  ​	当我们使用`QStandardItemModel`或一个`QStandardItemModel`的子类来代表数据时，所有的数据项都保存为`QStandardItem`项，或自定义的`QStandardItem`子类。
  
  ​	从概念上说，`QStandardItem`对象的定位介于视图部件加上模型的方式与便利窗体和内置模型部件的方式之间。使用`QStandardItemModel`要比创建自定义模型容易一些，又要比便利窗体部件更灵活，因为我们经常能直接使用`QStandardItemModel`，甚至在需要子类化`QStandardItemModel`的时候，通常仅仅添加少量的方法就可以了。另一个喜欢`QStandardItemModel`的因素是，它使用一个更加熟悉的基于项的`API`而不是自定义模型的基于索引的`API`。
  
  ​	`QStandardItem`提供了丰富的`API`接口，可以使得`QStandardItem`对象非常方便的去直接使用。一个`QStandardItem`对象的最常用函数是提供项的背景色(`background color`)、是否能选择(`checkabliity`)、选中状态(`checked status`)、是否可编辑(`editability`)、字体(`font`)、前景色(`foreground color`)、图标(`icon`)、状态提示(`status tip`)、文本(`text`)、文本对齐方式(`text alignment`)和工具提示(`tooltip`)的属性的获取和设定方法。额外的数据可以保存在`QStandardItem`对象的未使用的角色中，例如`Qt::UserRole`、`Qt::UserRole + 1`诸如此类。我们甚至可以把`QStandardItem`对象从`QDataStream`流中读入或者输出到`QDataStream`流中。
  
  ​	`QStandardItem`对象为所有的便利性和强大功能所付出的代价是内存消耗，或者还有操作速度。一个自定义模型获取根本不需要存储每个单独的项，或者仅需一些像字符串或数字的轻量级的项。
  
  ​	在所有的情况下，使用`QStandardItemModel`和`QStandardItem`，或它们的派生类通常是最容易和最快捷的方法，至少一开始是这样的。使用这些类允许我们快速地完成一个可工作的原型程序。如果过些时候发现内存消耗或操作速度不令人满意，我们就可以考虑创建一个自定义的模型来代替它。
  
  ​	那些数据项是轻量级的、不需要那么多`QStandardItem`类提供的特性的，以及数据项特别多的应用程序是最可能从自定义模型中获得好处的。并且，对于树模型，`QStandardItemModel`的`API`无法提供我们用自定义模型可以实现的那么多功能。
  
  ​	创建自定义列表和表格模型比较简单，因此它们是在大数据集的情况下潜在地获取性能优势的一个方法。自定义树模型需要做相当多的工作，并且要处理正确是很复杂的，但是它对于仅仅是为了获取额外的、只有用自定义模型才能提供的功能而言可能是必要的。例如，经常性的在树中移动项(包括其子项，递归)，当然，这对于我们创建的特别数据集而言是有意义的。
  
  
  
  ## 2、 树模型
  
  ## 3、 委托
  
  ​	所有的Qt标准视图类(`QListView`、`QTableView`、`QColumnView`、`QTreeView`和`QComboBox`)都为要访问的数据提供了一个`QStyledItemDelegate`进行显示和编辑(对于可编辑模型)。
  
  ​	使用自定义委托，能让我们对视图中显示的项的外观进行完全的控制，或者允许对可编辑项提供自己的编辑器，或者二者兼具。广义的将，有三种使用委托的方式：**与数据类型相关的编辑器**、**与数据类型相关的委托**、**与模型相关的委托**。
  
  ​	Qt的内置委托对象使用特定的窗口部件来对特定的数据类型进行编辑。可以把Qt使用的默认窗口部件改为自己选择的其他内置窗口部件或者自定义窗口部件。这种方法很强大，它将影响所有视图中的相关数据类型的可编辑项。但因为这个原因，它也是最不灵活的方法，尤其是与使用自定义委托相比。
  
  ### 3.1 与数据类型相关的编辑器
  
  ​	如果想提供一个仅仅取决于项类型的全局编辑器，可以创建一个`QItemEditorFactory对象，并且注册一个特定的窗口部件来作为该特定类型的编辑器。例如：`
  
  ~~~c++
  QItemEditorFactory* editorFactory = new QItemEditorFactory;
  QItemEditorCreatorBase* numberEditorCreator = new QStandardItemEditorCreator<SpinBox>();
  editorFactory->registerEditor(QVariant::Double, numberEditorCreator);
  editorFactory->registerEditor(QVariant::Int, numberEditorCreator);
  QItemEditorFactory::setDefaultFactory(editorFactory);
  ~~~
  
  ​	这里为所有值为双精度或整型的可编辑项使用自定义的`SpinBox`部件作为编辑器。`SpinBox`的构造函数为：
  
  ~~~c++
  explicit SpinBox(QWidget* parent = 0)
      : QDoubleSpinBox(parent)
  {
  	setRange(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
      setDecimals(3);
      setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  }
  ~~~
  
  *这里不能使用`std::numeric_limits<T>::min()`，因为对于浮点型数据来说，它将返回一个大于0的最小值(小数)，而不是最小的负数。对整型则返回最小的负数。*
  
  ​	遗憾的是，像这样注册的编辑器和`QStandardItem`中的那些非字符串类型的数据不能很好地一起工作，至少不能直接使用。这是因为在`QStandardItem`项中是以`QString`类型来保存数据的，所以当开始编辑时供编辑的数据是`QString`类型的。这就不能触发使用注册的编辑器`SpinBox`。解决方案也很简单：只要确保从`QStandardItemModel`中存储和获取数据时使用的是`Qt::EditRole`，并且使用`QStandardItem`的派生类来保存数据。例如：
  
  ~~~c++
  class StandardItem : public QStandardItem
  {
  public:
  	explicit StandardItem(const double value) : QStandardItem()
  	{
  		setData(value, Qt::EditRole);
  	}
  
  	QStandardItem* clone() const
  	{ return new StandardItem(data(Qt::EditRole).toDouble()); }
  
  	QVariant data(int role = Qt::UserRole + 1) const
  	{
  		if(role == Qt::DisplayRole)
  		{ return QString("%1").arg(QStandardItem::data(Qt::EditRole).toDouble(), 0, 'f', 3); }
  		
  		if(role == Qt::TextAlignmentRole)
  		{ return static_cast<int>(Qt::AlignVCenter | Qt::AlignRight); }
  
  		return QStandardItem::data(role);
  	}
  }
  ~~~
  
  ​	在构造函数中使用`Qt::EditRole`角色来存储双精度数据。还提供了一个`clone()`方法，以确保如果模型复制一个项的话它能正确的创建一个`StandardItem`实例而非普通的`QStandardItem`实例。
  
  ​	`QStandardItem::data()`方法和`QAbstractItemModel::data()`方法不同；具体地说，它是使用了基类版本的传统C++方法，所以对于未处理的情况，它应该总是返回调用基类的返回值，而不是无效的`QVariant`。
  
  ### 3.2 与数据类型相关的委托
  
  ​	一个避免重复和提高代码复用性的方法，是为特定的行或列创建数据类型相关的委托。
  
  ##### 3.2.1 一个只读的列或行委托
  
  创建模型、视图和委托的代码：
  
  ~~~c++
      QFileSystemModel *model = new QFileSystemModel;
      model->setRootPath(QDir::homePath());
      QModelIndex index = model->index(QDir::homePath());
  
      QTreeView *view = new QTreeView;
      view->setItemDelegateForColumn(3, new DateTimeDelegate);
  
      view->setModel(model);
      view->setColumnHidden(1, true);
      view->setColumnHidden(2, true);
      view->scrollTo(index);
      view->expand(index);
      view->setCurrentIndex(index);
  ~~~
  
  因为我们只想改变它要处理的项的外观，而不是行为，所以需要重新实现构造函数和`paint()`方法。
  
  **`paint()`**
  
  ~~~c++
  void DateTimeDelegate::paint(QPainter *painter,
          const QStyleOptionViewItem &option,
          const QModelIndex &index) const
  {
      const QFileSystemModel *model =
              qobject_cast<const QFileSystemModel*>(index.model());
      Q_ASSERT(model);
      const QDateTime &lastModified = model->lastModified(index);
      painter->save();
      painter->setRenderHints(QPainter::Antialiasing|
                              QPainter::TextAntialiasing);
  
      if (option.state & QStyle::State_Selected)
          painter->fillRect(option.rect, option.palette.highlight());
      const qreal diameter = qMin(option.rect.width(),
                                  option.rect.height());
      const QRectF rect = clockRect(option.rect, diameter);
      drawClockFace(painter, rect, lastModified);
      drawClockHand(painter, rect.center(), diameter / 3.5,
                    ((lastModified.time().hour() +
                     (lastModified.time().minute() / 60.0))) * 30);
      drawClockHand(painter, rect.center(), diameter / 2.5,
                    lastModified.time().minute() * 6);
      drawDate(painter, option, diameter, lastModified);
      painter->restore();
  }
  ~~~
  
  **`clockRect()`**
  
  ~~~c++
  QRectF DateTimeDelegate::clockRect(const QRectF &rect,
                                     const qreal &diameter) const
  {
      QRectF rectangle(rect);
      rectangle.setWidth(diameter);
      rectangle.setHeight(diameter);
      return rectangle.adjusted(1.5, 1.5, -1.5, -1.5);
  }
  ~~~
  
  ​	这个方法基于给定的矩形区域参数返回一个矩形，保持相同的x和y坐标，但根据给定的直径缩小为一个正方形，最后再缩小一些以获得一些空白边距。
  
  ​	`QRect::adjusted()`方法返回一个新的矩形区域，这个矩形区域坐标的左上角和右下角按照给定的值进行调整。比如这个例子中，左上角坐标向右、向下移动了1.5个像素，右下角则向左、向上移动了1.5个像素。
  
  **`drawClockFace()`**
  
  ~~~c++
  void DateTimeDelegate::drawClockFace(QPainter *painter,
          const QRectF &rect, const QDateTime &lastModified) const
  {
      const int Light = 120;
      const int Dark = 220;
  
      int shade = lastModified.date() == QDate::currentDate()
                  ? Light : Dark;
      QColor background(shade, shade,
                        lastModified.time().hour() < 12 ? 255 : 175);
      painter->setPen(background);
      painter->setBrush(background);
      painter->drawEllipse(rect);
      shade = shade == Light ? Dark : Light;
      painter->setPen(QColor(shade, shade,
                      lastModified.time().hour() < 12 ? 175 : 255));
  }
  ~~~
  
  
  
  **`drawClockHand()`**
  
  ~~~c++
  void DateTimeDelegate::drawClockHand(QPainter *painter,
          const QPointF &center, const qreal &length,
          const qreal &degrees) const
  {
      const qreal angle = AQP::radiansFromDegrees(
              (qRound(degrees) % 360) - 90);
      const qreal x = length * std::cos(angle);
      const qreal y = length * std::sin(angle);
      painter->drawLine(center, center + QPointF(x, y));
  }
  ~~~
  
  
  
  **`drawDate()`**
  
  ~~~c++
  void DateTimeDelegate::drawDate(QPainter *painter,
          const QStyleOptionViewItem &option, const qreal &diameter,
          const QDateTime &lastModified) const
  {
      painter->setPen(option.state & QStyle::State_Selected
                      ? option.palette.highlightedText().color()
                      : option.palette.windowText().color());
  #ifndef DEBUG
      QString text = lastModified.date().toString(Qt::ISODate);
  #else
      QString text = lastModified.toString(Qt::ISODate);
  #endif
      painter->drawText(option.rect.adjusted(
              qRound(diameter * 1.2), 0, 0, 0), text,
              QTextOption(Qt::AlignVCenter|Qt::AlignLeft));
  }
  ~~~
  
  ​	`QTextOption`类用来存储一段富文本的对齐方式、换行模式、制表符位置以及其他各种格式标记。在绘制文字时最常用的是提供想要的文本对齐方式以及作用于多行文本的换行模式。
  
  ​	这部分工作和`Qt`中的大多数绘制一样，主要涉及到使用合适的颜色以及尺寸和位置的数学运算。
  
  ##### 3.2.2 一个可编辑的列或行委托
  
  ​	一个自定义委托可以用来渲染项或编辑项，或二者兼具。对于渲染项，只需要重新实现`paint()`方法。如果想支持编辑，就必须实现下面列出的那些`QStyledItemDelegate API`，至少实现`createEditor()`、`setEditorData()`和`setModelData()`方法。
  
  ​	基类的`sizeHint()`和`updateEditorGeometry()`实现几乎总是够用了，因此很少需要重新实现它们。通常也不需要重新实现`paint()`方法，特别是如果数据只是简单的纯文本、日期、时间或数字的话。
  
  | 方法                                             | 说明                                                         |
  | ------------------------------------------------ | ------------------------------------------------------------ |
  | `createEditor(parent,styleOption,index)`         | 为指定模型索引`index`所对应的项创建一个合适的编辑用的窗口部件并返回 |
  | `paint(painter,styleOption,index)`               | 绘制给定模型索引`index`所对应的项(对于纯文本、日期、时间和数字类型很少有需要去重新实现) |
  | `setEditorData(editor,index)`                    | 使用给定模型索引`index`所对应的模型项的数据来填充编辑器`editor` |
  | `setModelData(editor,model,index)`               | 从编辑器`editor`中获取数据并设置为给定模型索引`index`所对应的模型项的数据 |
  | `sizeHint(styleOption,index)`                    | 返回委托需要的、显示或编辑给定模型索引`index`所代表的项的尺寸 |
  | `updateEditorGeometry(editor,styleOption,index)` | 为就地编辑(in-place editing)设置编辑器`editor`的尺寸和位置(很少需要重新实现) |
  
  ​	在自定义委托中渲染项的时候，可以采取三种方法：
  
  - 一种是自己绘制每一项。它的缺点是必须自己处理平台间的差异。
  
  - 另一种是使用`Qt`中的`QStyle`类，例如使用`QStyle::drawControl()`、`QStyle::drawComplexControl()`等等，这是一种强大但比较底层的实现方式，需要十分小心并且需要相当多的代码。
  - 第三种是绘制部件，这是一种简单的高层次的方式，留给`Qt`去处理平台相关性，保持代码尽可能整洁。
  
  这里以`RichTextDelegate`类为例。它将渲染和编辑功能都实现了。
  
  **构造函数**
  
  ~~~c++
  RichTextDelegate::RichTextDelegate(QObject *parent)
      : QStyledItemDelegate(parent)
  {
      checkbox = new QCheckBox;
      checkbox->setFixedSize(
              qRound(1.3 * checkbox->sizeHint().height()),
              checkbox->sizeHint().height());
      label = new QLabel;
      label->setTextFormat(Qt::RichText);
      label->setWordWrap(false);
  }
  ~~~
  
  ​	设置复选框占据的宽度比它实际需要的多一点，以提供一些边距，这样它就不会紧靠着标签部件了。
  
  **`paint()`**
  
  ~~~c++
  void RichTextDelegate::paint(QPainter *painter,
          const QStyleOptionViewItem &option,
          const QModelIndex &index) const
  {
      bool selected = option.state & QStyle::State_Selected;
      QPalette palette(option.palette);
      palette.setColor(QPalette::Active, QPalette::Window,
                       selected ? option.palette.highlight().color()
                                : option.palette.base().color());
      palette.setColor(QPalette::Active, QPalette::WindowText,
                       selected
                       ? option.palette.highlightedText().color()
                       : option.palette.text().color());
  
      int yOffset = checkbox->height() < option.rect.height()
              ? (option.rect.height() - checkbox->height()) / 2 : 0;
      QRect checkboxRect(option.rect.x(), option.rect.y() + yOffset,
              checkbox->width(), checkbox->height());
      checkbox->setPalette(palette);
      bool checked = index.model()->data(index, Qt::CheckStateRole)
                                         .toInt() == Qt::Checked;
      checkbox->setChecked(checked);
  
      QRect labelRect(option.rect.x() + checkbox->width(),
              option.rect.y(), option.rect.width() - checkbox->width(),
              option.rect.height());
      label->setPalette(palette);
      label->setFixedSize(qMax(0, labelRect.width()),
                          labelRect.height());
      QString html = index.model()->data(index, Qt::DisplayRole)
                                         .toString();
      label->setText(html);
  
      QString checkboxKey = QString("CHECKBOX:%1.%2").arg(selected)
                                                     .arg(checked);
      paintWidget(painter, checkboxRect, checkboxKey, checkbox);
      QString labelKey = QString("LABEL:%1.%2.%3x%4").arg(selected)
              .arg(html).arg(labelRect.width()).arg(labelRect.height());
      paintWidget(painter, labelRect, labelKey, label);
  }	
  ~~~
  
  ​	首先基于参数`option`的调色板创建一个新的调色板，然后设置新调色板的`QPalette::Window`背景色和`QPalette::Text`前景色为从`option`参数中获得颜色，并且把该项是否选中计入考量。
  
  ​	进行复选框相关的设置：先创建一个将在后面绘制复选框时用到的`checkboxRect`矩形区域对象。创建了该矩形之后，如果`option`矩形的高度大于复选框所需高度的话，那么设置该矩形在可用空间中垂直居中。接着设置复选框的调色板为先前创建的调色板对象，最后设置复选框的选中状态为对应项的选中状态。
  
  ​	对于标签必须为每一项进行大小设置。为标签创建的`labelRect`矩形区域是基于参数`option`给定的矩形区域的，只是向右边进行了偏移，以空出复选框的空间。宽度的减少可能会导致产生负的宽度，所以我们使用`qMax()`来进行纠正。一旦标签设置了调色板和大小尺寸，就获取对应项的文本并把它设置为标签的文本。
  
  ~~~c++
  void RichTextDelegate::paintWidget(QPainter *painter,
          const QRect &rect, const QString &cacheKey,
          QWidget *widget) const
  {
      QPixmap pixmap(widget->size());
  #if QT_VERSION >= 0x040600
      if (!QPixmapCache::find(cacheKey, &pixmap)) {
  #else
      if (!QPixmapCache::find(cacheKey, pixmap)) {
  #endif
          widget->render(&pixmap);
          QPixmapCache::insert(cacheKey, pixmap);
      }
      painter->drawPixmap(rect, pixmap);
  }
  ~~~
  
  ​	先创建一个指定尺寸的空白图像。`QPixmapCache::find()`方法用来根据指定的标志从缓存中获取一个图像。如果在缓存中找到该标志，则返回`true`，并且填充通过指针传入的`QPixmap`对象；否则返回`false`。第一次时我们把给定的窗口部件渲染到空白图像上，并把它插入到缓存中。最后，在给定的矩形区域中绘制这个图像。另一个获取窗口部件的图像的方式是使用`QPixmap::gradWidget()`方法，把窗口部件作为参数传入。
  
  ​	这种方法最主要的优点是它把几乎所有的绘制和样式方面的工作都交给了`Qt`本身去做，使得我们的代码比起用其他方式要极尽简单，而且得益于图像缓存的使用，我们的代码效率更高。
  
  **`sizeHint()`**
  
  ~~~c++
  QSize RichTextDelegate::sizeHint(const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
  {
      QString html = index.model()->data(index, Qt::DisplayRole)
                                         .toString();
      document.setDefaultFont(option.font);
      document.setHtml(html);
      return QSize(document.idealWidth(), option.fontMetrics.height());
  }
  ~~~
  
  ​	大多数情况下，不需要重新实现`sizeHint()`方法，这里遇到了非常情况，需要自己计算文本的宽度。
  
  ​	最明显的测定宽度的方法是把文本内容转换为纯文本并调用`QFontMetrics::width()`方法计算。但遗憾的是，这种方法没有考虑更精细的细节，例如作为上标或下标的字符，或加粗、倾斜的字母要比普通字母要宽些等等。这里需要的精确计算已经由使用的`QTextDocument::idealWidth()`方法实现了。
  
  ​	在一些平台下，每一次获取尺寸提示调用时都创建和销毁一个`QTextDocument`对象的代价非常昂贵，所以在类的私有成员中，声明了一个易变的`mutable QTextDocument`对象；也就是说，每次都重用同一个`QTextDocument`对象。
  
  ~~~c++
  class RichTextDelegate : public QStyledItemDelegate
  {
      Q_OBJECT
  	......
  private:
      QCheckBox *checkbox;
      QLabel *label;
      mutable QTextDocument document;
  };
  ~~~
  
  
  
  **`createEditor()`**
  
  ~~~c++
  QWidget *RichTextDelegate::createEditor(QWidget *parent,
          const QStyleOptionViewItem &option, const QModelIndex&) const
  {
      RichTextLineEdit *editor = new RichTextLineEdit(parent);
      editor->viewport()->setFixedHeight(option.rect.height());
      connect(editor, SIGNAL(returnPressed()),
              this, SLOT(closeAndCommitEditor()));
      return editor;
  }
  ~~~
  
  ​	这个方法用来为指定的模型索引代表的项创建一个合适的编辑器。这里设置编辑器的可视高度为固定值(参数`option`所指的矩形区域的高度)是必要的，用以防止`RickTextLineEdit`部件在文本输入时，其内容的忽上忽下的变化。
  
  ​	如果用户按下`Return`或`Enter`按键，就把它看作是编辑完成的确认，所以把`RickTextLineEdit`的`returnPressed()`信号连接到一个私有自定义槽`closeAndCommitEditor()`。
  
  ~~~c++
  void RichTextDelegate::closeAndCommitEditor()
  {
      RichTextLineEdit *lineEdit = qobject_cast<RichTextLineEdit*>(
                                                sender());
      Q_ASSERT(lineEdit);
      emit commitData(lineEdit);
      emit closeEditor(lineEdit);
  }
  ~~~
  
  ​	发送两个信号，一个信号通知委托提交编辑器的数据，另一个信号通知委托关闭编辑器，因为不再需要编辑器了。
  
  **`setEditorData()`**
  
  ~~~c++
  void RichTextDelegate::setEditorData(QWidget *editor,
          const QModelIndex &index) const
  {
      QString html = index.model()->data(index, Qt::DisplayRole)
                                         .toString();
      RichTextLineEdit *lineEdit = qobject_cast<RichTextLineEdit*>(
                                                editor);
      Q_ASSERT(lineEdit);
      lineEdit->setHtml(html);
  }
  ~~~
  
  ​	一旦编辑器创建完毕，委托调用`setEditorData()`方法用从模型中获取的数据来初始化它。
  
  **`setModelData()`**
  
  ~~~c++
  void RichTextDelegate::setModelData(QWidget *editor,
          QAbstractItemModel *model, const QModelIndex &index) const
  {
      RichTextLineEdit *lineEdit = qobject_cast<RichTextLineEdit*>(
                                                editor);
      Q_ASSERT(lineEdit);
      model->setData(index, lineEdit->toSimpleHtml());
  }
  ~~~
  
  ​	如果用户确认编辑完毕，通过在编辑器外部点击、通过制表符键切换到其他项、或通过按下`Return`或`Enter`键，将调用上面的这个`setModelData()`方法。通过按下`ESC`键来取消编辑。
  
  ​	这个委托是用于数据类型相关的列或行委托，也就是说，它从来也不需要去检查给定项的行或列来确认要处理的数据类型，而且总是用相同的方式处理每一项。
  
  ### 3.3 与模型相关的委托
  
  ​	如果我们不创建大量的模型，那么在需要时创建自定义模型相关的委托，而不是创建一组更通用的行或列数据相关的委托，会更方便一些。
  
  ​	一种常见的实现委托的情况，是实现`paint()`、`createEditor()`、`setEditorData()`和`setModelData()`方法。自定义委托继承自`QStyledItemDelegate`类。
  
  **`paint()`**
  
  ~~~c++
  void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, 
  	const QModelIndex &index) const
  {
  	if (index.column() == Zipcode)
  	{
  		QStyleOptionViewItemV4 opt(option);
  		initStyleOption(&opt, index);
  		QString text = QString("%1").arg(opt.text.toInt(), 5, 10, QChar('0'));
  		
  		painter->save();
  		painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
  		if (opt.state & QStyle::State_Selected)
  		{
  			painter->fillRect(opt.rect, opt.palette.highlight());
  			painter->setPen(opt.palette.highlightedText().color());
  		}
  		else
  		{
  			painter->setPen(opt.palette.windowText().color());
  		}
  		painter->drawText(opt.rect.adjusted(0, 0, -3, 0), text, 
  			QTextOption(Qt::AlignVCenter | Qt::AlignRight));
  		painter->restore();
  	}
  	else
  	{
  		QStyledItemDelegate::paint(painter, option, index);
  	}
  }
  ~~~
  
  ​	选择自己来绘制邮编`Zipcode`，而把其他数据的绘制留给基类。这些方法中使用了`index.column()`来断定要处理的数据的列。
  
  ​	`QStyleOptionViewItem`类是在`Qt4.0`中引入的，在随后的版本中有增补了`QStyleOptionViewItemV2`、`QStyleOptionViewItemV3`和`QStyleOptionViewItemV4`，每个类都增加新的公有成员。使用`QStyleOptionViewItem`也能工作的很好，但使用后续版本，能够利用那些额外的数据成员带来的便利。
  
  ​	正确获取后续版本的`QStyleOptionViewItem`方法是依照下面的的模式：创建一个`QStyleOptionViewItem4`（或任何一个需要的版本）对象并把参数(`QStyleOptionViewItem`类型)传递给构造函数，然后调用`QStyleOptionViewItem::initStyleOption()`方法，把新样式选项和要处理的项的模型索引作为参数传递进去。
  
  ​	通过获取一个`QStyleOptionViewItemV4`对象，就能访问到它的`text`成员，而不需要使用`index.model()->data(index).toString()`来获取。
  
  **`createEditor()`**
  
  ~~~c++
  QWidget* ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, 
  	const QModelIndex &index) const
  {
  	static QStringList usStates;
  	if (usStates.isEmpty())
  	{
  		usStates << "(Unknown)" << "Alabama" << "Alaska"
  			<< "Arizona" << "Arkansas" << "California"
  			<< "Colorado" << "Connecticut" << "Delaware"
  			<< "District Of Columbia" << "Florida" << "Georgia"
  			<< "Hawaii" << "Idaho" << "Illinois" << "Indiana"
  			<< "Iowa" << "Kansas" << "Kentucky" << "Louisiana"
  			<< "Maine" << "Maryland" << "Massachusetts"
  			<< "Michigan" << "Minnesota" << "Mississippi"
  			<< "Missouri" << "Montana" << "Nebraska" << "Nevada"
  			<< "New Hampshire" << "New Jersey" << "New Mexico"
  			<< "New York" << "North Carolina" << "North Dakota"
  			<< "Ohio" << "Oklahoma" << "Oregon" << "Pennsylvania"
  			<< "Rhode Island" << "South Carolina"
  			<< "South Dakota" << "State" << "Tennessee" << "Texas"
  			<< "Utah" << "Vermont" << "Virginia" << "Washington"
  			<< "West Virginia" << "Wisconsin" << "Wyoming";
  	}
  
  	if (index.column() == Zipcode)
  	{
  		return new ZipcodeSpinBox(parent);
  	}
  	if (index.column() == State)
  	{
  		QComboBox* editor = new QComboBox(parent);
  		editor->addItems(usStates);
  		return editor;
  	}
  
  	return QStyledItemDelegate::createEditor(parent, option, index);
  }
  ~~~
  
  ​	`createEditor()`方法必须判断应该使用哪一个编辑器，然后创建、设置并返回编辑器，从被编辑项的数据中准备好要在编辑器中填充的数据，然后显示给用户。需要注意，如果我们自己创建了一个编辑器，那么有必要传递一个合适的`parent`参数给它，这将确保`Qt`获得该编辑器的所有权，并能在合适的时间删除它。
  
  **`setEditorData()`**
  
  ~~~c++
  void ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
  {
  	if (index.column() == Zipcode)
  	{
  		int value = index.model()->data(index).toInt();
  		ZipcodeSpinBox* spinBox = qobject_cast<ZipcodeSpinBox*>(editor);
  		Q_ASSERT(spinBox);
  		spinBox->setValue(value);
  	}
  	else if (index.column() == State)
  	{
  		QString state = index.model()->data(index).toString();
  		QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
  		Q_ASSERT(comboBox);
  		comboBox->setCurrentIndex(comboBox->findText(state));
  	}
  	else
  	{
  		QStyledItemDelegate::setEditorData(editor, index);
  	}
  }
  ~~~
  
  ​	一旦编辑器创建完毕，`Qt`的模型/视图架构会调用`setEditorData()`方法，在编辑器显示给用户之前会给我们一个填充编辑器数据的机会。我们必须总是处理那些自己创建的窗口部件，而把那些基类负责的窗口部件的工作交由基类去做。
  
  ​	这里用到的逻辑：获取给定模型索引对应的项，把`QWidget`类型的指针转化为一个正确类型的编辑器部件的指针，然后填充编辑器的数据。
  
  **`setModelData()`**
  
  ~~~c++
  void ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, 
  	const QModelIndex &index) const
  {
  	if (index.column() == Zipcode)
  	{
  		ZipcodeSpinBox* spinBox = qobject_cast<ZipcodeSpinBox*>(editor);
  		Q_ASSERT(spinBox);
  		spinBox->interpretText();
  		model->setData(index, spinBox->value());
  	}
  	else if (index.column() == State)
  	{
  		QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
  		Q_ASSERT(comboBox);
  		model->setData(index, comboBox->currentText());
  	}
  	else
  	{
  		QStyledItemDelegate::setModelData(editor, model, index);
  	}
  }
  ~~~
  
  ​	如果用户确认编辑完毕，对于我们负责的那些编辑器部件，必须获取编辑器的值，并把它设置为给定模型索引所代表的的项的值。对于其他窗口部件，把工作交由基类去做。
  
  ​	对于微调框，调用`interpretText()`方法来确认，如果用户通过输入或删除数字改变了值，而不是通过使用微调按钮，微调框中保存的值应该正确反映用户的修改。
  
  ## 4、 视图
  
  ​	如果想以一种不同于任何一种`Qt`内置视图的方式呈现数据项，或者想自定义互相关联的项的外观，这时就需要创建自定义视图了。
  
  ​	大多数时候，`Qt`中的标准模型视图，及`QListView`、`QTableView`、`QColumnView`和`QTreeView`对大多数场合来说足够用了。它们也能被子类化或使用自定义委托，以影响模型项的显示。然而， 有两种需要创建自定义视图的情况：
  
  - 第一种是我们想呈现数据的方式与`Qt`的标准视图呈现数据的方式从根本上不一样；
  
  - 另一种是我们想以某种方式把两项或多项的数据项组合起来进行显示；
  
    广义的讲，有两种创建自定义视图的方式可用：
  
  - 一种方式在我们想要创建一个视图组件时使用，即一个可能在多个不同的模型间进行复用，并且与`Qt`的模型/视图架构相适应的视图。在这种情况下，我们通常子类化`QAbstractItemView`，并且提供标准的视图`API`，这样任何模型都能使用我们的视图。
  - 另一种方式，当我们想以一种独特的方式显示一个特定模型中的数据，并且这种显示方式没什么可能或根本没有可能进行复用时比较有用。在这种情况下，我们只能简单地创建一个刚好有且仅有所需功能的自定义模型视图。这通常涉及子类化`QWidget`并提供自己的`API`，但包含一个`setModel`方法。
  
  ### 4.1`QAbstractItemView`子类
  
  ​	下面列出了实现一个自定义`QAbstractItemView`派生类所需的最小`API`集合：
  
  | 方法(*表示纯虚函数)                      | 说明                                                         |
  | ---------------------------------------- | ------------------------------------------------------------ |
  | `dataChanged(topLeft,bottomRight)`       | 当模型中由`topLeft`到`bottomRight`涵盖的模型索引对应的项发生变化时调用这个槽 |
  | `horizontalOffset()`*                    | 返回视图的水平偏移量                                         |
  | `indexAt(point)`*                        | 返回视口(`viewport`) 中坐标点`point`处的项的模型索引         |
  | `isIndexHidden(index)`*                  | 如果模型索引`index` 所对应的项是隐藏状态，则返回`true`       |
  | `mousePressEvent(event)`                 | 通常用于把鼠标点击的项的模型索引设置为当前模型索引           |
  | `mouseCursor(how,modifiers)`*            | 在按照`how`(向上、下、左、右) 的指示进行移动后返回新项的模型索引，同时把由`modifiers`指明的控制按键计入考量 |
  | `paintEvent(event)`                      | 绘制视图的内容到视口                                         |
  | `resizeEvent(event)`                     | 通常用于更新滚动条                                           |
  | `rowsAboutToBeRemoved(parent,start,end)` | 当以`parent` 为父索引的从`start`到`end`结束的行要被删除的时候调用该方法 |
  | `rowsInserted(parent,start,end)`         | 当从`start` 开始到`end`结束的行要被插入到`parent`父索引之下时调用该方法 |
  | `scrollContentsBy(dx,dy)`                | 在水平方向和垂直方向上分别滚动视图的视口`dx`和`dy`像素       |
  | `scrollTo(index,hint)`*                  | 滚动视图以确保给定模型索引`index`所对应的项是可见的，按照参数`hint`所指的滚动属性来滚动 |
  | `setModel(model)`                        | 使视图使用给定的模型`model`                                  |
  | `setSelection(rect,flags)`               | 应用选中标志`flags`到参数`rect`涵盖或接触到的所有项          |
  | `updateGeometries()`                     | 通常用于更新视图的子窗口部件的位置和尺寸，例如滚动条         |
  | `verticalOffset()`                       | 返回视图的垂直偏移量                                         |
  | `visualRect(index)`*                     | 返回给定模型索引`index`对应的项所占据的矩形区域              |
  | `visualRegionForSelection(selection)`*   | 返回参数`selection` 中包含的那些项的视口区域                 |
  
  ​	`QAbstractItemView`基类为它要显示的内容提供了一个滚动区域。一个`QAbstractItemView`子类窗口部件的唯一可见区域是它的视口(`viewport`)，就是在滚动区域中显示出来的那部分。这个可见区域可以通过`viewport()`方法访问到。这个窗口部件的实际大小无关紧要；重要的是要显示所有的模型数据所需要的窗口部件尺寸，即使它远远超出屏幕大小。
  
  **构造函数**
  
  ~~~c++
  TiledListView::TiledListView(QWidget* parent /*= 0*/)
  	: QAbstractItemView(parent), idealWidth(0), idealHeight(0),
  	hashIsDirty(false)
  {
  	setFocusPolicy(Qt::WheelFocus);
  	setFont(QApplication::font("QListView"));
  	horizontalScrollBar()->setRange(0, 0);
  	verticalScrollBar()->setRange(0, 0);
  }
  ~~~
  
  ​	调用`setFont()`方法来设置窗口部件的字体，而不是像通常在自定义窗口部件中所做的那样使用继承过来的字体。字体有`QApplication::font()`方法返回，如果给定一个类名，就会返回一个该类使用的平台相关的字体。
  
  ​	设置滚动条的范围为(0,0)，这将确保滚动条一直隐藏直到需要时才显示，而把关于隐藏和显示方面的工作留给基类去处理。
  
  **`setModel()`**
  
  ~~~c++
  void TiledListView::setModel(QAbstractItemModel* model)
  {
  	QAbstractItemView::setModel(model);
  	hashIsDirty = true;
  }
  ~~~
  
  **`calculateRectsIfNecessary()`**
  
  ~~~c++
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
  ~~~
  
  ​	首先看看那些矩形区域是否需要重新计算。如果需要重新计算，就先计算出显示一行所需要的高度，以及在视口中可用的最大宽度，即可用的可见宽度。
  
  ​	方法的主循环中，遍历模型中的每一行(即每一项)，然后获取项的文本内容。接着计算该项所需要的宽度以及该项应该显示的`x`坐标和`y`坐标，这取决于该项能否在与前一项所在的同一行中合适地显示出来，或者是必须开始一个新行。一旦知道了该项的尺寸和位置，就由此创建一个矩形区域，并把它以该项的行号为键值加入到`rectForRow`哈希表中。
  
  ​	需要注意的是，在循环中进行计算的过程中，使用的是实际的可见宽度，但是假设了可用高度是在这个宽度下能够显示的所有项所需要的高度。为了获得想要的模型索引，把`QAbstractItemView::rootIndex()`而不是一个无效的模型索引(`QModelIndex()`)作为父索引参数。这两者都在列表模型中工作得一样出色，但在`QAbstractItemView`的派生类中使用更加通用的`rootIndex()`是一种更好的风格。
  
  ​	在末尾重新计算了理想的宽度值(即最宽的项的宽度加上一些边距)和高度值(高度是在视口的当前宽度下能够显示所有项所必需的高度，而不管视口的真实高度是多少)，这时候`y`变量保存的是所有行的总共高度。理想宽度可能比可用宽度要大些，例如，视口宽度比要显示的最宽项的宽度要窄的话。一旦计算完毕，我们就在视口上调用`update()`方法，因为所有的重绘工作都只在视口上进行，而不是在`QAbstractItemView`自定义窗口部件自身上进行，这样数据就会被重绘。
  
  ​	去关注`QAbstractItemView`自定义窗口部件自身的真实尺寸是没有意义的，所有的计算工作都是依照视口以及理想宽度和高度进行的。
  
  **`visualRect()`**
  
  ~~~c++
  QRect TiledListView::visualRect(const QModelIndex &index) const
  {
  	QRect rect;
  	if (index.isValid())
  	{
  		rect = viewportRectForRow(index.row()).toRect();
  	}
  	return rect;
  }
  ~~~
  
  ​	这个纯虚函数必须返回给定模型索引所对应的项占据的矩形区域。
  
  **`viewportRectForRow()`**
  
  ~~~c++
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
  ~~~
  
  ​	它返回某一项的最大精度的`QRectF`对象(例如对于`paintEvent()`方法)；其他调用者则使用`QRectF::toRect()`方法把返回值转化为了普通的基于整型的`QRect`对象。
  
  ​	`rectForRow`哈希表中的矩形区域有每一行的`x`和`y`坐标，这些坐标基于理想宽度(通常是可视宽度)和理想高度(在当前宽度下显示所有项所需要的高度)。这就是说，这些矩形区域有效地使用了基于该窗口部件的理想尺寸的窗口部件坐标系统。`viewportRectForRow()`方法必须返回一个在视口坐标系统中的矩形区域，这样我们才能调整坐标把滚动(`scrolling`)计入考量。
  
  ![1560406901886](.\images\1560406901886.png)
  
  **`isIndexHidden()`**
  
  ~~~c++
  bool isIndexHidden(const QModelIndex &index) const { return false; }
  ~~~
  
  ​	必须重新实现上面这个纯虚方法。这个方法是为那些可能有隐藏项的数据设计的。
  
  **`scrollTo()`**
  
  ~~~c++
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
  ~~~
  
  ​	这也是一个必须实现的纯虚方法。如果指定项占据的矩形区域在视口的左边框的左边，那么视口必须要滚动。滚动是通过改变水平滚动条的值来实现的，即加上指定项占据的矩形区域的左边框与视口的左边框距离之差。其他情况与之类似。
  
  **`indexAt()`**
  
  ~~~c++
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
  ~~~
  
  ​	这个纯虚方法必须返回给定坐标点对应的项的模型索引。这个坐标点是基于视口坐标系统，但是`rectForRow`中的那些矩形区域是基于窗口部件的坐标系统。我们不是把每一个矩形区域坐标进行转换来检查它是否包含这个坐标点，而是只做一次转换，把该坐标点的坐标转换为窗口部件坐标系统中的坐标。
  
  ​	`QPoint::rx()`和`QPoint::ry()`方法返回该坐标点的`x`坐标和`y`坐标的一个非常量引用，这样就易于改变它们。如果没有这样的方法，就必须像`point.setX(horizontalScrollBar()->value()+point.x())`这样做。
  
  **`dataChanged()`**
  
  ~~~c++
  void TiledListView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, 
  	const QVector<int> &roles /*= QVector<int>()*/)
  {
  	hashIsDirty = true;
  	QAbstractItemView::dataChanged(topLeft, bottomRight);
  }
  ~~~
  
  ​	注意这里没有调用`viewport()->update()`来安排重绘。发生变化的数据可能是不可见的，所以重绘可能不是必要的，如果确实需要重绘，`dataChanged()`的基类实现会代我们做重绘调度。
  
  **`rowsInserted()`和`rowsAboutToBeRemoved()`**
  
  ~~~c++
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
  ~~~
  
  ​	如果有新行插入到模型中，或者有一些行将要被删除，那么必须确保视图能够正确响应。这种情况只需简单地把工作交给基类完成就可以了。
  
  **`moveCursor()`**
  
  ~~~c++
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
  ~~~
  
  ​	这个方法必须返回所请求移动动作要移动到的位置处的项的模型索引，如果没有移动动作发生，就返回一个无效的模型索引。如果返回一个无效的`QModelIndex`，`QAbstractItemView`基类什么也不做。
  
  **`horizontalOffset()`和`verticalOffset()`**
  
  ~~~c++
  int TiledListView::horizontalOffset() const
  {
  	return horizontalScrollBar()->value();
  }
  
  int TiledListView::verticalOffset() const
  {
  	return verticalScrollBar()->value();
  }
  ~~~
  
  ​	这个纯虚方法必须实现。它们必须返回视口在窗口部件中的`x`坐标和`y`坐标偏移量。
  
  **`scrollContentsBy()`**
  
  ~~~c++
  void TiledListView::scrollContentsBy(int dx, int dy)
  {
  	scrollDirtyRegion(dx, dy);
  	viewport()->scroll(dx, dy);
  }
  ~~~
  
  ​	当滚动条移动时就调用这个方法。它的职责是确保视口按给定的数值进行滚动，并安排适当地重绘。这里通过在进行滚动之前调用`QAbstractItemView::scrollDirtyRegion()`方法来设置重绘。另一种选择是在执行完滚动后调用`viewport()->update()`方法。
  
  ​	基类的实现仅是调用了`viewport()->update()`，并没有真正执行滚动。如果想在程序中用编码方式进行滚动，就需要通过调用`QScrollBar::setValue()`作用于滚动条来实现。
  
  **`setSelection()`**
  
  ~~~c++
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
  ~~~
  
  ​	这个纯虚方法用于把给定的选择标志应用到给定矩形区域涵盖的或接触到的项中。真正的选择动作必须通过调用`QAbstractItemView::selectionModel()->select()`来完成。
  
  ​	传入的矩形区域是基于视口坐标系统的，所以必须首先创建一个基于窗口部件坐标系统的矩形区域，因为它将被`rectForRow`哈希表所使用。然后以任意顺序遍历哈希表中的所有行，如果任意一项所处的区域与给定的矩形区域有交集，就扩展选集涵盖的起始行和结束行以包含该项(如果该项还没有包含进来的话)。
  
  ****
  
  **`visualRegionForSelection()`**
  
  ~~~c++
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
  ~~~
  
  ​	这个纯虚方法必须实现，并且要返回一个`QRegion`对象，这个对象包含了视图中所有在视口中显示出来的选中项，并使用视口的坐标系统。
  
  **`paintEvent()`**
  
  ~~~c++
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
  ~~~
  
  ​	要注意我们是在窗口部件的视口上进行绘制，而不是在窗口部件本身上绘制。最重要的是，我们不自己绘制这些项，而是交由视图的委托 -- 可能是基类的内置`QStyledItemDelegate`或由类的使用者设置的一个自定义委托来绘制这些项。这是为了确保视图支持自定义委托。
  
  **`resizeEvent()`**
  
  ~~~c++
  void TiledListView::resizeEvent(QResizeEvent *event)
  {
  	hashIsDirty = true;
  	calculateRectsIfNecessary();
  	updateGeometries();
  }
  ~~~
  
  ​	如果视图调整了尺寸大小，那么必须重新计算所有项的矩形区域并更新滚动条。
  
  **`updateGeometries()`**
  
  ~~~c++
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
  ~~~
  
  ​	这个方法用于更新视图的子窗口部件，如滚动条。
  
  **`mousePressEvent()`**
  
  ~~~c++
  void TiledListView::mousePressEvent(QMouseEvent *event)
  {
  	QAbstractItemView::mousePressEvent(event);
  	setCurrentIndex(indexAt(event->pos()));
  }
  ~~~
  
  
  
  ### 4.2 与模型相关的可视化视图