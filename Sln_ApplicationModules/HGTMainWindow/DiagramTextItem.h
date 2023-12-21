#pragma once
#include "QtWidgets/QGraphicsTextItem"
#include "QtGUI/QTextBlockFormat"
#include "QtGUI/QTextCursor"

#include "Utility.h"

class DiagramTextItem : public QGraphicsTextItem
{
public:
	DiagramTextItem(QGraphicsItem *parent = nullptr);
	~DiagramTextItem();
	void setCenter();
	void setText(QString str);

protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

