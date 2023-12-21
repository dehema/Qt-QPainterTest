#include "DiagramMenuItem.h"

DiagramMenuItem::DiagramMenuItem(DiagramItemData* _data, QWidget *parent) :QWidget(parent)
{
	data = _data;
	//mainWindow = parent;
	this->setFixedSize(itemWidth, itemHeight);
	this->setStyleSheet(itemStyle());
	lbText = new QLabel(this);
	lbText->setObjectName("common_label");
	lbText->setFixedSize(itemWidth, itemHeight);
	lbText->setAlignment(Qt::AlignCenter);
	lbText->setText(data->tips);
	itemPos = parent->pos() + pos();

}

DiagramMenuItem::~DiagramMenuItem()
{
}

void DiagramMenuItem::paintEvent(QPaintEvent * ev)
{

}

void DiagramMenuItem::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit signals_menuItemDrag();
		signals_menuItemClick(event->pos() + itemPos, data);
	}
}

void DiagramMenuItem::mouseMoveEvent(QMouseEvent * event)
{

	if (event->buttons() & Qt::LeftButton)
	{
		int distance = (event->pos() - itemPos).manhattanLength();
		if (distance >= QApplication::startDragDistance())
		{
			emit signals_menuItemDrag();
		}
	}
}

void DiagramMenuItem::dropEvent(QDropEvent* event)
{
}