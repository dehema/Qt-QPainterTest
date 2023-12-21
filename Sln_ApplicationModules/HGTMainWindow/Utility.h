#pragma once
#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QFile>

#define qClamp(val, min, max) qMin(qMax(val, min), max)

enum class DiagramItemType
{
	//过程 正方
	Step,
	//条件 棱形
	Conditional,
	//起止 椭圆
	StartEnd,
	//数据/输入输出 斜四边形
	IO
};

enum class ConsumableType
{
	//1000ul枪头
	Tips1000,
	//50ul枪头
	Tips50,
	//垃圾仓
	Rubbish,
	//深孔板
	HolePlate
};

enum class PageType {
	//日志
	Log,
	//消耗品
	Consumable,
	//样本
	Sample,
	//运行状态
	OperationState,
	//等待队列
	WaitingQueue,
	//历史任务
	HistoricalTask,
	//结果
	Result,
	//报告
	Report,
	//管理
	Manager,
	//维护
	Maintenance,
	Length,
};

//日志类型
enum LogType
{
	Info,
	Warning,
	Error,
};

//测试类型
enum class TestType
{
	Sample
};

//诊断类型
enum class DiagnosticType
{
	Common
};

//任务状态
enum class TaskState
{
	Complete,
};

//发布状态
enum class ReleaseState
{
	Release,
	Unrelease
};

//结果类型
enum class ResultType
{
	Positive,
	Negative
};

class Utility
{
public:
	static Utility* Ins() {
		static Utility* instance = nullptr;
		if (instance == nullptr)
		{
			instance = new Utility;
			qDebug() << "new Utility";
		}
		return instance;
	}

	//屏幕宽度
	static const int ScreenWidth = 1920;
	//屏幕高度
	static const int ScreenHeight = 1080;
	//内容页宽度
	static const int ContentPageWidth = ScreenWidth - 50;
	//内容页高度
	static const int ContentPageHeight = 600;
	//弹窗窗口宽度
	static const int DialogDefaultWidth = 800;

	static void Log(const char *str)
	{
		qDebug() << QString::fromLocal8Bit(str);
	}

	static void Log(QString str)
	{
		qDebug() << str;
	}

	static void Log(int str)
	{
		qDebug() << QString::number(str);
	}

	static void Log(QByteArray byte)
	{
		qDebug() << byte;
	}
	//分页标题
	const QString GetPageTitle(PageType);
	//耗材名称
	const QString GetConsumableName(ConsumableType);

	//读取qss
	static QString LoadStyleFile()
	{
		//QString path = QCoreApplication::applicationDirPath();
		//path = path.append(_path);
		QString path = "../QPainterTest/Sln_ApplicationModules/qss/qss_mainView.qss";
		QFile qss(path);
		if (!qss.exists())
		{
			Log("读取qss失败");
			return QString();
		}
		qss.open(QFile::ReadOnly);
		QString str = qss.readAll();
		qss.close();
		return str;
	}

	//日志类型文本
	const QString GetLogTypeStr(LogType logType);
	//测试类型文本
	const QString GetTestTypeStr(TestType testType);
	//诊断类型文本
	const QString GetDiagnosticTypeStr(DiagnosticType diagnosticType);
	//任务状态文本
	const QString GetTaskStateStr(TaskState taskState);
	//发布状态文本
	const QString GetReleaseStateStr(ReleaseState releaseState);
	//结果类型文本
	const QString GetResultTypeStr(ResultType testType);
	//多边形
	void getPainterPath(DiagramItemType myDiagramType, QPolygonF& polygonF);
	//多边形尺寸
	const QSize getDiagramSize(DiagramItemType myDiagramType);
};

//QSize Utility::ScreenSize = new QSize(1080, 1920);
