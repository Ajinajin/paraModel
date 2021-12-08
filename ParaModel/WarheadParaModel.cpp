#include <QTextCodec>

#include <fstream>
#include <iomanip>

#include "WarheadParaModel.h"
#include <qtreeview.h>
#include <qtreewidget.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsView>
#include <qtreewidget.h>
#include <QTimer>
#include <QDateTime>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QFormLayout>

#include "bqgraphicsitem.h"
#include "bqgraphicsscene.h"
#include "WarheadDataConvert.h"

#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include "SARibbonPannel.h"
#include "SARibbonToolButton.h"
#include "SARibbonMenu.h"
#include "SARibbonComboBox.h"
#include "SARibbonLineEdit.h"
#include "SARibbonGallery.h"
#include "SARibbonCheckBox.h"
#include "SARibbonQuickAccessBar.h"
#include "SARibbonButtonGroupWidget.h"
#include "SARibbonApplicationButton.h"
#include "SARibbonCustomizeWidget.h"
#include "SARibbonCustomizeDialog.h"
#include "SAFramelessHelper.h"

#define FREEPTR(p) if(p!=NULL)\
	{ \
		delete p;\
		p = NULL ;\
	 }\


#pragma region 初始化窗口
//初始化建筑楼层窗口
void WarheadParaModel::InitWarheadWidget(QDockWidget* from)
{
	LoadModeTreeProperty = from;
	LoadModeTreeProperty->setWindowTitle("战斗部属性");
	LoadModeTreeProperty->setFixedWidth(300);
}

//初始化系统模型窗口
void WarheadParaModel::InitSysUnitWidget(QDockWidget* from)
{
	from->setFixedWidth(300);
	from->setWindowTitle("系统战斗部模型");

	QWidget* mytreewidget = new QWidget();
	QTreeWidget* pWarheadModelTreeWidget = new QTreeWidget(mytreewidget);
	pWarheadModelTreeWidget->setHeaderHidden(true);
	for (vector<ArmHeadTopo>::const_iterator iter = vWarhead.begin(); iter != vWarhead.end(); iter++)
	{
		QTreeWidgetItem* rootItem = new QTreeWidgetItem(pWarheadModelTreeWidget);
		rootItem->setText(0, iter->sArmHeadName);
		rootItem->setData(0, Qt::UserRole, iter->nArmHeadIdx);
	}
	//树按钮响应
	connect(pWarheadModelTreeWidget, &QTreeWidget::itemDoubleClicked, this, [=](QTreeWidgetItem* item, int column)
		{
			QVariant variant = item->data(0, Qt::UserRole);
			int nArmHeadIdx = variant.value<int>();
			vLoadWarhead = vWarhead[nArmHeadIdx];
			ReLoadModelTree();
			ReLoadModelProperty();
			AddSceneData();
			if (if_data == 0)
			{
				MyLogOutput("当前无画布信息，请新建或者打开后在操作");
				return;
			}
		});
	from->setWidget(pWarheadModelTreeWidget);
	pWarheadModelTreeWidget->expandAll();
}

//初始化已加载的模型窗口
void WarheadParaModel::InitLoadModelWidget(QDockWidget* from)
{
	LoadModeTreeWidget = from;
	LoadModeTreeWidget->setWindowTitle("模型结构树");
	LoadModeTreeWidget->setFixedWidth(300);
	//layout布局 
}

//初始化日志窗口
void WarheadParaModel::InitLogWidget(QDockWidget* from)
{
	from->setMaximumHeight(100);
	from->setFixedHeight(100);
	from->setWindowTitle("信息输出");
	if (myLogOutLabel == nullptr)
		myLogOutLabel = new QTextEdit();
	from->setWidget(myLogOutLabel);
}

//初始化编辑视图
void WarheadParaModel::InitEditManagerWidget(QDockWidget* from)
{
	MainDockWidget = from;

	graphicsViewMain = new BQGraphicsView();
	graphicsViewMain->setScene(&pSceneMain);
	pSceneMain.setBackgroundBrush(Qt::darkGray);
	MainDockWidget->setMinimumWidth(1310);
	MainDockWidget->setWindowTitle("编辑视图 （二维X）");
	MainDockWidget->setWidget(graphicsViewMain);
	//中间大屏三位窗口
	MainDockWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
}

