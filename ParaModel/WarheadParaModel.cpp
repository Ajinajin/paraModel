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

#include "BezierOpt.h"
#include "BezierWidget.h"

#include "ArmHeadDesign.h"
#include "ArmDesignWidget.h"
#include <QColorDialog>

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
	connect(pWarheadModelTreeWidget, &QTreeWidget::itemDoubleClicked, this, &WarheadParaModel::NewArmHeadTmpl);

	from->setWidget(pWarheadModelTreeWidget);
	pWarheadModelTreeWidget->expandAll();
}

void WarheadParaModel::NewArmHeadTmpl(QTreeWidgetItem* item, int column)
{
	DataConvert.bFirst = true;
	QVariant variant = item->data(0, Qt::UserRole);
	int nArmHeadIdx = variant.value<int>();
	vLoadWarhead = vWarhead[nArmHeadIdx];
	//清空变成默认颜色
	defColor.clear();
	pArmHeadWnd->defColor.clear();
	ReLoadModelTree();
	ReLoadModelProperty();
	AddSceneData();
	if (if_data == 0)
	{
		MyLogOutput("当前无画布信息，请新建或者打开后在操作");
		return;
	}

	return;
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
//初始化日志窗口
void WarheadParaModel::InitCurveWidget(QDockWidget* from)
{
	from->setMaximumHeight(1200);
	// 	from->setFixedHeight(600);
	from->setWindowTitle("参数化设计");
	pArmHeadWnd = NULL;
	if (pArmHeadWnd == NULL)
		pArmHeadWnd = new ArmDesignWidget();
	connect(pArmHeadWnd, &ArmDesignWidget::OutLog, this, &WarheadParaModel::MyLogOutput);
	from->setWidget(pArmHeadWnd);
	// 	from->setMaximumHeight(1200);
	// // 	from->setFixedHeight(600);
	// 	from->setWindowTitle("参数化设计");
	// 	pCurveWnd = NULL; 
	// 	if (pCurveWnd == NULL)
	// 		pCurveWnd = new BezierWidget();
	// 	connect(pCurveWnd, &BezierWidget::OutLog, this, &WarheadParaModel::MyLogOutput); 
	// 	pCurveWnd->lstUpCtrlPts.clear();
	// 	int nPt = listUpCurveCtrlPt.size();
	// 	for (int i = 0; i < nPt; i++)
	// 	{
	// 		QPoint oPt(listUpCurveCtrlPt[i].x(), listUpCurveCtrlPt[i].y()); 
	// 		pCurveWnd->lstUpCtrlPts.push_back(oPt);
	// 	}
	// 	from->setWidget(pCurveWnd);
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
	warheadParaOglmanager = new WarheadOGLManager();
	warheadParaOglmanager->setMinimumHeight(100);
	from->setWindowTitle("三维视图");
	from->setWidget(warheadParaOglmanager);
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
	warheadParaOglmanager->update();
	if (!vLoadWarhead.mapArmHead.empty())
	{
		warheadParaOglmanager->oglWarhead = vLoadWarhead;
	}
	if (defColor.size() != 0)
	{
		warheadParaOglmanager->oglDefColor = defColor;
	}
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

	QDockWidget* CurveWidget = new QDockWidget(this);
	InitCurveWidget(CurveWidget);

	addDockWidget(Qt::LeftDockWidgetArea, sysWidget);
	splitDockWidget(sysWidget, editManagerWidget, Qt::Horizontal);
	splitDockWidget(editManagerWidget, logWidget, Qt::Vertical);
	splitDockWidget(logWidget, CurveWidget, Qt::Horizontal);
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
	// 测试功能
// 	CArmHeadDesign oDesign;
// 	oDesign.ReadArmHeadInfo("");

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
	DataConvert.bFirst = true;
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
	//将数据变成 输出成txt文件
	//获取数据，将数据变成k文件
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
		if (warheadParaOglmanager->allNodes.size() != 0)
		{
			//让用户选择文件夹，在选择的文件夹中导出k文件,可以指定名字

			QString curPath = QCoreApplication::applicationDirPath();
			QString dlgTitle = "保存文件";
			QString filter = "h文件(*.h);;c++文件(*.cpp);;k文件(*.k);;所有文件(*.*)";

			QString dirpath = QFileDialog::getSaveFileName(this, dlgTitle, curPath, filter);


			fstream outfile;
			outfile.open(dirpath.toStdString(), fstream::out);

			outfile << "*KEYWORD" << "\n";
			outfile << "*NODE" << "\n";

			for (int index = 0; index < warheadParaOglmanager->allNodes.size(); index++)
			{
				outfile << std::setw(8) << (index + 1) << std::setw(16) << std::scientific << std::uppercase << std::setprecision(8) << float(warheadParaOglmanager->allNodes[index].fXYZ[0] / 100.0)
					<< std::setw(16) << std::scientific << std::uppercase << std::setprecision(8) << float(warheadParaOglmanager->allNodes[index].fXYZ[1] / 100.0)
					<< std::setw(16) << std::scientific << std::uppercase << std::setprecision(8) << float(warheadParaOglmanager->allNodes[index].fXYZ[2] / 100.0) << std::setw(8) << "0" << std::setw(8) << "0" << std::endl;
			}

			outfile << "*FRAGMENTS-POSITION" << "\n";
			for (int index = 0; index < warheadParaOglmanager->allFragPos.size(); index++)
			{
				outfile << /*std::setw(8) << (index + 1) << */std::setw(16) << std::scientific << std::uppercase << std::setprecision(8) << float(warheadParaOglmanager->allFragPos[index].fXYZ[0] / 100.0)
					<< std::setw(16) << std::scientific << std::uppercase << std::setprecision(8) << float(warheadParaOglmanager->allFragPos[index].fXYZ[1] / 100.0)
					<< std::setw(16) << std::scientific << std::uppercase << std::setprecision(8) << float(warheadParaOglmanager->allFragPos[index].fXYZ[2] / 100.0) /*<< std::setw(8) << "0" << std::setw(8) << "0" */<< std::endl;
			}
			MyLogOutput("K文件导出成功");

		}


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
/// 设置构件颜色属性
/// </summary>
void WarheadParaModel::SetingUnitColorAction(int UnitIndex, QString UnitName)
{
	//QColor color = QColorDialog::getColor(Qt::gray, this, "选择你要的颜色"); 
	QColor color = QColorDialog::getColor(Qt::white, NULL, "选择你要的颜色", QColorDialog::ShowAlphaChannel);

	//修改颜色
	defColor[UnitIndex] = color;
	//修改右侧菜单
	ReLoadModelTree();
	//刷新画布
	AddSceneData();
}
/// <summary>
/// 保存战斗部信息
/// </summary>
void WarheadParaModel::ApplyDataAction()
{
	if (pArmHeadEdit.size() == 0)
		return;
	for (size_t i = 0; i < pArmHeadEdit.size() - 1; i++)
	{
		vLoadWarhead.mapArmHead[i].nUnitPropty = pArmHeadEdit[i]->text().toFloat();
	}
	AddSceneData();
	return;
}
/// <summary>
/// 打开场景
/// </summary>
void WarheadParaModel::OpenFileAction()
{
	//打开保存的数据文件
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
		QList<QPointF> turnPoint;
		QTextStream readStream(&file);
		QString versionStr = "";
		vector<PARADES> plist;
		int unitindex = 0;
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
			p.nUnitIndex = unitindex;
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
			unitindex++;
		}
		ArmHeadTopo t;
		t.nArmHeadIdx = i;
		QFileInfo fileInfo(file.fileName());
		t.sArmHeadName = fileInfo.baseName();
		t.sArmHeadVersion = versionStr;
		t.vCtrlPts = turnPoint;
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

int TransPixelToPt(VPIXELPOS const& vPix, QList<QPointF>& lstPt)
{
	int nPtNum = vPix.size();
	int i = 0;
	lstPt.clear();
	for (i = 0; i < nPtNum; i++)
	{
		QPointF oPt(vPix[i].nXY[0], vPix[i].nXY[1]);
		lstPt.push_back(oPt);
	}
	return  0;
}
//画布增加数据
void WarheadParaModel::AddSceneData()
{
	/*if (if_data == 0)
		return;*/
		//清除画布
	SceneMainClear();
	pSceneMain.nArmHeadType = vLoadWarhead.nArmHeadIdx;
	DataConvert.GetArmHeadPara(vLoadWarhead);
	DataConvert.CaArmHeadData(vOriginShape);
	QSize oSize(600, 300);
	DataConvert.CenterArmHeadData(vOriginShape, vDrawShape, oSize);
	// 转存画布上绘图坐标
	TransPixelToPt(vDrawShape[2].vCorner, listUpCurveCtrlPt);
	TransPixelToPt(vDrawShape[3].vCorner, listDnCurveCtrlPt);
	pSceneMain.listDnCurveCtrlPt = listDnCurveCtrlPt;
	pSceneMain.listUpCurveCtrlPt = listUpCurveCtrlPt;

	//根据vDrawShape 绘制界面 
	for (size_t i = 0; i < vDrawShape.size(); i++)
	{
		if (vDrawShape[i].unitType == 1)//绘制矩形
		{
			BRectangle* viewItem = new BRectangle(
				vDrawShape[i].nCen[0], vDrawShape[i].nCen[1],
				vDrawShape[i].nWH[0], vDrawShape[i].nWH[1],
				BGraphicsItem::ItemType::Rectangle);
			viewItem->isAuxiliary = false;
			viewItem->nUnitType = vDrawShape[i].unitType;
			viewItem->nUnitIdx = vDrawShape[i].unitIdx;
			viewItem->setBrush(ColorHelper(vDrawShape[i].unitIdx));
			pSceneMain.addItem(viewItem);
		}
		else if (vDrawShape[i].unitType == 2)//绘制圆形
		{

		}
		else if (vDrawShape[i].unitType == 3)//绘制曲线
		{
			BCurveLine* pCurveLine = new BCurveLine(BGraphicsItem::ItemType::Curve);
			pCurveLine->isAuxiliary = false;
			pCurveLine->nUnitType = vDrawShape[i].unitType;
			pCurveLine->nUnitIdx = vDrawShape[i].unitIdx;
			QPen pen = QPen(ColorHelper(2), 1);
			pen.setStyle(Qt::SolidLine);
			QList<QPointF> lstPt;
			TransPixelToPt(vDrawShape[i].vCorner, lstPt);
			pCurveLine->setPen(pen);
			pCurveLine->lstCtrlPt = lstPt;

			pSceneMain.addItem(pCurveLine);
		}
		else if (vDrawShape[i].unitType == 4)//绘制直线
		{

			BLine* m_line = new BLine(BGraphicsItem::ItemType::Line);

			QPen pen = QPen(ColorHelper(vDrawShape[i].unitIdx), vDrawShape[i].nWH[0]);
			pen.setStyle(Qt::SolidLine);
			m_line->setPen(pen);
			if (vDrawShape[i].unitIdx == 7 || vDrawShape[i].unitIdx == 4)
			{
				m_line->setBrush(ColorHelper(vDrawShape[i].unitIdx));
			}
			for (size_t j = 0; j < vDrawShape[i].vCorner.size(); j++)
			{
				m_line->point.push_back(QPointF(vDrawShape[i].vCorner[j].nXY[0], vDrawShape[i].vCorner[j].nXY[1]));
			}
			pSceneMain.addItem(m_line);
		}
	}

	// 赋给独立的曲线窗口 完成曲线窗口相关变量赋值
	oSize = pArmHeadWnd->size();
	pArmHeadWnd->oPreSize = oSize;
	pArmHeadWnd->oArmHeadDesign.ReadArmHeadInfo("");
	pArmHeadWnd->vDesignShape = vOriginShape;
	pArmHeadWnd->lstUpCurveCtrlPts = DataConvert.vCtrlPts;
	memcpy(pArmHeadWnd->fR, DataConvert.fR, 20 * sizeof(float));
	memcpy(pArmHeadWnd->fT, DataConvert.fT, 10 * sizeof(float));
	memcpy(pArmHeadWnd->fH, DataConvert.fH, 10 * sizeof(float));
	// 需要计算初始曲线轮廓
	pArmHeadWnd->bCalPartShape = true;
	pArmHeadWnd->update();
	// 	oSize = pCurveWnd->size(); 
	// 	pCurveWnd->oPreSize = oSize; 
	// 	pCurveWnd->vDesignShape = vOriginShape;
	// 	pCurveWnd->lstUpCtrlPts = DataConvert.vCtrlPts; 
	// 	memcpy(pCurveWnd->fR, DataConvert.fR, 20 * sizeof(float)); 
	// 	memcpy(pCurveWnd->fT, DataConvert.fT, 10 * sizeof(float));
	// 	memcpy(pCurveWnd->fH, DataConvert.fH, 10 * sizeof(float));
	// 	// 需要计算初始曲线轮廓
	// 	pCurveWnd->bCalPartShape = true; 
	// 	pCurveWnd->update(); 

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

QString WarheadParaModel::QColorToString(const QColor& color, const QString type)
{
	if (type == "RGBA") {
		return QString("rgba(%1, %2, %3, %4)")
			.arg(color.red())
			.arg(color.green())
			.arg(color.blue())
			.arg(color.alpha());
	}
	if (type == "RGB") {
		return QString("rgba(%1, %2, %3)")
			.arg(color.red())
			.arg(color.green())
			.arg(color.blue());
	}
	if (type == "HEX") {
		return QString().sprintf("#%1%02X%02X%02X",
			color.red(),
			color.green(),
			color.blue()).arg(color.alpha() != 255 ? QString().sprintf("%02X", color.alpha()) : QString());
	}

	return color.name();
}

QColor WarheadParaModel::ColorHelper(int unitIdx)
{ 
	if (defColor.size() == 0)
	{
		defColor.push_back(QColor(72, 104, 146));
		defColor.push_back(QColor(47, 65, 80));
		defColor.push_back(QColor(69, 173, 206));
		defColor.push_back(QColor(62, 179, 203));
		defColor.push_back(QColor(255, 255, 255));
		defColor.push_back(QColor(232, 220, 102));
		defColor.push_back(QColor(170, 101, 96));
		defColor.push_back(QColor(254, 235, 248));
		defColor.push_back(QColor(204, 178, 102));
		defColor.push_back(QColor(255, 127, 0));
		defColor.push_back(QColor(255, 131, 158));
		defColor.push_back(QColor(255, 255, 122));
		defColor.push_back(QColor(190, 255, 255));
		defColor.push_back(QColor(0, 0, 0));
	}
	if (unitIdx >= defColor.size())
	{
		defColor.push_back(QColor(72, 104, 146));
		defColor.push_back(QColor(47, 65, 80));
		defColor.push_back(QColor(69, 173, 206));
		defColor.push_back(QColor(62, 179, 203));
		defColor.push_back(QColor(255, 255, 255));
		defColor.push_back(QColor(232, 220, 102));
		defColor.push_back(QColor(170, 101, 96));
		defColor.push_back(QColor(254, 235, 248));
		defColor.push_back(QColor(204, 178, 102));
		defColor.push_back(QColor(255, 127, 0));
		defColor.push_back(QColor(255, 131, 158));
		defColor.push_back(QColor(255, 255, 122));
		defColor.push_back(QColor(190, 255, 255));
		defColor.push_back(QColor(0, 0, 0));
	}
	/*if (unitIdx == 1)
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
	}*/
	return defColor[unitIdx];
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
	pLoadModelTreeWidget->setColumnCount(2);
	//第一行宽度 
	pLoadModelTreeWidget->setColumnWidth(0, 150);
	for (vector<PARADES>::const_iterator iter = vLoadWarhead.mapArmHead.begin(); iter != vLoadWarhead.mapArmHead.end(); iter++)
	{
		QTreeWidgetItem* rootItem = new QTreeWidgetItem(pLoadModelTreeWidget);
		rootItem->setText(0, iter->sUnitName);
		QPushButton* ptn = new QPushButton(this);
		connect(ptn, &QPushButton::clicked, this, [=](bool checked) {
			SetingUnitColorAction(iter->nUnitIndex, iter->sUnitName);
			});
		//根据加载名称的id获取颜色 并且去掉边框
		//根据id获取颜色
		QString colorStr = QColorToString(ColorHelper(iter->nUnitIndex),"RGBA");
		ptn->setStyleSheet(QString("background-color:%1;border:none").arg(colorStr));
		rootItem->setCheckState(0, Qt::Checked);
		rootItem->setData(0, Qt::UserRole, iter->sUnitName);
		rootItem->setData(1, Qt::UserRole, iter->nUnitIndex);
		pLoadModelTreeWidget->setItemWidget(rootItem, 1, ptn);
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

			variant = item->data(1, Qt::UserRole);
			qint32 sUnitIdx = variant.value<qint32>();
			if (item->checkState(0) == Qt::Checked)
			{
				//选中
				warheadParaOglmanager->showFlags[sUnitIdx] = GL_TRUE;		//三维显示窗口对应的构件显示
			}
			else
			{
				//未选中
				warheadParaOglmanager->showFlags[sUnitIdx] = GL_FALSE;		//三维显示窗口对应的构件不显示
				
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


void WarheadParaModel::mousePressEvent(QMouseEvent* event)
{
	// 画布上的点击点
	QPointF oCurPos = pSceneMain.oScePt;
	QPointF oPos = event->pos();
	nMoveXY[0] = oPos.x() - oCurPos.x();
	nMoveXY[1] = oPos.y() - oCurPos.y();

	return;
}
void WarheadParaModel::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		iUpSelected = -1;
		iDnSelected = -1;
		update();
	}

	return;
}
void WarheadParaModel::mouseMoveEvent(QMouseEvent* event)
{
	if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton)
	{
		// 屏幕坐标
		QPointF oPos = event->pos();
		// 画布坐标
		QPointF oScePos(oPos.x() - nMoveXY[0], oPos.y() - nMoveXY[1]);
		// 		if( iUpSelected >= 0 )
		// 		{
		// 			listUpCurveCtrlPt[iUpSelected] = oScePos;
		// 			// 另一条控制线对称操作
		// 		}
		// 		else if (iDnSelected >= 0 )
		// 		{
		// 			listDnCurveCtrlPt[iDnSelected] = oScePos;
		// 		}
				// 战斗部相对坐标系
		// 		QPointF oHeadCoorSysPos(oScePos.x()-600, oScePos.y()-300); 
		// 		vLoadWarhead.vTurnPoint[iSelected] = oHeadCoorSysPos; 

		update();
	}

	return;
}
void WarheadParaModel::keyPressEvent(QKeyEvent* event)
{
	int iPoint;

	switch (event->key())
	{
	case Qt::Key_Escape:
		if (!newUpPoints.empty())
		{
			iPoint = newUpPoints.pop();
			listUpCurveCtrlPt.removeAt(iPoint);

			if (iPoint == iUpSelected)
				iUpSelected = -1;
			update();
		}
		break;
	default:
		break;
	}
	return;
}
