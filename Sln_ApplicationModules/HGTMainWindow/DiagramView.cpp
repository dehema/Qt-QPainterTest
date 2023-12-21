#include "DiagramView.h"


DiagramView::DiagramView(QGraphicsScene *scene, QWidget *parent = Q_NULLPTR) :QGraphicsView(scene, parent)
{
	setMouseTracking(true);
	//this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	blockSignals(true);
	//setEnabled(false);
	//this->verticalScrollBar()->blockSignals(true);
	//this->horizontalScrollBar()->blockSignals(true);
	//this->horizontalScrollBar()->setEnabled(false);
	//this->verticalScrollBar()->setEnabled(false);
	//verticalScrollBar()->setValue(this->verticalScrollBar()->minimum());
	//horizontalScrollBar()->setValue(this->horizontalScrollBar()->minimum());
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	//setDragMode(QGraphicsView::RubberBandDrag);
	setRubberBandSelectionMode(Qt::ContainsItemBoundingRect);
	setAcceptDrops(true);
}

DiagramView::~DiagramView()
{
}

void DiagramView::dropEvent(QDropEvent * event)
{
	QGraphicsView::dropEvent(event);
}

void DiagramView::wheelEvent(QWheelEvent * mouseEvent)
{
	if (dragMode() != DragMode::NoDrag)
	{
		return;
	}
	if ((mouseEvent->modifiers() == Qt::ControlModifier))
	{
		//ctrl缩放模式
		int wheelValue = mouseEvent->angleDelta().y();
		qreal scaleFactor = matrix().m11();
		double ratio = (double)wheelValue / (double)1200 + 1;
		if ((scaleFactor < 0.5f&& ratio < 1) || (scaleFactor > 1.5f&& ratio > 1))
		{
			return;
		}
		scale(ratio, ratio);
	}
	else
		QGraphicsView::wheelEvent(mouseEvent);
}

void DiagramView::mousePressEvent(QMouseEvent * event)
{
	if (dragMode() == DragMode::ScrollHandDrag)
	{
		isDropping = true;
		lastMousePoint = event->pos();
	}
	QGraphicsView::mousePressEvent(event);
}

void DiagramView::mouseReleaseEvent(QMouseEvent * event)
{
	isDropping = false;
	QGraphicsView::mouseReleaseEvent(event);
}

void DiagramView::mouseMoveEvent(QMouseEvent * event)
{
	if (isDropping)
	{
		//QPointF disPointF = event->pos() - lastMousePoint;
		//lastMousePoint = event->pos();
		//double posx = qClamp(scene()->sceneRect().x() + disPointF.x(), (double)0, (double)scene()->sceneRect().width() - width());
		//scene()->setSceneRect(
		//	posx,
		//	qClamp(scene()->sceneRect().y() + disPointF.y(), (double)0, (double)scene()->sceneRect().height()),
		//	scene()->sceneRect().width(),
		//	scene()->sceneRect().height());
	}
	QGraphicsView::mouseMoveEvent(event);
}

void DiagramView::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Space)
	{
		setDragMode(DragMode::ScrollHandDrag);
	}
	QGraphicsView::keyPressEvent(event);
}

void DiagramView::keyReleaseEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Space)
	{
		setDragMode(DragMode::NoDrag);
	}
	QGraphicsView::keyReleaseEvent(event);
}
