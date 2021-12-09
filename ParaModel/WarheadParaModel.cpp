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
#include "DimDataConvert.h"

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
	from->setWindowTitle("战斗部属性");
	from->setFixedWidth(300);
}

//初始化系统模型窗口
void WarheadParaModel::InitSysUnitWidget(QDockWidget* from)
{  
	from->setFixedWidth(300);
	from->setWindowTitle("系统战斗部模型");
}

//初始化已加载的模型窗口
void WarheadParaModel::InitLoadModelWidget(QDockWidget* from)
{   
	 
	from->setWindowTitle("模型结构树");
	from->setFixedWidth(300);
	//layout布局

	QWidget* visutal = new QWidget();


	from->setWidget(visutal);

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
	InitUnitLib();

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
/// 关闭场景
/// </summary>
void WarheadParaModel::CloseFileAction()
{
	if_data = 0;
	
	SceneMainClear(); 
	MyLogOutput("清除数据成功");
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
	pModelEdit[0] = new QLineEdit();
	pModelEdit[1] = new QLineEdit();
	myLogOutLabel = new QTextEdit();
	pSceneOffset = 4700;
	pAuxiliaryLine = 20000;
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
int WarheadParaModel::InitUnitLib()
{
	QString Path = QString::fromStdString(oPath.sTopoUnitDir + u8"/buildinglib.txt");
	QFile file(Path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(NULL, "信息提示", "系统基本构建库解析失败！");
		MyLogOutput("系统基本构建库解析失败！");
		return 0;
	}
	QTextStream readStream(&file);
	while (!readStream.atEnd()) {
		QString content = readStream.readLine();
		QStringList list = content.split(' ');
	 
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
	if (if_data == 0)
		return; 
	//清除画布
	SceneMainClear();

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
