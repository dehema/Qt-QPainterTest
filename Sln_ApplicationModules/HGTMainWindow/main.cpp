#include <QtCore/QtPlugin>
#include <QtWidgets/QApplication>
#include <QtCore/QTextCodec>
#include <QtCore/QMetaType>
#include "MainWindow.h"
#include "UIMgr.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//QTextCodec *codec = QTextCodec::codecForName("GB2312");
	//QTextCodec::setCodecForLocale(codec);
	//QTextCodec::setCodecForCStrings(codec);
	//QTextCodec::setCodecForTr(codec);
	//QTextCodec::setCodecForTr(QTextCodec::codecForLocale

	UIMgr::ins().init();

	MainWindow mainWindow;
	UIMgr::ins().mainWindow = &mainWindow;
	mainWindow.show();
	//mainWindow.show();
	return a.exec();
}
