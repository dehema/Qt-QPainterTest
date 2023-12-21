#pragma once

#include <QtWidgets/QGraphicsScene>
#include <Qt3DInput/QWheelEvent>
#include <QtCore/QList>

#include "diagramitem.h"
#include "diagramitemData.h"
#include "DiagramView.h"
#include "Utility.h"
#include "UIMgr.h"

class DiagramScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit DiagramScene(QMenu *itemMenu, QObject *parent = nullptr);
	QFont font() const { return myFont; }
	QColor textColor() const { return myTextColor; }
	QColor itemColor() const { return myItemColor; }
	QColor lineColor() const { return myLineColor; }
	void setLineColor(const QColor &color);
	void setTextColor(const QColor &color);
	void setItemColor(const QColor &color);
	void setFont(const QFont &font);
	DiagramItem* createDiagramItem(DiagramItemData*& data);
	void removeDiagramItem(DiagramItem* item);
	void createNewLine();
	int getNewItemIndex();
	void removeAllDiagramItem();
	void linkLine(DiagramItem* startItem, DiagramItem* endItem);
	DiagramItem* getItemByIndex(int index);

	//------------------------field------------------------
	QList<DiagramItem*> diagramItemList = QList<DiagramItem*>();
	DiagramView *diagramView;
	QGraphicsLineItem *line;
	public slots:
	void setItemType(DiagramItemType type);
	//void editorLostFocus(DiagramTextItem *item);

signals:
	void itemInserted(DiagramItem *item);
	void textInserted(QGraphicsTextItem *item);
	void itemSelected(QGraphicsItem *item);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
	bool isItemChange(int type) const;

	DiagramItemType myItemType;
	QMenu *myItemMenu;
	bool leftButtonDown;
	QPointF startPoint;
	QFont myFont;
	//DiagramTextItem *textItem;
	QColor myTextColor;
	QColor myItemColor;
	QColor myLineColor;
};