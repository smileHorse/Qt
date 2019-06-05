# QT的模型/视图架构

​	QT提供了几个窗口部件用于显示存储在模型中的那些数据项。

​	有几种纯粹的**视图部件**：`QListView`、`QTableView`、`QColumnView`(能够以列表的横向序列方式来显示树型层次结构的视图)和`QTreeView`。所有这些视图都必须提供一个模型(自定义的，或是QT提供的)来与之配合。

​	QT提供了一些便利**窗口部件**(它们提供了自己的内置模型并能直接使用)，如`QListWidget`、`QTableWidget`和`QTreeWidget`。还有`QComboBox`，它既是一个便利窗口部件也是一个视图部件，我们能直接使用它(因为它提供了内置模型)，也能把它当成一个模型的视图部件(这种情况下，可以提供一个合适的模型给它)。

​	所有的标准视图都提供了一个默认的`QStyledItemDelegate`委托 -- 该类显示视图的各个项，并为可编辑的项提供一个合适的编辑器。我们也能创建自己的委托，来控制视图的各个项的显示和编辑。

​	关于模型、视图、委托以及其背后的数据之间的关系，如下图所示：

​	![1559699099552](E:\code\GitHub\Qt\文档\images\1559699099552.png)

​	*在一些环境中，特别是对于那些数据集比较小的应用程序，假如它们使用的数据在任何时候仅仅显示在一个部件中，那么选择便利窗口部件比较合适。*

​	尽管有统一的API，仍然有两种不同类型的模型：有行列特性的表格模型，有父子关系的树模型。

​	除了考虑表格模型和树模型之外，还有三类模型可能用到：

- QT预定义的一些模型，如`QStringListModel`、`QDirModel`、还有`QDirModel`的后继者`QFileSystemModel`。这些模型可以直接使用并且只需做很少的工作。

- `QStandardItemModel`,它是一个可被当做列表模型、表格模型或树模型来使用的通用模型。它提供了一个基于项的API。对于那些能够直接把数据恰好地填充入列表、表格或树模型的各个数据项中，并能够直接使用或仅需做微小改动即可的情况，使用`QStandardItemModel`是理想的选择。

- 最后一种是从`QAbstractItemModel`或`QAbstractListModel` 、`QAbstractTableModel`派生而来的自定义模型。如果想达到最佳的性能，或者该模型不符合基于项的特征，那就需要使用自定义模型。

  一些QT模型的层次结构如下图所示：

  ![1559699874831](E:\code\GitHub\Qt\文档\images\1559699874831.png)

  使用`QStandardItemModel`的话，就不需要再创建自定义的模型了。但它有两个潜在的缺点：

- 首先，在加载大数据集时它明显比自定义模型慢。

- 其次，对于树模型来说，它提供的API不像自定义模型那样能有那么多的功能。

  大部分情况下，可以先使用`QStandardItemModel`，等到必要的时候再实现一个自定义模型来代替它。

  ## 1、 表格模型

  ​	表格模型按照行和列进行工作，每一项的父项都是一个无效的`QModelIndex`对象。列表模型和表格模型没有本质的不同：列表模型仅仅是只有一列的表格模型而已。

  ### 1.1 用于表格的`QStandardItemModel`

  	#### 使用模型的QComboBox

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

  ​	加载数据：

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

  ​	保存数据：

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

  ## 4、 视图

  