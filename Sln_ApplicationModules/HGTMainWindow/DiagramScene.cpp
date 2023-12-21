#include "DiagramScene.h"
#include "arrow.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QTextCursor>
#include "MainWindow.h"

DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent) : QGraphicsScene(parent)
{
	myItemMenu = itemMenu;
	myItemType = DiagramItemType::Step;
	line = nullptr;
	myItemColor = Qt::white;
	myTextColor = Qt::black;
	myLineColor = Qt::black;
	setSceneRect(QRectF(0, 0, 5000, 5000));
}

void DiagramScene::setLineColor(const QColor &color)
{
	myLineColor = color;
	//isItemChange是我们自定义的函数，判断当前选择的items中，有没有指定的type类型的item
	if (isItemChange(Arrow::Type)) {
		Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
		item->setColor(myLineColor);
		update();
	}
}

//void DiagramScene::setTextColor(const QColor &color)
//{
//	myTextColor = color;
//	if (isItemChange(DiagramTextItem::Type)) {
//		DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
//		item->setDefaultTextColor(myTextColor);
//	}
//}

void DiagramScene::setItemColor(const QColor &color)
{
	myItemColor = color;
	if (isItemChange(DiagramItem::Type)) {
		DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
		item->setBrush(myItemColor);
	}
}

DiagramItem* DiagramScene::createDiagramItem(DiagramItemData*& data)
{
	DiagramItem *item = new DiagramItem(data, myItemMenu);
	diagramItemList.append(item);
	item->setBrush(myItemColor);
	addItem(item);
	qreal scaleFactor = diagramView->matrix().m11();
	QSize size = Utility::Ins()->getDiagramSize(data->diagramItemType);
	QPointF newItemPos = diagramView->mapToScene(data->pos) - QPointF(160, 0) / scaleFactor - QPointF(size.width() / 2, size.height() / 2) / scaleFactor;
	item->setPos(newItemPos);
	emit itemInserted(item);
	return item;
}

void DiagramScene::removeDiagramItem(DiagramItem * item)
{
	item->removeArrows();
	diagramItemList.removeOne(item);
	delete item;
}

void DiagramScene::createNewLine()
{
	QGraphicsItem* graphicsItem = nullptr;
	if (selectedItems().length() != 1)
	{
		return;
	}
	graphicsItem = selectedItems()[0];
	line = new QGraphicsLineItem(QLineF(graphicsItem->scenePos(), graphicsItem->scenePos()));
	line->setPen(QPen(myLineColor, 2));
	addItem(line);
}

int DiagramScene::getNewItemIndex()
{
	int newIndex = 0;
	if (diagramItemList.length() > 0)
	{
		newIndex = diagramItemList.last()->diagramItemData->index + 1;
	}
	return newIndex;
}

void DiagramScene::removeAllDiagramItem()
{
	foreach(DiagramItem* item, diagramItemList)
	{
		item->removeArrows();
	}
	for (DiagramItem* item : diagramItemList)
	{
		removeDiagramItem(item);
	}
	diagramItemList.clear();
}

void DiagramScene::linkLine(DiagramItem* startItem, DiagramItem* endItem)
{
	Arrow *arrow = new Arrow(startItem, endItem);
	arrow->setColor(myLineColor);
	startItem->addArrow(arrow);
	startItem->diagramItemData->arrows.append(endItem->diagramItemData->index);
	arrow->setZValue(-1000.0);
	addItem(arrow);
	//下面这条语句好像有点多余
	arrow->updatePosition();
}

DiagramItem* DiagramScene::getItemByIndex(int index)
{
	for (size_t i = 0; i < diagramItemList.count(); i++)
	{
		if (diagramItemList[i]->diagramItemData->index == index)
		{
			return diagramItemList[i];
		}
	}
	return nullptr;
}

//void DiagramScene::setFont(const QFont &font)
//{
//	myFont = font;
//
//	if (isItemChange(DiagramTextItem::Type)) {
//		QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
//		//此时，选择可以更改，因此第一个选择的项可能不是DiagramTextItem
//		if (item)
//			item->setFont(myFont);
//	}
//}

void DiagramScene::setItemType(DiagramItemType type)
{
	myItemType = type;
}


//void DiagramScene::editorLostFocus(DiagramTextItem *item)
//{
//	QTextCursor cursor = item->textCursor();
//	cursor.clearSelection();
//	item->setTextCursor(cursor);
//
//	if (item->toPlainText().isEmpty()) {
//		removeItem(item);
//		item->deleteLater();
//	}
//}

//在scene中点击鼠标，通过模式选择需要创建的图元
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (mouseEvent->button() == Qt::RightButton)
	{
		line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
			mouseEvent->scenePos()));
		line->setPen(QPen(myLineColor, 2));
		addItem(line);
	}
	if (diagramView->dragMode() != QGraphicsView::DragMode::NoDrag)
	{
		return;
	}

	if (mouseEvent->button() != Qt::LeftButton)
		return;
	QGraphicsScene::mousePressEvent(mouseEvent);

	if (selectedItems().count() > 0)
	{
		DiagramItem* selectItem = qgraphicsitem_cast<DiagramItem*>(selectedItems().at(0));
		if (selectItem != nullptr)
		{
			MainWindow* main = dynamic_cast<MainWindow*>(UIMgr::ins().getUI(ViewName::MainWindow));
			main->setSelItem(selectItem);
		}
	}
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (line != nullptr) {
		QLineF newLine(line->line().p1(), mouseEvent->scenePos());
		line->setLine(newLine);
	}
	else {
		QGraphicsScene::mouseMoveEvent(mouseEvent);
	}
}
//只对箭头起作用
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (line != nullptr) {
		QList<QGraphicsItem *> startItems = items(line->line().p1());
		//startItems里包含line本身，需要去掉。
		//这里if判断看着有点多余，但在极端情况下有用。比如划线时间特别短，只画了一个点就release。
		if (startItems.count() && startItems.first() == line)
			startItems.removeFirst();
		QList<QGraphicsItem *> endItems = items(line->line().p2());
		if (endItems.count() && endItems.first() == line)
			endItems.removeFirst();

		removeItem(line);
		delete line;

		QGraphicsItem* _startItem = nullptr;
		QGraphicsItem* _endItem = nullptr;
		for (size_t i = 0; i < startItems.length(); i++)
		{
			if (startItems[i]->type() == DiagramItem::Type)
			{
				_startItem = startItems[i];
				break;
			}
		}
		for (size_t i = 0; i < endItems.length(); i++)
		{
			if (endItems[i]->type() == DiagramItem::Type)
			{
				_endItem = endItems[i];
				break;
			}
		}
		//如果startItem和endItem都存在，切都是图形。则可以开始画箭头线了。
		if (_startItem != nullptr&&_endItem != nullptr&&_startItem != _endItem) {
			DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(_startItem);
			DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(_endItem);
			linkLine(startItem, endItem);
		}
	}
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	line = nullptr;
}

bool DiagramScene::isItemChange(int type) const
{
	const QList<QGraphicsItem *> items = selectedItems();
	//cb是一个lambda函数
	const auto cb = [type](const QGraphicsItem *item) { return item->type() == type; };
	//吧items中的item，一个个放到cb函数中进行对比
	return std::find_if(items.begin(), items.end(), cb) != items.end();
}