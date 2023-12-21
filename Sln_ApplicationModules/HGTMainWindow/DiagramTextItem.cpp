#include "DiagramTextItem.h"

DiagramTextItem::DiagramTextItem(QGraphicsItem *parent) : QGraphicsTextItem(parent)
{
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);

}

DiagramTextItem::~DiagramTextItem()
{
}

void DiagramTextItem::setCenter()
{
	moveBy(-boundingRect().width() / 2, -boundingRect().height() / 2);
}

void DiagramTextItem::setText(QString str)
{
	setPlainText(str);
	QTextBlockFormat format;
	format.setAlignment(Qt::AlignCenter);
	QTextCursor cursor = textCursor();
	cursor.select(QTextCursor::Document);
	cursor.mergeBlockFormat(format);
	cursor.clearSelection();
	setTextCursor(cursor);
}

void DiagramTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (textInteractionFlags() == Qt::NoTextInteraction)
		setTextInteractionFlags(Qt::TextEditorInteraction);
	QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void DiagramTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
}