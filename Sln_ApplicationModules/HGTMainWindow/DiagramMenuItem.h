#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <Qt3DInput/QMouseEvent>

#include "DiagramItemData.h"
#include "Utility.h"

class DiagramMenuItem : public QWidget
{
	Q_OBJECT
public:
	DiagramMenuItem(DiagramItemData* _data, QWidget *parent = nullptr);
	~DiagramMenuItem();

	//field
	static const int itemWidth = 100;
	static const int itemHeight = 100;
	static const QString itemStyle()
	{
		return  QString::fromLocal8Bit("background-color: rgb(222, 222, 222);border: 1px solid rgb(121, 121, 121); ");
	}
	DiagramItemData* data;
	//function
private:
	QWidget* mainWindow;
	QPoint itemPos;
	QString text;
	//UI
	QLabel* img;
	QLabel* lbText;


protected:
	//function
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void dropEvent(QDropEvent* event) override;
signals:
	void signals_menuItemClick(QPoint _mouseOffset, DiagramItemData* _diagramItemData);
	void signals_menuItemDrag();
};