//初始化三维显示窗口
void WarheadParaModel::InitOpenGlWidget(QDockWidget* from)
{
	paraOglmanagerMain = new ParaOGLManager();
	paraOglmanagerMain->setMinimumHeight(100);
	from->setWindowTitle("三维视图");
	from->setWidget(paraOglmanagerMain);
	//中间大屏三位窗口
	from->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &WarheadParaModel::updateOGL);
	//每20ms刷新一次OpenGL界面 50FPS
	timer->start(20);
}

//更新OpenGL窗口
void WarheadParaModel::updateOGL()
{
	paraOglmanagerMain->update();

}

//初始化内容区域
void WarheadParaModel::InitCentralWidget()
{
	//删除中央窗体
	QWidget* p = takeCentralWidget();
	if (p)
		delete p;

	setDockNestingEnabled(true);

	QDockWidget* sysWidget = new QDockWidget(this);
	InitSysUnitWidget(sysWidget);


	QDockWidget* editManagerWidget = new QDockWidget(this);
	InitEditManagerWidget(editManagerWidget);

	QDockWidget* loadModelTreeWidget = new QDockWidget(this);
	InitLoadModelWidget(loadModelTreeWidget);


	QDockWidget* openglWidget = new QDockWidget(this);
	InitOpenGlWidget(openglWidget);


	QDockWidget* ModelTreeWidget = new QDockWidget(this);
	InitWarheadWidget(ModelTreeWidget);

	QDockWidget* logWidget = new QDockWidget(this);
	InitLogWidget(logWidget);

	addDockWidget(Qt::LeftDockWidgetArea, sysWidget);
	splitDockWidget(sysWidget, editManagerWidget, Qt::Horizontal);
	splitDockWidget(editManagerWidget, logWidget, Qt::Vertical);
	addDockWidget(Qt::RightDockWidgetArea, loadModelTreeWidget);
	splitDockWidget(loadModelTreeWidget, openglWidget, Qt::Vertical);
	tabifyDockWidget(loadModelTreeWidget, ModelTreeWidget);
	loadModelTreeWidget->raise();
}

//初始化状态栏文字提示
void WarheadParaModel::InitStatusWidget()
{
	winStatus = new QStatusBar();
	pTipBar = new QLabel;
	pTipBar->setMinimumSize(1920, 20);
	pTipBar->setFrameShape(QFrame::WinPanel);
	pTipBar->setFrameShadow(QFrame::Sunken);
	winStatus->addWidget(pTipBar);
	setStatusBar(winStatus);
}


