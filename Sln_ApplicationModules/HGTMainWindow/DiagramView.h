#pragma once
#include <QtWidgets/QGraphicsView>
#include <Qt3DInput/QWheelEvent>
#include <Qt3DInput/QKeyEvent>
#include <QtCore/QtMath>

#include "Utility.h"

class DiagramView :public QGraphicsView
{
public:
	DiagramView(QGraphicsScene *scene, QWidget *parent);
	~DiagramView();
	bool isDropping = false;
	QPointF lastMousePoint;
protected:
	virtual void dropEvent(QDropEvent* event) override;
	virtual void wheelEvent(QWheelEvent *mouseEvent) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void keyReleaseEvent(QKeyEvent* event) override;
};

