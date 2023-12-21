#pragma once
#include "Single.h"
//#include "MainWindow.h"
#include "Utility.h"



enum class ViewName
{
	MainWindow,
};

class UIMgr :public Single<UIMgr>
{
public:
	UIMgr();
	~UIMgr();

	QWidget* mainWindow;
	void init()
	{
		//MainWindow mainWindow;
		//mainWindow.show();
	}

	QWidget* getUI(ViewName viewName)
	{
		switch (viewName)
		{
		case ViewName::MainWindow:
			return mainWindow;
		}
	};

	//void getUI(T _view) {
	//	Utility::Log(_view);
	//};
};