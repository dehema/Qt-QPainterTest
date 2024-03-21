#pragma once
#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QDrag>
#include <QtCore/QDebug>
#include <QtCore/QMimeData>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QDir>

#include "Utility.h"
#include "DiagramMenuItem.h"
#include "DiagramItem.h"
#include "DiagramView.h"
#include "DiagramScene.h"
#include "DiagramItemData.h"


class MainWindow : public QDialog
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void setSelItem(DiagramItem* selectItem);
private:
	const int topHeight = 100;
	const int leftWidth = 260;
	QWidget* widgetMain;
	QGridLayout* layoutMain;

	//顶部菜单
	QWidget* widgetTop;
	QHBoxLayout* layoutTop;
	QPushButton* btSave;
	QPushButton* btLoad;
	QPushButton* btRemoveAllItem;

	//左侧
	QWidget* widgetLeft;
	QVBoxLayout* layoutLeft;

	//左侧菜单
	QWidget* widgetMenu;
	QGridLayout* layoutMenu;
	QList<DiagramMenuItem*> widgetDiagramMenuCubeList = {};

	//-----------------------------------------左侧属性-----------------------------------------
	QWidget* widgetAttribute;
	QGridLayout* layoutAttribute;

	//提示
	QLabel* lbTips = nullptr;
	QLineEdit* editTips = nullptr;

	//方法
	QLabel* lbFunc = nullptr;
	QComboBox* cbFunc = nullptr;

	//参数1
	QLabel* lbParam1 = nullptr;
	QLineEdit* editParam1 = nullptr;

	//指向
	QLabel* lbTargetStep = nullptr;
	QComboBox* cbTargetStep = nullptr;
	//-----------------------------------------左侧属性end-----------------------------------------

	//画布
	QWidget* widgetPaint;
	DiagramScene* scene;
	DiagramView *view;

	//浮动临时拖拽
	QWidget* widgetTempDropItem;
	QLabel* lbTempDropItem;
	//-----------------------------------------field-----------------------------------------
	QPainter* painter;
	QPoint mouseOffset;
	//拖拽对象
	DiagramItemData dropDiagramItemData;
	bool isDropping = false;
	QMenu *itemMenu;
	QMenuBar *menuBar;

	QAction *actionDeleteItem;
	QAction *actionAddLine;

	DiagramItem* selectItem;
	DiagramItemData* selectData;
	//属性初始行数
	int attrRowInitCount;
	//-----------------------------------------function-----------------------------------------
	void createMenus();
    void createDiagramMenuItem(DiagramItemType, QString);
	void refreshAttrUI(DiagramItemType _diagramItemType);
	QString getSavePath();

	//-----------------------------------------流程函数-----------------------------------------
    enum logicfuncType {
		none,
		logProjectName,
		logNumber,
        length,
	};
	void logic_func_logProjectName() { Utility::Log("QPaintTest"); };
	void logic_func_logNumber(int num) { Utility::Log(num); };
	QString logic_getLogicFuncName(logicfuncType type) {
		switch (type)
		{
		case MainWindow::none:
			return QString::fromLocal8Bit("none");
		case MainWindow::logProjectName:
			return QString::fromLocal8Bit("log项目名");
		case MainWindow::logNumber:
			return QString::fromLocal8Bit("log数字");
		}
		return QString("");
	};
	void logic_refreshTargetStep() {
		cbTargetStep->clear();
		for (int index : selectData->arrows)
		{
			DiagramItem* item = scene->getItemByIndex(index);
			cbTargetStep->addItem(item->diagramItemData->tips);
		}
	};
protected:
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	//virtual void wheelEvent(QWheelEvent *mouseEvent) override;
	private slots :
	void paintEvent(QPaintEvent *);
	void slots_menuItemClick(QPoint _mouseOffset, DiagramItemData* _diagramItemData);
	void slots_menuItemDrag();
	void slots_deleteItem();
	void slots_save();
	void slots_load();
	void slots_removeAllItem();
	void slots_diagramTipsEdit(QString str);
	void slots_diagramFuncChange(int index);
	void slots_diagramParamsEdit(QString str);
	void slots_diagramTargetStep(int index);
signals:
	void signals_createDiagramItem(QPoint _mousePos, DiagramItemData _diagramItemData);
};
#endif // CENTRIFUGE_MANAGE_H