//初始化Ribbon中的文件菜单
void WarheadParaModel::InitCategoryMain(SARibbonCategory* page)
{
	SARibbonPannel* pannel = page->addPannel(("文件"));

	QAction* act = new QAction(this);
	act->setObjectName(("新建"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("新建"));
	act->setShortcut(QKeySequence(QLatin1String("Ctrl+N")));
	pannel->addLargeAction(act);
	connect(act, &QAction::triggered, this, &WarheadParaModel::NewFileAction);

	act = new QAction(this);
	act->setObjectName(("打开"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("打开"));
	pannel->addLargeAction(act);
	connect(act, &QAction::triggered, this, &WarheadParaModel::OpenFileAction);


	act = new QAction(this);
	act->setObjectName(("保存"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("保存"));
	act->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel->addLargeAction(act);
	connect(act, &QAction::triggered, this, &WarheadParaModel::SaveFileAction);


	act = new QAction(this);
	act->setObjectName(("另存为"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("另存为"));
	pannel->addLargeAction(act);
	connect(act, &QAction::triggered, this, &WarheadParaModel::SaveasFileAction);



	act = new QAction(this);
	act->setObjectName(("导出K文件"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("导出K文件"));
	pannel->addLargeAction(act);
	connect(act, &QAction::triggered, this, &WarheadParaModel::ExportFileAction);

	act = new QAction(this);
	act->setObjectName(("关闭"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("关闭"));
	pannel->addLargeAction(act);
	connect(act, &QAction::triggered, this, &WarheadParaModel::CloseFileAction);


	act = new QAction(this);
	act->setObjectName(("刷新"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("刷新"));
	pannel->addLargeAction(act);
	connect(act, &QAction::triggered, this, &WarheadParaModel::RefreshAction);

}
//初始化Ribbon
void WarheadParaModel::InitSARibbon()
{
	SARibbonBar* ribbon = ribbonBar();

	//应用按钮 当前隐藏
	ribbon->applicationButton()->hide();
	ribbon->applicationButton()->setText(("首页"));

	//初始化文件ribbon
	SARibbonCategory* categoryMain = new SARibbonCategory();
	categoryMain->setCategoryName(("主菜单"));
	categoryMain->setObjectName(("categoryMain"));
	ribbon->addCategoryPage(categoryMain);
	InitCategoryMain(categoryMain);

	//初始化顶部快捷菜单
	//InitQuickAccess(ribbon);
}

//初始化窗口
void WarheadParaModel::InitWindow()
{
	setWindowTitle(("战斗部参数化模型"));
	//设置界面Ribbon
	InitSARibbon();
	ribbonBar()->setRibbonStyle(SARibbonBar::RibbonStyle::OfficeStyleTwoRow);

	//初始化界面内容
	InitCentralWidget();

	//初始化界面状态栏
	InitStatusWidget();
	//设置最小宽度
	setMinimumWidth(1000);
	//窗口最大化
	showMaximized();
	//窗口图标
	setWindowIcon(QIcon(":/qss/res/qss/White/icon2.png"));
	//进制鼠标拖动
	//SAFramelessHelper* helper = framelessHelper();
	//helper->setRubberBandOnResize(false);
	//QElapsedTimer cost;
	//int lastTimes = 0;
	//cost.start();
	 
}
//初始化弹出窗口
void WarheadParaModel::InitTipWindow()
{
}
#pragma endregion

WarheadParaModel::WarheadParaModel(QWidget* parent)
	: SARibbonMainWindow(parent)
{
	//初始化系统变量
	InitSysData();
	//初始化系统路径
	InitPath();
	//初始化系统数据
	InitWarheadLib();

	//初始化界面
	InitWindow();
	InitTipWindow();
}

#pragma region 界面交互


/// <summary>
/// 新建场景
/// </summary>
void WarheadParaModel::NewFileAction()
{
	if (if_data == 1)
	{
		QMessageBox::information(NULL, "信息提示", "当前已有加载数据，请关闭后在新建");
		MyLogOutput("当前已有加载数据，请关闭后在新建");
		return;
	}
	if_data = 1;

	MyLogOutput("新建场景文件成功");
}
/// <summary>
/// 保存原来txt文件
/// </summary>
void WarheadParaModel::SaveFileAction()
{
	if (if_data == 0)
	{
		QMessageBox::information(NULL, "信息提示", "当前并无数据可保存");
	}
	else
	{

		MyLogOutput("当前文件保存成功");
	}
	return;
}
/// <summary>
/// 保存原来txt文件
/// </summary>
void WarheadParaModel::SaveasFileAction()
{
	//让用户选择文件夹，在选择的文件夹中保存文件,可以指定名字
	if (if_data == 0)
	{
		QMessageBox::information(NULL, "信息提示", "当前并无数据可保存");
	}
	else
	{

		MyLogOutput("文件另存成功");
	}
	return;
}
/// <summary>
/// 导出k文件
/// </summary>
void WarheadParaModel::ExportFileAction()
{
	if (if_data == 0)
	{
		QMessageBox::information(NULL, "信息提示", "当前并无K文件数据可导出");
	}
	else
	{
		MyLogOutput("K文件导出成功");
	}

	return;
}

/// <summary>
/// 刷新画布
/// </summary>
void WarheadParaModel::RefreshAction()
{
	graphicsViewMain->hide();
	graphicsViewMain->show();
}

/// <summary>
/// 关闭场景
/// </summary>
void WarheadParaModel::CloseFileAction()
{
	if_data = 0;

	SceneMainClear();
	MyLogOutput("清除数据成功");
}

/// <summary>
/// 保存战斗部信息
/// </summary>
void WarheadParaModel::ApplyDataAction()
{
	if (pArmHeadEdit.size() == 0)
		return;
	for (size_t i = 0; i < pArmHeadEdit.size()-1; i++)
	{
		vLoadWarhead.mapArmHead[i].nUnitPropty= pArmHeadEdit[i]->text().toFloat();
	} 
	AddSceneData();
	return;
}
/// <summary>
/// 打开场景
/// </summary>
void WarheadParaModel::OpenFileAction()
{
	QFileDialog* f = new QFileDialog(this);
	f->setWindowTitle("选择数据文件*.txt");
	f->setNameFilter("*.txt");
	f->setViewMode(QFileDialog::Detail);
	QString filePath;
	if (f->exec() == QDialog::Accepted) {
		filePath = f->selectedFiles()[0];
	}
	if (filePath == "")
	{
		return;
	}
	// 获取文件内容
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(NULL, "信息提示", "数据加载失败,请检查文件。");
		MyLogOutput("数据加载失败,请检查文件。");
		return;
	}
	existPath = filePath;

	QTextStream readStream(&file);
	int parsingState = 0;
	QString iconStr = "";
	QString versionStr = "";
	while (!readStream.atEnd()) {
		QString content = readStream.readLine();
		QStringList list = content.split(' ');

	}
	if_data = 1;

}



void WarheadParaModel::MyLogOutput(QString myLogout)
{
	if (myLogOutLabel->toPlainText() == "")
	{
		myLogOutLabel->setText(myLogout);
	}
	else
	{
		myLogOutLabel->setText(myLogout + "\r\n" + myLogOutLabel->toPlainText());
	}
}

#pragma endregion

#pragma region 初始化数据
// 初始化系统变量 
int WarheadParaModel::InitSysData()
{
	myLogOutLabel = new QTextEdit();
	pSceneOffset = 300;
	pAuxiliaryLine = 1500;
	if_data = 0;
	return 1;
}
// 初始化路径 
int WarheadParaModel::InitPath()
{
	// Exe文件完整路径
	oPath.sExePath = qApp->applicationFilePath().toLocal8Bit().data();
	// Exe文件所在目录
	oPath.sExeDir = qApp->applicationDirPath().toLocal8Bit().data();

	oPath.sTopoUnitDir = oPath.sExeDir + "/TopoUnit";
	oPath.sTopoLayerDir = oPath.sExeDir + "/TopoLayer";
	oPath.sWarheadDir = oPath.sExeDir + "/Warhead";


	oPath.sBoomLibDir = oPath.sExeDir + "/BoomLib";
	oPath.sModelLibDir = oPath.sExeDir + "/ModelLib";
	oPath.sProcLibDir = oPath.sExeDir + "/ProcLib";
	oPath.sTmpDir = oPath.sExeDir + "/Tmp";
	oPath.sParaLibDir = oPath.sExeDir + "/ParaLib";

	QString sLog = "完成系统路径初始化";
	MyLogOutput(sLog);

	return 1;
}

// 初始化基本构件库 
int WarheadParaModel::InitWarheadLib()
{
	QString Path = QString::fromStdString(oPath.sWarheadDir);
	QDir cfgPathDir = Path;
	if (!cfgPathDir.exists()) {
		MyLogOutput("系统无法找到系统战斗部库！");
		return 0;
	}
	QStringList filters;
	filters << QString("*.txt");
	cfgPathDir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
	cfgPathDir.setNameFilters(filters);                   //设置文件名称过滤器，只为filters格式
	int dirCount = cfgPathDir.count();
	if (dirCount <= 0) {
		MyLogOutput("系统战斗部库中无数据！");
		return 0;
	}
	//遍历该路径下的所有文件夹 
	for (int i = 0; i < dirCount; i++) {

		QString Path = QString("%1/%2").arg(QString::fromStdString(oPath.sWarheadDir)).arg(cfgPathDir[i]);
		QFile file(Path);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::information(NULL, "信息提示", "系统战斗部库解析失败！");
			MyLogOutput("系统战斗部库解析失败！");
			return 0;
		}
		vector<QPointF> turnPoint;
		QTextStream readStream(&file);
		QString versionStr = "";
		vector<PARADES> plist;
		while (!readStream.atEnd()) {
			QString content = readStream.readLine();
			QStringList list = content.split(' ');
			bool verify;
			QString str = list[0];
			if (list[0].toLower() == "*version")
			{
				versionStr = list[1];
				continue;
			}
			float  locationX = str.toInt(&verify);
			if (!verify)
				continue;
			PARADES p;
			p.sUnitName = QString(list[1]);
			p.nUnitPropty = list[2].toFloat();
			plist.push_back(p);

			if (list[1] == "外壳Shell")
			{
				for (size_t j = 0; j < list.size() - 3; j = j + 2)
				{
					turnPoint.push_back(QPointF(list[j + 3].toFloat(), list[j + 4].toFloat()));
				}
			}
		}
		ArmHeadTopo t;
		t.nArmHeadIdx = i;
		QFileInfo fileInfo(file.fileName());
		t.sArmHeadName = fileInfo.baseName();
		t.sArmHeadVersion = versionStr;
		t.vTurnPoint = turnPoint;
		t.mapArmHead = plist;
		vWarhead.push_back(t);
	}
	return 1;
}
// 初始化参数化生成模板
int WarheadParaModel::InitParaTmpl()
{
	return 0;
}
#pragma endregion

#pragma region 释放资源

WarheadParaModel::~WarheadParaModel()
{
	ReleaseUISource();
	ReleaseSysModel();
}

// 释放各种界面资源
void WarheadParaModel::ReleaseUISource()
{
	// 释放各种资源
	FREEPTR(pTipBar);
	FREEPTR(myLogOutLabel);
	FREEPTR(graphicsViewMain);
	return;
}

// 释放系统模型库
void WarheadParaModel::ReleaseSysModel()
{

	return;
}
#pragma endregion



#pragma region 画布操作

//画布增加数据
void WarheadParaModel::AddSceneData()
{
	/*if (if_data == 0)
		return;*/
		//清除画布
	SceneMainClear();
	DataConvert.CalPlaneData(vLoadWarhead, viewShape);

	//根据viewShape 绘制界面 
	for (size_t i = 0; i < viewShape.size(); i++)
	{
		if (viewShape[i].unitType == 1)//绘制矩形
		{
			BRectangle* viewItem = new BRectangle(
				viewShape[i].nCen[0], viewShape[i].nCen[1],
				viewShape[i].nWH[0], viewShape[i].nWH[1],
				BGraphicsItem::ItemType::Rectangle);
			viewItem->isAuxiliary = false;
			viewItem->nUnitType = viewShape[i].unitType;
			viewItem->nUnitIdx = viewShape[i].unitIdx;
			viewItem->setBrush(ColorHelper(viewShape[i].unitIdx));
			pSceneMain.addItem(viewItem);
		}
		else if (viewShape[i].unitType == 2)//绘制圆形
		{

		}
		else if (viewShape[i].unitType == 3)//绘制多边形
		{

		}
		else if (viewShape[i].unitType == 4)//绘制多边形
		{

			BLine* m_line = new BLine(BGraphicsItem::ItemType::Line);

			QPen pen = QPen(ColorHelper(viewShape[i].unitIdx), viewShape[i].nWH[0]);
			pen.setStyle(Qt::SolidLine);
			m_line->setPen(pen);
			if (viewShape[i].unitIdx == 7 || viewShape[i].unitIdx == 4)
			{
				m_line->setBrush(ColorHelper(viewShape[i].unitIdx));
			}
			for (size_t j = 0; j < viewShape[i].vCorner.size(); j++)
			{
				m_line->point.push_back(QPointF(viewShape[i].vCorner[j].nXY[0], viewShape[i].vCorner[j].nXY[1]));
				//m_line->lineWidth.push_back(viewShape[i].vCorner[j].nLineWidth);
			}
			pSceneMain.addItem(m_line);
		}
	}



	graphicsViewMain->hide();
	graphicsViewMain->show();
}

//清除画布数据
void WarheadParaModel::SceneMainClear()
{
	pSceneMain.clear();
	graphicsViewMain->setSceneRect(0, 0, pAuxiliaryLine / 2, pAuxiliaryLine / 2);
	for (int x = 0; x <= pAuxiliaryLine / 2; x += 10)
		pSceneMain.addLine(0, x, pAuxiliaryLine / 2, x, QPen(Qt::red));
	for (int y = 0; y <= pAuxiliaryLine / 2; y += 10)
		pSceneMain.addLine(y, 0, y, pAuxiliaryLine / 2, QPen(Qt::red));
}
#pragma endregion


QColor WarheadParaModel::ColorHelper(int unitIdx)
{
	if (unitIdx == 1)
	{
		return QColor(47, 65, 80);
	}
	else if (unitIdx == 2)
	{
		return QColor(69, 173, 206);
	}
	else if (unitIdx == 3)
	{
		return QColor(62, 179, 203);
	}
	else if (unitIdx == 4)
	{
		return QColor(255, 255, 255);
	}
	else if (unitIdx == 5)
	{
		return QColor(232, 220, 102);
	}
	else if (unitIdx == 6)
	{
		return QColor(170, 101, 96);
	}
	else if (unitIdx == 7)
	{
		return QColor(254, 235, 248);
	}
	else if (unitIdx == 8)
	{
		return QColor(204, 178, 102);
	}
	else if (unitIdx == 9)
	{
		return QColor(255, 127, 0);
	}
	else if (unitIdx == 11)
	{
		return QColor(255, 131, 158);
	}
	else if (unitIdx == 12)
	{
		return QColor(255, 255, 122);
	}
	else if (unitIdx == 13)
	{
		return QColor(190, 255, 255);
	}
	return QColor(72, 104, 146);
}
//重新加载模型属性
void WarheadParaModel::ReLoadModelProperty()
{
	if (vLoadWarhead.mapArmHead.size() == 0)
		return;
	QWidget* temp = new QWidget();
	pArmHeadEdit.clear();
	QFormLayout* pLayout = new QFormLayout();
	for (vector<PARADES>::const_iterator iter = vLoadWarhead.mapArmHead.begin(); iter != vLoadWarhead.mapArmHead.end(); iter++)
	{ 
		QLabel* unitName = new QLabel(iter->sUnitName);
		unitName->setFixedWidth(100);
		QLineEdit* txt = new QLineEdit(QString("%1").arg(iter->nUnitPropty));
		pLayout->addRow(unitName, txt);
		pArmHeadEdit.push_back(txt);
	}

	QLabel* unitName = new QLabel("曲线点");
	unitName->setFixedWidth(100);
	QLineEdit* txt = new QLineEdit(QString("%1").arg(5));
	pLayout->addRow(unitName, txt);
	pArmHeadEdit.push_back(txt);

	QPushButton* pApplyBtn = new QPushButton("应用", this);
	pApplyBtn->setIcon(QIcon(":/shaders/res/ToolIcon/run.png"));
	pApplyBtn->setFixedWidth(100);
	connect(pApplyBtn, &QPushButton::clicked, this, &WarheadParaModel::ApplyDataAction);
	pLayout->addRow(pApplyBtn);
	temp->setLayout(pLayout);
	LoadModeTreeProperty->setWidget(temp);
}
/// <summary>
/// 重新加载模型树
/// </summary>
void WarheadParaModel::ReLoadModelTree()
{
	if (vLoadWarhead.mapArmHead.size() == 0)
		return;
	QWidget* mytreewidget = new QWidget();
	QTreeWidget* pLoadModelTreeWidget = new QTreeWidget(mytreewidget);
	pLoadModelTreeWidget->setHeaderHidden(true);
	for (vector<PARADES>::const_iterator iter = vLoadWarhead.mapArmHead.begin(); iter != vLoadWarhead.mapArmHead.end(); iter++)
	{
		QTreeWidgetItem* rootItem = new QTreeWidgetItem(pLoadModelTreeWidget);
		rootItem->setText(0, iter->sUnitName);
		rootItem->setCheckState(0, Qt::Checked);
		rootItem->setData(0, Qt::UserRole, iter->sUnitName);
	}
	//复选选中
	connect(pLoadModelTreeWidget, &QTreeWidget::itemChanged, this, [=](QTreeWidgetItem* item, int column)
		{
			if (if_data == 0)
			{
				MyLogOutput("当前无画布信息，请新建或者打开后在操作");
				return;
			}
			QVariant variant = item->data(0, Qt::UserRole);
			QString sUnitName = variant.value<QString>();
			if (item->checkState(0) == Qt::Checked)
			{
				//选中
				int x = 0;
			}
			else
			{
				//未选中
				int x = 1;
			}
		});

	//树按钮响应
	connect(pLoadModelTreeWidget, &QTreeWidget::itemDoubleClicked, this, [=](QTreeWidgetItem* item, int column)
		{
			if (item->checkState(0) == Qt::Checked)
			{
				item->setCheckState(0, Qt::Unchecked);
			}
			else
			{
				item->setCheckState(0, Qt::Checked);
			}
		});

	LoadModeTreeWidget->setWidget(pLoadModelTreeWidget);
	pLoadModelTreeWidget->expandAll();
}