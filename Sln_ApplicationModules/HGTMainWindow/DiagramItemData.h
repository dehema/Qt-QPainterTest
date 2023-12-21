#pragma once

#include <QtCore/QString>
#include <QtCore/QPoint>

#include "Utility.h"

struct DiagramItemData
{
public:
	DiagramItemData() {};
	DiagramItemData(DiagramItemType _diagramItemType, QString _text) {
		diagramItemType = _diagramItemType;
		tips = _text;
	}

	//节点ID
	int index;
	//节点类型
	DiagramItemType diagramItemType;
	//节点提示文本
	QString tips;
	//节点在视图中的坐标
	QPoint pos;
	//所有指向的节点ID
	QList<int> arrows;
	//节点方法类型
	int funcType;
	//节点参数
	QString params1;
	//指向节点
	int targetStep;
};