#include "DiagramItem.h"

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneContextMenuEvent>
#include <QtWidgets/QMenu>
#include <QtGui/QPainter>

DiagramItem::DiagramItem(DiagramItemData*& _diagramItemData, QMenu * contextMenu, QGraphicsItem * parent)
{
	diagramItemData = _diagramItemData;
	myContextMenu = contextMenu;
	Utility::Log(diagramItemData->tips);
	Utility::Ins()->getPainterPath(diagramItemData->diagramItemType, myPolygon);
	setPolygon(myPolygon);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	//发生位移，缩放，选择变化
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

	diagramTextItem = new DiagramTextItem(this);
	diagramTextItem->setTextWidth(Utility::Ins()->getDiagramSize(diagramItemData->diagramItemType).width());
	diagramTextItem->setText(diagramItemData->tips);
	diagramTextItem->setCenter();
}

void DiagramItem::removeArrow(Arrow* arrow)
{
	int index = arrows.indexOf(arrow);

	if (index != -1)
		arrows.removeAt(index);
	DiagramScene* scene = dynamic_cast<DiagramScene*>(this->scene());
	int endItemIndex = scene->diagramItemList.indexOf(arrow->endItem());
	if (endItemIndex != -1)
	{
		DiagramItem* endItem = scene->diagramItemList.at(endItemIndex);
		diagramItemData->arrows.removeOne(endItem->diagramItemData->index);
	}
}

void DiagramItem::removeArrows()
{
	foreach(Arrow *arrow, arrows) {
		arrow->startItem()->removeArrow(arrow);
		arrow->endItem()->removeArrow(arrow);
		scene()->removeItem(arrow);
		delete arrow;
	}
}

void DiagramItem::addArrow(Arrow *arrow)
{
	arrows.append(arrow);
}

QPixmap DiagramItem::image() const
{
	QPixmap pixmap(250, 250);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setPen(QPen(Qt::black, 8));
	painter.translate(125, 125);
	painter.drawPolyline(myPolygon);

	return pixmap;
}

void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	scene()->clearSelection();
	setSelected(true);
	myContextMenu->exec(event->screenPos());
}

QVariant DiagramItem::itemChange(GraphicsItemChange change,
	const QVariant &value)
{
	if (change == QGraphicsItem::ItemPositionChange) {
		for (Arrow *arrow : qAsConst(arrows))
			arrow->updatePosition();
	}

	return value;
}

void DiagramItem::refreshTips()
{
	diagramTextItem->setText(diagramItemData->tips);
}

DiagramItem::~DiagramItem()
{
}
