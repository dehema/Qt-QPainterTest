#include "Utility.h"

const QString Utility::GetPageTitle(PageType _pageType)
{
	switch (_pageType)
	{
	case PageType::Log:
		return QString::fromLocal8Bit("日志");
	case PageType::Consumable:
		return QString::fromLocal8Bit("消耗品");
	case PageType::Sample:
		return QString::fromLocal8Bit("样本");
	case PageType::OperationState:
		return QString::fromLocal8Bit("运行状态");
	case PageType::WaitingQueue:
		return QString::fromLocal8Bit("等待队列");
	case PageType::HistoricalTask:
		return QString::fromLocal8Bit("历史任务");
	case PageType::Result:
		return QString::fromLocal8Bit("结果");
	case PageType::Report:
		return QString::fromLocal8Bit("报告");
	case PageType::Manager:
		return QString::fromLocal8Bit("管理");
	case PageType::Maintenance:
		return QString::fromLocal8Bit("维护");
	default:
		return QString::fromLocal8Bit("");
	}
}

const QString Utility::GetConsumableName(ConsumableType consumableType)
{
	switch (consumableType)
	{
	case ConsumableType::Tips1000:
		return QString::fromLocal8Bit("1000ul枪头");
	case ConsumableType::Tips50:
		return QString::fromLocal8Bit("50ul枪头");
	case ConsumableType::Rubbish:
		return QString::fromLocal8Bit("垃圾仓");
	case ConsumableType::HolePlate:
		return QString::fromLocal8Bit("深孔板");
	default:
		return QString::fromLocal8Bit("");
	}
}

const QString Utility::GetLogTypeStr(LogType logType)
{
	switch (logType)
	{
	case Info:
	default:
		return QString::fromLocal8Bit("Info");
	case Warning:
		return QString::fromLocal8Bit("Warning");
	case Error:
		return QString::fromLocal8Bit("Error");
		break;
	}
}

const QString Utility::GetTestTypeStr(TestType testType)
{
	switch (testType)
	{
	case TestType::Sample:
	default:
		return QString::fromLocal8Bit("样本");
	}
}

const QString Utility::GetDiagnosticTypeStr(DiagnosticType diagnosticType)
{
	switch (diagnosticType)
	{
	case DiagnosticType::Common:
	default:
		return QString::fromLocal8Bit("通用");
	}
}

const QString Utility::GetTaskStateStr(TaskState taskState)
{
	switch (taskState)
	{
	case TaskState::Complete:
	default:
		return QString::fromLocal8Bit("已完成");
	}
}

const QString Utility::GetReleaseStateStr(ReleaseState releaseState)
{
	switch (releaseState)
	{
	case ReleaseState::Release:
		return QString::fromLocal8Bit("已发布");
	case ReleaseState::Unrelease:
	default:
		return QString::fromLocal8Bit("未发布");
	}
}

const QString Utility::GetResultTypeStr(ResultType testType)
{
	switch (testType)
	{
	case ResultType::Positive:
		return QString::fromLocal8Bit("阳性");
	case ResultType::Negative:
	default:
		return QString::fromLocal8Bit("阴性");
	}
}

void Utility::getPainterPath(DiagramItemType myDiagramType, QPolygonF &myPolygon)
{
	QPainterPath path;
	//根据形状类型，赋予点数据
	switch (myDiagramType) {
	case DiagramItemType::StartEnd:
	{
		//moveTo（第一个点位置）
		//arcTo(矩形原点x，矩形原点y，长，宽，start angle，sweep)
		path.moveTo(90, 0);
		path.arcTo(40, -60, 50, 50, 0, 90);//从0度，扫过90度（0-90°）
		path.arcTo(-80, -60, 50, 50, 90, 90);//从90度，扫过90度（90-180°）
		path.arcTo(-80, 0, 50, 50, 180, 90);//从180度，扫过90度（180-270°）
		path.arcTo(40, 0, 50, 50, 270, 90);//从270度，扫过90度（270-360°）
										   //      path.lineTo(90, 0);
		myPolygon = path.toFillPolygon();
		break;
	}
	case DiagramItemType::Conditional:
	{
		myPolygon << QPointF(-100, 0) << QPointF(0, 100)
			<< QPointF(100, 0) << QPointF(0, -100)
			<< QPointF(-100, 0);
		break;
	}
	case DiagramItemType::Step:
	{
		myPolygon << QPointF(-100, -100) << QPointF(100, -100)
			<< QPointF(100, 100) << QPointF(-100, 100)
			<< QPointF(-100, -100);
		break;
	}
	case DiagramItemType::IO:
	default:
	{
		myPolygon << QPointF(-120, -80) << QPointF(-70, 80)
			<< QPointF(120, 80) << QPointF(70, -80)
			<< QPointF(-120, -80);
		break;
	}
	}
}

const QSize Utility::getDiagramSize(DiagramItemType myDiagramType)
{
	return QSize(200, 200);
}
