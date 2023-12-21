#pragma once
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtCore/QVector>

#include "Arrow.h"
#include "DiagramTextItem.h"
#include "DiagramScene.h"
#include "DiagramItemData.h"
#include "Utility.h"

class DiagramItem : public QGraphicsPolygonItem
{
public:
	enum { Type = UserType + 15 };
	DiagramItem(DiagramItemData*& diagramItemData, QMenu *contextMenu, QGraphicsItem *parent = nullptr);
	~DiagramItem();
	DiagramItemType diagramType() const { return myDiagramType; }
	QPolygonF polygon() const { return myPolygon; }
	QPixmap image() const;
	int type() const override { return Type; }
	DiagramItemData* diagramItemData;
	//Item与其他ITem的连接关系
	QList<Arrow *> arrows;

	//function
	void refreshTips();
	void addArrow(Arrow *arrow);
	void removeArrow(Arrow *arrow);
	void removeArrows();

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
	//对于一个Item来说，我们需要知道它的形状，各个点的值，右键菜单，连接情况
	//图形类型，只是我们前面自己定义的枚举量
	DiagramItemType myDiagramType;
	//点的数据集合QVector<QPointF>
	QPolygonF myPolygon;
	//右键菜单
	QMenu *myContextMenu;
	DiagramTextItem* diagramTextItem;
};