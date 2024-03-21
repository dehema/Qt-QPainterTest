#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QDialog(parent)
{
	setWindowFlag(Qt::WindowType::FramelessWindowHint);
	createMenus();

	this->setStyleSheet(Utility::LoadStyleFile());
	this->setFixedSize(QSize(Utility::ScreenWidth, Utility::ScreenHeight));
	widgetMain = new QWidget(this);
	widgetMain->setObjectName("widgetMain");
	widgetMain->setFixedSize(QSize(Utility::ScreenWidth, Utility::ScreenHeight));
	widgetMain->move(0, 0);
	layoutMain = new QGridLayout(widgetMain);
	layoutMain->setSpacing(0);
	layoutMain->setContentsMargins(0, 0, 0, 0);
	layoutMain->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	widgetMain->setLayout(layoutMain);

	widgetTop = new QWidget();
	widgetTop->setObjectName("paint_top");
	widgetTop->setFixedSize(Utility::ScreenWidth, topHeight);
	layoutMain->addWidget(widgetTop, 0, 0, 1, 2);

	layoutTop = new QHBoxLayout();
	widgetTop->setLayout(layoutTop);

	btSave = new QPushButton();
	btSave->setObjectName("common_button");
	btSave->setText(QString::fromLocal8Bit("保存"));
	btSave->setFixedSize(160, 35);
	connect(btSave, SIGNAL(clicked()), this, SLOT(slots_save()));
	layoutTop->addWidget(btSave);

	btLoad = new QPushButton();
	btLoad->setObjectName("common_button");
	btLoad->setText(QString::fromLocal8Bit("读取"));
	btLoad->setFixedSize(160, 35);
	connect(btLoad, SIGNAL(clicked()), this, SLOT(slots_load()));
	layoutTop->addWidget(btLoad);

	btRemoveAllItem = new QPushButton();
	btRemoveAllItem->setObjectName("common_button");
	btRemoveAllItem->setText(QString::fromLocal8Bit("清除"));
	btRemoveAllItem->setFixedSize(160, 35);
	connect(btRemoveAllItem, SIGNAL(clicked()), this, SLOT(slots_removeAllItem()));
	layoutTop->addWidget(btRemoveAllItem);

	//左侧
	widgetLeft = new QWidget();
	widgetLeft->setObjectName("paintTest_left");
	widgetLeft->setFixedSize(leftWidth, Utility::ScreenHeight - topHeight);
	layoutMain->addWidget(widgetLeft, 1, 0);

	layoutLeft = new QVBoxLayout();
	widgetLeft->setLayout(layoutLeft);

	//左侧菜单
	widgetMenu = new QWidget();
	layoutLeft->addWidget(widgetMenu);
	layoutMenu = new QGridLayout();
	layoutMenu->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	layoutMenu->setMargin(20);
	layoutMenu->setSpacing(20);
	widgetMenu->setLayout(layoutMenu);

	createDiagramMenuItem(DiagramItemType::Step, QString::fromLocal8Bit("过程"));
	createDiagramMenuItem(DiagramItemType::Conditional, QString::fromLocal8Bit("条件"));
	createDiagramMenuItem(DiagramItemType::StartEnd, QString::fromLocal8Bit("起止"));
	createDiagramMenuItem(DiagramItemType::IO, QString::fromLocal8Bit("数据"));

	//左侧属性
	QWidget* widgetAttribute = new QWidget();
	layoutLeft->addWidget(widgetAttribute);

	layoutAttribute = new QGridLayout();
	layoutAttribute->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	layoutAttribute->setMargin(20);
	layoutAttribute->setSpacing(20);
	widgetAttribute->setLayout(layoutAttribute);

	//画布
	widgetPaint = new QWidget();
	widgetPaint->setFixedSize(Utility::ScreenWidth - leftWidth, Utility::ScreenHeight - topHeight);
	widgetPaint->setStyleSheet(DiagramMenuItem::itemStyle());
	layoutMain->addWidget(widgetPaint, 1, 1);

	scene = new DiagramScene(itemMenu, this);
	scene->setParent(widgetPaint);

	view = new DiagramView(scene, widgetPaint);
	view->setFixedSize(Utility::ScreenWidth - leftWidth, Utility::ScreenHeight - topHeight);
	scene->diagramView = view;

	//浮动临时拖拽
	widgetTempDropItem = new QWidget(this);
	widgetTempDropItem->setWindowFlag(Qt::WindowStaysOnTopHint, true);
	widgetTempDropItem->setStyleSheet(DiagramMenuItem::itemStyle());
	widgetTempDropItem->setFixedSize(DiagramMenuItem::itemWidth, DiagramMenuItem::itemHeight);
	widgetTempDropItem->hide();

	lbTempDropItem = new QLabel(widgetTempDropItem);
	lbTempDropItem->setObjectName("common_label");
	lbTempDropItem->move(0, 0);
	lbTempDropItem->setAlignment(Qt::AlignCenter);
	lbTempDropItem->setFixedSize(DiagramMenuItem::itemWidth, DiagramMenuItem::itemHeight);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setSelItem(DiagramItem* _selectItem)
{
	selectItem = _selectItem;
	selectData = selectItem->diagramItemData;
	refreshAttrUI(selectData->diagramItemType);
}

void MainWindow::createMenus()
{
	menuBar = new QMenuBar();
	itemMenu = menuBar->addMenu(tr("&Item"));
	actionDeleteItem = new QAction(QString::fromLocal8Bit("删除"));
	itemMenu->addAction(actionDeleteItem);
	itemMenu->releaseShortcut(Qt::Key::Key_Delete);
	itemMenu->setStatusTip(tr("删除"));
	connect(actionDeleteItem, SIGNAL(triggered()), this, SLOT(slots_deleteItem()));
	itemMenu->addSeparator();
	actionAddLine = new QAction(QString::fromLocal8Bit("连线"));
	itemMenu->addAction(actionAddLine);
	connect(actionAddLine, &QAction::triggered, this, [=]() {scene->createNewLine(); });
}

void MainWindow::createDiagramMenuItem(DiagramItemType _itemType, QString _text)
{
	DiagramItemData* data = new DiagramItemData(_itemType, _text);
	DiagramMenuItem* diagramMenuItem = new DiagramMenuItem(data, widgetLeft);
	widgetDiagramMenuCubeList.append(diagramMenuItem);
	layoutMenu->addWidget(diagramMenuItem, layoutMenu->count() / 2, layoutMenu->count() % 2);
	connect(diagramMenuItem, SIGNAL(signals_menuItemClick(QPoint, DiagramItemData*)), this, SLOT(slots_menuItemClick(QPoint, DiagramItemData*)));
	connect(diagramMenuItem, SIGNAL(signals_menuItemDrag()), this, SLOT(slots_menuItemDrag()));
}

void MainWindow::refreshAttrUI(DiagramItemType _diagramItemType)
{
	if (lbFunc != nullptr)
		lbFunc->hide();
	if (cbFunc != nullptr)
		cbFunc->hide();
	if (lbParam1 != nullptr)
		lbParam1->hide();
	if (editParam1 != nullptr)
		editParam1->hide();
	if (lbTargetStep != nullptr)
		lbTargetStep->hide();
	if (cbTargetStep != nullptr)
		cbTargetStep->hide();

	QLabel* lbCommon = new QLabel();
	lbCommon->setObjectName("common_diagramAttrTips");
	lbCommon->setText(QString::fromLocal8Bit("通用"));
	lbCommon->setAlignment(Qt::AlignCenter);
	layoutAttribute->addWidget(lbCommon, 0, 0, 1, 2);

	//提示
	if (lbTips == nullptr)
	{
		lbTips = new QLabel();
		lbTips->setObjectName("common_diagramAttrTips");
		lbTips->setText(QString::fromLocal8Bit("提示"));
		layoutAttribute->addWidget(lbTips, 1, 0);
	}

	if (editTips == nullptr)
	{
		editTips = new QLineEdit();
		editTips->setMaxLength(20);
		layoutAttribute->addWidget(editTips, 1, 1);
		connect(editTips, SIGNAL(textChanged(QString)), this, SLOT(slots_diagramTipsEdit(QString)));
	}
	editTips->setText(selectData->tips);


	if (attrRowInitCount == 0)
	{
		attrRowInitCount = layoutAttribute->rowCount();
	}
	if (_diagramItemType == DiagramItemType::Step)
	{
		//方法
		if (lbFunc == nullptr)
		{
			lbFunc = new QLabel();
			lbFunc->setMinimumWidth(50);
			lbFunc->setText(QString::fromLocal8Bit("方法"));
			lbFunc->setObjectName("common_diagramAttrTips");
		}
		layoutAttribute->addWidget(lbFunc, attrRowInitCount, 0);
		lbFunc->show();

		if (cbFunc == nullptr)
		{
			cbFunc = new QComboBox();
			for (int i = 0; i < logicfuncType::length; i++)
			{
				cbFunc->addItem(logic_getLogicFuncName(static_cast<logicfuncType>(i)));
			}
			connect(cbFunc, SIGNAL(currentIndexChanged(int)), this, SLOT(slots_diagramFuncChange(int)));
		}
		layoutAttribute->addWidget(cbFunc, attrRowInitCount, 1);
		cbFunc->show();

		//参数1
		if (lbParam1 == nullptr)
		{
			lbParam1 = new QLabel();
			lbParam1->setText(QString::fromLocal8Bit("参数1"));
			lbParam1->setObjectName("common_diagramAttrTips");
		}
		layoutAttribute->addWidget(lbParam1, attrRowInitCount + 1, 0);
		lbParam1->show();

		if (editParam1 == nullptr)
		{
			editParam1 = new QLineEdit();
			connect(editParam1, SIGNAL(textChanged(QString)), this, SLOT(slots_diagramParamsEdit(QString)));
		}
		layoutAttribute->addWidget(editParam1, attrRowInitCount + 1, 1);
		editParam1->show();
	}
	else if (_diagramItemType == DiagramItemType::Conditional)
	{
		//参数1
		if (lbTargetStep == nullptr)
		{
			lbTargetStep = new QLabel();
			lbTargetStep->setText(QString::fromLocal8Bit("指向"));
			lbTargetStep->setObjectName("common_diagramAttrTips");
		}
		layoutAttribute->addWidget(lbTargetStep, attrRowInitCount, 0);
		lbTargetStep->show();

		if (cbTargetStep == nullptr)
		{
			cbTargetStep = new QComboBox();
			connect(cbFunc, SIGNAL(currentIndexChanged(int)), this, SLOT(slots_diagramTargetStep(int)));
		}
		layoutAttribute->addWidget(cbTargetStep, attrRowInitCount, 1);
		cbTargetStep->show();
		logic_refreshTargetStep();
	}
}

QString MainWindow::getSavePath()
{
	QString path = QCoreApplication::applicationDirPath() + QString::fromLocal8Bit("/存档");
	QDir dir(path);
	if (!dir.exists()) {
		dir.mkdir(path);//新建文件夹
	}
	path += QString::fromLocal8Bit("/abc.json");
	return path;
}

void MainWindow::mouseReleaseEvent(QMouseEvent * event)
{
	if (isDropping)
	{
		bool iscontains = widgetPaint->geometry().contains(QCursor::pos());
		if (iscontains)
		{
			dropDiagramItemData.pos = event->pos();
			dropDiagramItemData.index = scene->getNewItemIndex();

			DiagramItemData* data = new DiagramItemData();
			data->pos = dropDiagramItemData.pos;
			data->index = dropDiagramItemData.index;
			data->diagramItemType = dropDiagramItemData.diagramItemType;
			Utility::Log(dropDiagramItemData.tips);
			data->tips = dropDiagramItemData.tips;
			scene->createDiagramItem(data);
		}
	}
	widgetTempDropItem->hide();
	isDropping = false;
	QWidget::mouseReleaseEvent(event);
}

void MainWindow::slots_menuItemClick(QPoint _mouseOffset, DiagramItemData* _diagramItemData)
{
	mouseOffset = _mouseOffset;
	lbTempDropItem->setText(_diagramItemData->tips);
	dropDiagramItemData.diagramItemType = _diagramItemData->diagramItemType;
	dropDiagramItemData.tips = _diagramItemData->tips;
	isDropping = true;
}

void MainWindow::slots_menuItemDrag()
{
	if (!isDropping)
		return;
	QPoint offset = QCursor::pos() - mouseOffset;
	widgetTempDropItem->move(offset);
	widgetTempDropItem->show();
}

void MainWindow::slots_save()
{
	QJsonDocument doc = QJsonDocument();
	QJsonArray array = QJsonArray();
	for (int i = 0; i < scene->diagramItemList.length(); i++)
	{
		DiagramItemData* data = scene->diagramItemList[i]->diagramItemData;
		QJsonObject obj = QJsonObject();
		obj.insert("index", data->index);
		obj.insert("posX", data->pos.x());
		obj.insert("posY", data->pos.y());
		obj.insert("text", data->tips);
		obj.insert("type", (int)data->diagramItemType);
		QJsonArray arrowArray = QJsonArray();
		for (int i = 0; i < data->arrows.length(); i++)
		{
			arrowArray.append(data->arrows[i]);
		}
		obj.insert("arrows", arrowArray);
		array.append(obj);
	}
	doc.setArray(array);
	QByteArray byte = doc.toJson();
	QString path = getSavePath();
	QFile file(path);
	if (file.open(QIODevice::WriteOnly)) {
		file.write(byte);
		file.close();
	}
}

void MainWindow::slots_load()
{
	slots_removeAllItem();

	QFile file(getSavePath());
	if (!file.exists())
	{
		Utility::Log("找不到文件");
	}
	QByteArray jsonData;
	if (file.open(QIODevice::ReadOnly)) {
		jsonData = file.readAll();
		file.close();
	}
	Utility::Log("读取数据:");
	Utility::Log(jsonData);
	QJsonDocument doc = QJsonDocument::fromJson(jsonData);
	if (doc.isArray()) {
		QJsonArray array = doc.array();
		for (int i = 0; i < array.count(); i++)
		{
			DiagramItemData* data = new DiagramItemData();
			QJsonObject obj = array.at(i).toObject();
			int posX;
			int posY;
			data->index = obj.value("index").toInt();
			posX = obj.value("posX").toInt(0);
			posY = obj.value("posY").toInt(0);
			data->tips = obj.value("text").toString();
			QJsonArray arrows = obj.value("arrows").toArray();

			for (int j = 0; j < arrows.count(); j++)
			{
				data->arrows.append(arrows[j].toInt());
			}
			data->diagramItemType = (DiagramItemType)obj.value("type").toInt();
			data->pos = QPoint(posX, posY);
			DiagramItem* startItem = scene->createDiagramItem(data);
		}
	}
	//连线
	for (int i = 0; i < scene->diagramItemList.count(); i++)
	{
		DiagramItem* startItem = scene->diagramItemList[i];
		DiagramItemData* data = startItem->diagramItemData;
		for (int endItemIndex : data->arrows)
		{
			DiagramItem* endItem = scene->getItemByIndex(endItemIndex);
			if (endItem != nullptr && endItem != nullptr)
			{
				scene->linkLine(startItem, endItem);
			}
		}
	}
}

void MainWindow::slots_removeAllItem()
{
	scene->removeAllDiagramItem();
}

void MainWindow::slots_diagramTipsEdit(QString str)
{
	selectData->tips = str;
	selectItem->refreshTips();
}

void MainWindow::slots_diagramFuncChange(int index)
{
	selectData->funcType = index;
}

void MainWindow::slots_diagramParamsEdit(QString str)
{
	selectData->params1 = str;
}

void MainWindow::slots_diagramTargetStep(int index)
{
	selectData->targetStep = index;
}

void MainWindow::slots_deleteItem()
{
	foreach(QGraphicsItem *item, scene->selectedItems())
	{
		DiagramItem* diagramItem = qgraphicsitem_cast<DiagramItem*>(item);
		scene->removeDiagramItem(diagramItem);
	}
}

//void MainWindow::wheelEvent(QWheelEvent * mouseEvent)
//{
//	if (true)
//	{
//
//	}
//	int wheelValue = mouseEvent->angleDelta().y();
//	double ratio = (double)wheelValue / (double)1200 + 1;
//	view->scale(ratio, ratio);
//}

void MainWindow::paintEvent(QPaintEvent* paintEvent)
{
	painter = new QPainter(this);
	painter->drawLine(QPoint(0, 0), QPoint(100, 100));
}
