#include <QTextCodec>

#include "ParaModel.h"
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
//初始化属性窗口
void ParaModel::InitPropertyWidget(QDockWidget* from)
{

	from->setMaximumWidth(300);
	from->setMaximumHeight(390);
	from->setWindowTitle("属性窗口");
	// 当前选择弹的属性
	QLabel* lbl1 = new QLabel("属性");
	QLabel* lblvalue = new QLabel("值");
	QLabel* lbl2 = new QLabel("位置XYZ");
	pModelEdit[0] = new QLineEdit;
	pModelEdit[1] = new QLineEdit;
	pModelEdit[2] = new QLineEdit;
	pModelEdit[0]->setText("0");
	pModelEdit[1]->setText("0");
	pModelEdit[2]->setText("0");
	QLabel* lbl3 = new QLabel("速度");
	pModelEdit[3] = new QLineEdit;
	pModelEdit[3]->setText("1000");
	QLabel* lbl4 = new QLabel("方向XYZ");
	pModelEdit[4] = new QLineEdit;
	pModelEdit[5] = new QLineEdit;
	pModelEdit[6] = new QLineEdit;
	pModelEdit[4]->setText("0");
	pModelEdit[5]->setText("-1");
	pModelEdit[6]->setText("0");

	// 当前选择弹的战斗部
	QLabel* lbl5 = new QLabel("当量Kg");
	pArmHeadEdit[0] = new QLineEdit;
	pArmHeadEdit[0]->setText("100");
	QLabel* lbl6 = new QLabel("弹片数量");
	pArmHeadEdit[1] = new QLineEdit;
	pArmHeadEdit[1]->setText("1000");
	QLabel* lbl7 = new QLabel(QWidget::tr("弹片质量g"));
	pArmHeadEdit[2] = new QLineEdit;
	pArmHeadEdit[2]->setText("20");
	QLabel* lbl8 = new QLabel(QWidget::tr("分布角"));
	pArmHeadEdit[3] = new QLineEdit;
	pArmHeadEdit[4] = new QLineEdit;
	pArmHeadEdit[3]->setText("60");
	pArmHeadEdit[4]->setText("20");
	QWidget* mywidget = new QWidget;
	QVBoxLayout* myVBoxLayout = new QVBoxLayout();
	QHBoxLayout* myHBoxLayout = new QHBoxLayout();

	QVBoxLayout* myLayout = new QVBoxLayout();


	QWidget* temp = new QWidget();
	lbl5->setFixedWidth(56);
	lbl6->setFixedWidth(56);
	lbl7->setFixedWidth(56);
	lbl8->setFixedWidth(56);
	lbl1->setFixedWidth(56);
	lbl2->setFixedWidth(56);
	lbl3->setFixedWidth(56);
	lbl4->setFixedWidth(56);
	//通过这个把其他挤在一起
	//QLabel* lbl9 = new QLabel(QWidget::tr(""));
	//lbl9->hide();
	//myVBoxLayout->setAlignment(Qt::AlignTop);
	// 	mygridLayout->setRowStretch(1, 1);
	// 	mygridLayout->setRowStretch(5, 10);
	//myVBoxLayout->setMargin(15);
	//最小宽度
	// 	mygridLayout->setColumnMinimumWidth(4, 10); 
	//myVBoxLayout->addWidget(temp);
	//myHBoxLayout->addWidget(lbl1);
	//myHBoxLayout->addWidget(lblvalue);
	//temp->setLayout(myHBoxLayout);
	//myLayout->addWidget(temp);

	temp = new QWidget();
	myHBoxLayout = new QHBoxLayout();
	myHBoxLayout->addWidget(lbl2);
	myHBoxLayout->addWidget(pModelEdit[0]);
	myHBoxLayout->addWidget(pModelEdit[1]);
	myHBoxLayout->addWidget(pModelEdit[2]);
	temp->setLayout(myHBoxLayout);
	myVBoxLayout->addWidget(temp);
	myLayout->addWidget(temp);


	temp = new QWidget();
	myHBoxLayout = new QHBoxLayout();
	myVBoxLayout->addWidget(temp);
	myHBoxLayout->addWidget(lbl3);
	myHBoxLayout->addWidget(pModelEdit[3]);
	temp->setLayout(myHBoxLayout);
	myLayout->addWidget(temp);

	temp = new QWidget();
	myHBoxLayout = new QHBoxLayout();
	myVBoxLayout->addWidget(temp);
	myHBoxLayout->addWidget(lbl4);
	myHBoxLayout->addWidget(pModelEdit[4]);
	myHBoxLayout->addWidget(pModelEdit[5]);
	myHBoxLayout->addWidget(pModelEdit[6]);
	temp->setLayout(myHBoxLayout);
	myLayout->addWidget(temp);

	temp = new QWidget();
	myHBoxLayout = new QHBoxLayout();
	myVBoxLayout->addWidget(temp);
	myHBoxLayout->addWidget(lbl5);
	myHBoxLayout->addWidget(pArmHeadEdit[0]);
	temp->setLayout(myHBoxLayout);
	myLayout->addWidget(temp);


	temp = new QWidget();
	myHBoxLayout = new QHBoxLayout();
	myVBoxLayout->addWidget(temp);
	myHBoxLayout->addWidget(lbl6);
	myHBoxLayout->addWidget(pArmHeadEdit[1]);
	temp->setLayout(myHBoxLayout);
	myLayout->addWidget(temp);


	temp = new QWidget();
	myHBoxLayout = new QHBoxLayout();
	myVBoxLayout->addWidget(temp);
	myHBoxLayout->addWidget(lbl7);
	myHBoxLayout->addWidget(pArmHeadEdit[2]);
	temp->setLayout(myHBoxLayout);
	myLayout->addWidget(temp);

	temp = new QWidget();
	myHBoxLayout = new QHBoxLayout();
	myVBoxLayout->addWidget(temp);
	myHBoxLayout->addWidget(lbl8);
	myHBoxLayout->addWidget(pArmHeadEdit[3]);
	myHBoxLayout->addWidget(pArmHeadEdit[4]);
	temp->setLayout(myHBoxLayout);
	myLayout->addWidget(temp);


	QPushButton* pApplyBtn = new QPushButton("应用", this);
	pApplyBtn->setIcon(QIcon(":/shaders/res/ToolIcon/run.png"));
	pApplyBtn->setFixedWidth(100);
	temp = new QWidget();
	myHBoxLayout = new QHBoxLayout();
	myHBoxLayout->addWidget(pApplyBtn);
	temp->setLayout(myHBoxLayout);
	myLayout->addWidget(temp);
	mywidget->setLayout(myLayout);
	from->setWidget(mywidget);
	connect(pApplyBtn, &QPushButton::clicked, this, &ParaModel::ApplyDataAction);
}

//初始化系统模型窗口
void ParaModel::InitSysWidget(QDockWidget* from)
{
	QWidget* mytreewidget = new QWidget();
	pModelTreeWidget = new QTreeWidget(mytreewidget);
	pModelTreeWidget->setHeaderHidden(true);
	from->setWidget(mytreewidget);
	from->setFixedWidth(300);
	from->setWindowTitle("模型组件/属性列表");


	QTreeWidgetItem* rootItemPillar = new QTreeWidgetItem(pModelTreeWidget);
	rootItemPillar->setText(0, "柱");
	QTreeWidgetItem* rootItemBeam = new QTreeWidgetItem(pModelTreeWidget);
	rootItemBeam->setText(0, "梁");
	QTreeWidgetItem* rootItemBoard = new QTreeWidgetItem(pModelTreeWidget);
	rootItemBoard->setText(0, "板");
	QTreeWidgetItem* rootItemWall = new QTreeWidgetItem(pModelTreeWidget);
	rootItemWall->setText(0, "墙");
	QTreeWidgetItem* rootItemDoor = new QTreeWidgetItem(pModelTreeWidget);
	rootItemDoor->setText(0, "门");
	QTreeWidgetItem* rootItemWindow = new QTreeWidgetItem(pModelTreeWidget);
	rootItemWindow->setText(0, "窗");

	for (vector<BasicUnit>::const_iterator iter = vBaseUnit.begin(); iter != vBaseUnit.end(); iter++)
	{

		QTreeWidgetItem* childItem;
#pragma region nUnitType赋值
		if (iter->nUnitType == 1)
		{
			childItem = new QTreeWidgetItem(rootItemPillar);
		}
		else if (iter->nUnitType == 2)
		{
			childItem = new QTreeWidgetItem(rootItemBeam);
		}
		else if (iter->nUnitType == 3)
		{
			childItem = new QTreeWidgetItem(rootItemBoard);
		}
		else if (iter->nUnitType == 4)
		{
			childItem = new QTreeWidgetItem(rootItemWall);
		}
		else if (iter->nUnitType == 5)
		{
			childItem = new QTreeWidgetItem(rootItemDoor);
		}
		else if (iter->nUnitType == 6)
		{
			childItem = new QTreeWidgetItem(rootItemWindow);
		}
#pragma endregion
		//矩形 1 圆 2 多边形3

		childItem->setText(0, iter->oShape.nShapeName);

		childItem->setData(0, Qt::UserRole, iter->nUnitIdx);
	}

	//树按钮响应
	connect(pModelTreeWidget, &QTreeWidget::itemDoubleClicked, this, [=](QTreeWidgetItem* item, int column) {
		if (if_data == 0)
		{
			MyLogOutput("当前无画布信息，请新建或者打开后在操作");
			return;
		}

		QTreeWidgetItem* parent = item->parent();
		if (NULL == parent) //注意：最顶端项是没有父节点的，双击这些项时注意(陷阱)
			return;

		QVariant variant = item->data(0, Qt::UserRole);
		int nUnitIdx = variant.value<int>();
		QString msg;
		for (vector<BasicUnit>::const_iterator iter = vBaseUnit.begin(); iter != vBaseUnit.end(); iter++)
		{
			if (iter->nUnitIdx != nUnitIdx)
				continue;

			if (iter->oShape.nShapeType == 1)
			{

				//板墙门窗 宽度为1
				int wThickNess = 0;
				int hThickNess = 0;
				if (iter->nUnitType > 2)
				{
					wThickNess = 1;
					hThickNess = 20;
				}
				BRectangle* m_rectangle = new BRectangle(
					pSceneOffset + iter->oShape.nShapeRange[0], pSceneOffset + iter->oShape.nShapeRange[1],
					wThickNess + iter->oShape.nShapeRange[2], hThickNess + iter->oShape.nShapeRange[3], BGraphicsItem::ItemType::Rectangle);
				m_rectangle->wallwidth = iter->oShape.nThickNess;
				m_rectangle->nUnitType = iter->nUnitType;
				m_rectangle->nUnitIdx = iter->nUnitIdx;
				m_rectangle->setBrush(ColorHelper(iter->nUnitType));
				pSceneMain.addItem(m_rectangle);
			}
			else if (iter->oShape.nShapeType == 2)
			{

				BCircle* m_ellipse = new BCircle(pSceneOffset + iter->oShape.nCen[0], pSceneOffset + iter->oShape.nCen[1],
					iter->oShape.nNumOrRadius, BGraphicsItem::ItemType::Circle);
				m_ellipse->setBrush(ColorHelper(iter->nUnitType));
				m_ellipse->nUnitType = iter->nUnitType;
				m_ellipse->nUnitIdx = iter->nUnitIdx;
				pSceneMain.addItem(m_ellipse);
			}
			else if (iter->oShape.nShapeType == 3)
			{
				vector<float> point;
				for (size_t i = 0; i < iter->oShape.vPolyPt.size(); i++)
				{
					point.push_back(iter->oShape.vPolyPt[i] + 100);
				}
				drawWall(point);

				//setBtnEnabled(false);
				/*BPolygon* m_polygon = new BPolygon(BGraphicsItem::ItemType::Polygon);
				m_polygon->is_create_finished = true;*/
				//m_polygon->paint();
				//pSceneMain.addItem(m_polygon);

			}
		}
		msg = item->text(0) + "构件加载完成";
		MyLogOutput(msg);
		});
	from->setWidget(pModelTreeWidget);
	pModelTreeWidget->expandAll();
}

//初始化已加载的模型窗口
void ParaModel::InitLoadModelWidget(QDockWidget* from)
{
	graphicsViewX = new BQGraphicsView();
	graphicsViewY = new BQGraphicsView();
	graphicsViewZ = new BQGraphicsView();

	graphicsViewOgl = new BQGraphicsView();


	pSceneX.setBackgroundBrush(Qt::darkGray);
	pSceneY.setBackgroundBrush(Qt::lightGray);
	pSceneZ.setBackgroundBrush(Qt::gray);

	graphicsViewX->setScene(&pSceneX);
	graphicsViewY->setScene(&pSceneY);
	graphicsViewZ->setScene(&pSceneZ);

	connect(graphicsViewX, &BQGraphicsView::GraphicsViewFocus, this, &ParaModel::GraphicsViewXFocus);
	connect(graphicsViewY, &BQGraphicsView::GraphicsViewFocus, this, &ParaModel::GraphicsViewYFocus);
	connect(graphicsViewZ, &BQGraphicsView::GraphicsViewFocus, this, &ParaModel::GraphicsViewZFocus);
	connect(graphicsViewOgl, &BQGraphicsView::GraphicsViewFocus, this, &ParaModel::GraphicsViewOgl);


	from->setWindowTitle("三视图+三维");
	//layout布局

	QGridLayout* myLayout = new QGridLayout();
	QWidget* temp = new QWidget();


	//myHBoxLayout->addWidget(graphicsViewX);
	myLayout->addWidget(graphicsViewX, 0, 0);
	myLayout->addWidget(graphicsViewY, 0, 1);
	myLayout->addWidget(graphicsViewZ, 1, 0);


	//右下角小三维窗口

	paraOglmanager = new ParaOGLManager();
	graphicsViewOgl->setViewport(paraOglmanager);
	myLayout->addWidget(graphicsViewOgl, 1, 1);



	temp->setLayout(myLayout);
	from->setWidget(temp);

}

//初始化日志窗口
void ParaModel::InitLogWidget(QDockWidget* from)
{
	from->setMaximumHeight(100);
	from->setFixedHeight(100);
	from->setWindowTitle("信息输出");
	if (myLogOutLabel == nullptr)
		myLogOutLabel = new QTextEdit();
	from->setWidget(myLogOutLabel);
}

//初始化OpenGL窗口
void ParaModel::InitOglManagerWidget(QDockWidget* from)
{
	MainDockWidget = from;

	graphicsViewMain = new BQGraphicsView();

	graphicsViewMain->setScene(&pSceneMain);
	pSceneMain.setBackgroundBrush(Qt::darkGray);

	MainDockWidget->setWindowTitle("当前编辑视图 （三维）");


	//中间大屏三位窗口

	MainDockState = 3;
	paraOglmanagerMain = new ParaOGLManager();
	MainDockWidget->setWidget(paraOglmanagerMain);
	MainDockWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);


	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &ParaModel::updateOGL);
	//每20ms刷新一次OpenGL界面 50FPS
	timer->start(20);



	//QTimer* timerSceneMain = new QTimer(this);
	//connect(timerSceneMain, &QTimer::timeout, this, &ParaModel::updateScene);
	//timerSceneMain->start(20);
}

//更新OpenGL窗口
void ParaModel::updateOGL()
{
	paraOglmanager->update();
	paraOglmanagerMain->update();


	//两个三维窗口要同步
	paraOglmanager->camera = paraOglmanagerMain->camera;
	paraOglmanager->isFirstMouse = paraOglmanagerMain->isFirstMouse;
	paraOglmanager->lastX = paraOglmanagerMain->lastX;
	paraOglmanager->lastY = paraOglmanagerMain->lastY;
	paraOglmanager->rotateRaw = paraOglmanagerMain->rotateRaw;
	paraOglmanager->rotatePitch = paraOglmanagerMain->rotatePitch;

	//传入所有的建筑数据

	paraOglmanager->oglTopTable = this->vModelTmpl;
	paraOglmanager->oglUnitTable = this->vBaseUnit;

	paraOglmanagerMain->oglTopTable = this->vModelTmpl;
	paraOglmanagerMain->oglUnitTable = this->vBaseUnit;



}

//初始化内容区域
void ParaModel::InitCentralWidget()
{
	//删除中央窗体
	QWidget* p = takeCentralWidget();
	if (p)
		delete p;

	setDockNestingEnabled(true);

	QDockWidget* sysWidget = new QDockWidget(this);
	InitSysWidget(sysWidget);

	QDockWidget* oglWidget = new QDockWidget(this);
	InitOglManagerWidget(oglWidget);

	QDockWidget* loadModelWidget = new QDockWidget(this);
	InitLoadModelWidget(loadModelWidget);

	QDockWidget* propertyWidget = new QDockWidget(this);
	InitPropertyWidget(propertyWidget);

	QDockWidget* logWidget = new QDockWidget(this);
	InitLogWidget(logWidget);



	addDockWidget(Qt::LeftDockWidgetArea, sysWidget);

	splitDockWidget(sysWidget, oglWidget, Qt::Horizontal);
	splitDockWidget(oglWidget, logWidget, Qt::Vertical);
	addDockWidget(Qt::RightDockWidgetArea, loadModelWidget);
	splitDockWidget(loadModelWidget, propertyWidget, Qt::Vertical);
	//tabifyDockWidget(loadModelWidget, propertyWidget);
	loadModelWidget->raise();

}

//初始化状态栏文字提示
void ParaModel::InitStatusWidget()
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
void ParaModel::InitCategoryMain(SARibbonCategory* page)
{
	SARibbonPannel* pannel = page->addPannel(("文件"));

	QAction* act = new QAction(this);
	act->setObjectName(("新建"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("新建"));
	act->setShortcut(QKeySequence(QLatin1String("Ctrl+N")));
	act->setCheckable(true);
	pannel->addLargeAction(act);
	connect(act, &QAction::triggered, this, &ParaModel::NewFileAction);

	act = new QAction(this);
	act->setObjectName(("打开"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("打开"));
	act->setShortcut(QKeySequence(QLatin1String("Ctrl+O")));
	pannel->addLargeAction(act);
	connect(act, &QAction::triggered, this, &ParaModel::OpenFileAction);

	act = new QAction(this);
	act->setObjectName(("保存"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("保存"));
	act->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel->addLargeAction(act);
	//connect(act, &QAction::triggered, this, &ParaModel::SaveFileAction);


	act = new QAction(this);
	act->setObjectName(("另存为"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("另存为"));
	pannel->addLargeAction(act);
	//connect(act, &QAction::triggered, this, &ParaModel::SaveasFileAction);


	act = new QAction(this);
	act->setObjectName(("关闭"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("关闭"));
	pannel->addLargeAction(act);
	connect(act, &QAction::triggered, this, &ParaModel::CloseFileAction);
}
//初始化Ribbon
void ParaModel::InitSARibbon()
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
void ParaModel::InitWindow()
{
	setWindowTitle(("参数化模型"));
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
void ParaModel::InitTipWindow()
{

}
#pragma endregion

ParaModel::ParaModel(QWidget* parent)
	: SARibbonMainWindow(parent)
{
	myLogOutLabel = new QTextEdit();
	pSceneOffset = 700;
	//初始化系统路径
	InitPath();
	//初始化系统数据
	InitUnitLib();

	//初始化界面
	InitWindow();
	InitTipWindow();
}

#pragma region 界面交互
void ParaModel::NewFileAction()
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

void ParaModel::CloseFileAction()
{
	if_data = 0;
	vModelTmpl.clear();
	SceneMainClear();
	SceneXClear();
	SceneYClear();
	SceneZClear();

	MyLogOutput("清除数据成功");
}
void ParaModel::OpenFileAction()
{
	if (if_data == 1)
	{
		QMessageBox::information(NULL, "信息提示", "当前已有加载数据，请关闭后在打开");
		MyLogOutput("当前已有加载数据，请关闭后在打开");
		return;
	}

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
	QTextStream readStream(&file);

	int parsingState = 0;
	while (!readStream.atEnd()) {
		QString content = readStream.readLine();
		QStringList list = content.split(' ');
		bool verify;
		QString str = list[0];
		if (list[0] == "*Topo")
		{
			parsingState = 1;
			continue;
		}
		float  locationX = str.toInt(&verify);
		if (!verify)
			continue;
		TopoUnit Topo;
		Topo.nUnitType = 0;
		Topo.nCenUnitIdx = 0;
		for (size_t i = 0; i < 10; i++)
		{
			Topo.nAdjUnitIdx[i] = -1;
		}
		Topo.nEdgeType = 0;
		Topo.nStatusFlag = 0;
		Topo.nUnitAngle = 0;
		for (size_t i = 0; i < 4; i++)
		{
			Topo.nCenPos[i] = 0;
		}
		if (parsingState == 0)
		{
			Topo.nUnitType = GetUnitTypeCode(list[1]);

			Topo.nCenUnitIdx = list[2].toInt();
			Topo.nTopoIdx = list[0].toInt();
			if (list.size() >= 4)
			{
				Topo.nCenPos[0] = list[3].toInt();
			}
			if (list.size() >= 5)
			{
				Topo.nCenPos[1] = list[4].toInt();
			}
			if (list.size() >= 6)
			{
				Topo.nCenPos[2] = list[5].toInt();
			}
			if (list.size() >= 7)
			{
				Topo.nCenPos[3] = list[6].toInt();
			}
			vModelTmpl.push_back(Topo);
		}
		else
		{
			for (size_t i = 0; i < vModelTmpl.size(); i++)
			{
				if (vModelTmpl[i].nTopoIdx == list[1].toInt())
				{
					for (size_t j = 0; j < list.size() - 2; j++)
					{
						vModelTmpl[i].nAdjUnitIdx[j] = list[j + 2].toInt();
					}
				}
			}
		}
	}
	if_data = 1;
	ParaModel::updateScene();
}

void ParaModel::GraphicsViewXFocus(bool b)
{
	MainDockWidget->setWindowTitle("当前编辑视图 （二维X）");
	if (MainDockState == 3)
	{
		MainDockWidget->setWidget(graphicsViewMain);
	}
	MainDockState = 0;
	pSceneMain.setBackgroundBrush(Qt::darkGray);
	return;
}

void ParaModel::GraphicsViewYFocus(bool b)
{
	MainDockWidget->setWindowTitle("当前编辑视图 （二维Y）");
	if (MainDockState == 3)
	{
		MainDockWidget->setWidget(graphicsViewMain);
	}
	MainDockState = 1;
	pSceneMain.setBackgroundBrush(Qt::lightGray);

	return;
}

void ParaModel::GraphicsViewZFocus(bool b)
{
	MainDockWidget->setWindowTitle("当前编辑视图 （二维Z）");
	if (MainDockState == 3)
	{
		MainDockWidget->setWidget(graphicsViewMain);
	}
	pSceneMain.setBackgroundBrush(Qt::gray);
	MainDockState = 2;


	return;
}

void ParaModel::GraphicsViewOgl(bool b)
{
	MainDockWidget->setWindowTitle("当前编辑视图 （三维模型）");
	if (MainDockState != 3)
	{
		MainDockState = 3;
		MainDockWidget->setWidget(paraOglmanagerMain);
	}
	return;
}

void ParaModel::MyLogOutput(QString myLogout)
{
	if (myLogOutLabel->toPlainText() == "")
	{
		myLogOutLabel->setText(myLogout);
	}
	else
	{
		myLogOutLabel->setText(myLogOutLabel->toPlainText() + "\r\n" + myLogout);
	}
}

// 数据修改后更新
void ParaModel::ApplyDataAction()
{  
	// 场景未 没有数据
	// if (pEnvir2 == NULL)
	// {
	// 	QMessageBox::information(NULL, "信息提示", "还未加载场景数据，无法保存！");
	// 	return;
	// }


	//组织界面上输入的数据
	//位置
	bool verify;
	QString str = pModelEdit[0]->text();
	float  locationX = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "信息提示", "位置X必须是数值，请重新输入");
	}
	if (locationX > 180 || locationX < -180)
	{
		QMessageBox::information(NULL, "信息提示", "位置X范围必须是正负180");
	}
	str = pModelEdit[1]->text();
	float locationY = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "信息提示", "位置Y必须是数值，请重新输入");
	}
	if (locationY > 180 || locationY < -180)
	{
		QMessageBox::information(NULL, "信息提示", "位置Y范围必须是正负180");
	}
	str = pModelEdit[2]->text();
	float locationZ = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "信息提示", "位置Z必须是数值，请重新输入");
	}
	if (locationZ > 180 || locationZ < -180)
	{
		QMessageBox::information(NULL, "信息提示", "位置Z范围必须是正负180");
	}
	//速度 
	str = pModelEdit[3]->text();
	float speed = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "信息提示", "速度必须是数值，请重新输入");
	}
	//方向XYZ
	str = pModelEdit[4]->text();
	float angleToX = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "信息提示", "方向X必须是数值，请重新输入");
	}
	if (angleToX > 180 || angleToX < -180)
	{
		QMessageBox::information(NULL, "信息提示", "方向X范围必须是正负180");
	}
	str = pModelEdit[5]->text();
	float angleToY = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "信息提示", "方向Y必须是数值，请重新输入");
	}
	if (angleToY > 180 || angleToY < -180)
	{
		QMessageBox::information(NULL, "信息提示", "方向Y范围必须是正负180");
	}
	str = pModelEdit[6]->text();
	float angleToZ = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "信息提示", "方向Z必须是数值，请重新输入");
	}
	if (angleToZ > 180 || angleToZ < -180)
	{
		QMessageBox::information(NULL, "信息提示", "方向Z范围必须是正负180");
	}
	//当量Kg	  
	str = pArmHeadEdit[0]->text();
	float tnt = str.toFloat(&verify);

	if (!verify)
	{
		QMessageBox::information(NULL, "信息提示", "TNT当量输入必须是数值，请重新输入");
	}
	//弹片数量	  
	str = pArmHeadEdit[1]->text();
	int fragNum = str.toInt(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "信息提示", "位置X输入必须是数值，请重新输入");
	}
	//弹片质量g 
	str = pArmHeadEdit[2]->text();
	float fragQuality = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "信息提示", "弹片质量必须是数值，请重新输入");
	}
	// 分布角 主飞散角  

	str = pArmHeadEdit[3]->text();
	float majorScatteringAngle = str.toFloat(&verify);

	if (!verify)
	{
		QMessageBox::information(NULL, "信息提示", "主飞散角必须是数值，请重新输入");
	}
	if (majorScatteringAngle > 90 || majorScatteringAngle < -90)
	{
		QMessageBox::information(NULL, "信息提示", "主飞散角范围必须是正负90");
	}
	// 分布角 副飞散角  
	str = pArmHeadEdit[4]->text();
	float secondScatteringAngle = str.toFloat(&verify);

	if (!verify)
	{
		QMessageBox::information(NULL, "信息提示", "副飞散角必须是数值，请重新输入");
	}
	if (secondScatteringAngle > 90 || secondScatteringAngle < -90)
	{
		QMessageBox::information(NULL, "信息提示", "副飞散角范围必须是正负90");
	}
	return;
}


void ParaModel::drawWall(const std::vector<float>& points) {
	MyItem* item = new MyItem;
	item->points = points;
	item->setPos(100, 100);
	item->setColor(QColor(Qt::red));
	pSceneMain.addItem(item);
}

#pragma endregion

#pragma region 初始化数据



int InitUnitPara(QStringList listInfo, BasicUnit& oUnit)
{
	// 0-5 柱梁板墙门窗
	QString sType = "柱梁板墙门窗";
	oUnit.nUnitIdx = listInfo[0].toInt();
	int nType = sType.indexOf(listInfo[1]);
	if (nType < 0)
		return 1;
	oUnit.nUnitType = nType;
	// 柱和梁类型   截面形状 0-2 矩形 圆形 多边形
	QString sShapeType = "矩形圆形多边形";
	int i = 0;
	if (nType < 2)
	{
		nType = sShapeType.indexOf(listInfo[2]);
		if (nType < 0)
			return 1;
		oUnit.oShape.nShapeType = nType / 2;
		switch (oUnit.oShape.nShapeType)
		{
		case 0:
			for (i = 0; i < 4; i++)
			{
				oUnit.oShape.vPolyPt.push_back(listInfo[3 + i].toInt());
			}
			break;
		case 1:
			oUnit.oShape.nCen[0] = listInfo[3].toInt();
			oUnit.oShape.nCen[1] = listInfo[4].toInt();
			oUnit.oShape.nNumOrRadius = listInfo[5].toInt();
			break;
		case 2:
			oUnit.oShape.nNumOrRadius = listInfo[3].toInt();
			for (i = 0; i < oUnit.oShape.nNumOrRadius; i++)
			{
				oUnit.oShape.vPolyPt.push_back(listInfo[4 + i].toInt());
			}
			break;
		default:
			break;
		}
	}
	// 墙和板类型
	if (nType == 2 || nType == 3)
		oUnit.oShape.nThickNess = listInfo[2].toInt();
	// 门和窗
	else
	{
		oUnit.oShape.nShapeType = 0;
		oUnit.oShape.vPolyPt.push_back(listInfo[3].toInt());
		oUnit.oShape.vPolyPt.push_back(listInfo[4].toInt());
	}
	return 0;
}
// 初始化路径 
int ParaModel::InitPath()
{
	// Exe文件完整路径
	oPath.sExePath = qApp->applicationFilePath().toLocal8Bit().data();
	// Exe文件所在目录
	oPath.sExeDir = qApp->applicationDirPath().toLocal8Bit().data();

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
int ParaModel::InitUnitLib()
{
	QString Path = QString::fromStdString(oPath.sExeDir + "/buildinglib.txt");
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
		BasicUnit basic;
		GeoShape shape;
		shape.nShapeType = 0;
		shape.nThickNess = 0;
		shape.nNumOrRadius = 0;
		shape.nShapeRange[0] = 0;
		shape.nShapeRange[1] = 0;
		shape.nCen[0] = 0;
		shape.nCen[1] = 0;
		shape.nShapeRange[2] = 0;
		shape.nShapeRange[3] = 0;
		vector<int> v;
		shape.vPolyPt = v;
		basic.nUnitIdx = list[0].toInt();
		QString shapeName = list[1];

#pragma region UnitType识别
		basic.nUnitType = GetUnitTypeCode(list[1]);
		if (basic.nUnitType == 3)
		{
			shapeName = list[2] + shapeName;
			shape.nThickNess = list[2].toInt();
			shape.nShapeType = 1;
		}
		if (basic.nUnitType == 4)
		{
			shapeName = list[2] + shapeName;
			shape.nThickNess = list[2].toInt();
			shape.nShapeType = 1;
		}
		if (basic.nUnitType > 4)
		{
			shape.nShapeType = 1;
		}
#pragma endregion

#pragma region 形状识别

		shape.nShapeType = GetShapeTypeCode(list[2]);
		if (shape.nShapeType == 1)
		{
			int unitType = GetUnitTypeCode(list[1]);
			if (unitType > 4) //如果是门和窗的情况
			{
				shapeName = list[3] + "-" + list[4] + " " + shapeName;
				shape.nShapeRange[0] = list[3].toInt();
				shape.nShapeRange[1] = list[4].toInt();
			}
			else
			{
				shapeName = list[2] + " " + shapeName;
				shape.nShapeRange[0] = list[3].toInt();
				shape.nShapeRange[1] = list[4].toInt();
				shape.nShapeRange[2] = list[5].toInt();
				shape.nShapeRange[3] = list[6].toInt();
			}
		}
		else if (shape.nShapeType == 2)
		{
			shapeName = list[2] + " " + shapeName;
			shape.nShapeType = 2;
			shape.nCen[0] = list[3].toInt();
			shape.nCen[1] = list[4].toInt();
			shape.nNumOrRadius = list[5].toInt();
		}
		else if (shape.nShapeType == 3)
		{
			shapeName = list[2] + " " + shapeName;
			shape.nShapeType = 3;
			shape.nNumOrRadius = list[3].toInt();
			for (size_t i = 0; i < shape.nNumOrRadius * 2; i++)
			{
				v.push_back(list[4 + i].toInt());
			}
			shape.vPolyPt = v;
		}
#pragma endregion

		shape.nShapeName = shapeName;
		basic.oShape = shape;
		vBaseUnit.push_back(basic);
	}
	return 1;
}
// 初始化平面图库
int ParaModel::InitPlaneDrawLib()
{
	return 0;
}
// 初始化参数化生成模板
int ParaModel::InitParaTmpl()
{
	return 0;
}
#pragma endregion

#pragma region 数据识别帮助方法

//获取系统构建库中的构建
BasicUnit ParaModel::GetBaseUnit(int idx)
{
	for (size_t i = 0; i < vBaseUnit.size(); i++)
	{
		if (vBaseUnit[i].nUnitIdx == idx)
			return vBaseUnit[i];
	}
	BasicUnit b;
	return b;
}


//获取加载构建集中的构建
TopoUnit ParaModel::GetTopoUnit(int idx)
{
	for (size_t i = 0; i < vModelTmpl.size(); i++)
	{
		if (vModelTmpl[i].nTopoIdx == idx)
			return vModelTmpl[i];
	}
	TopoUnit b;
	return b;
}
QColor ParaModel::ColorHelper(int nUnitType)
{
	if (nUnitType == 1)
	{
		return QColor(47, 65, 80);
	}
	else if (nUnitType == 2)
	{
		return QColor(69, 173, 206);
	}
	else if (nUnitType == 3)
	{
		return QColor(62, 179, 203);
	}
	else if (nUnitType == 4)
	{
		return QColor(64, 135, 163);
	}
	else if (nUnitType == 5)
	{
		return QColor(232, 220, 102);
	}
	else if (nUnitType == 6)
	{
		return QColor(170, 101, 96);
	}
	return QColor(72, 104, 146);
}
QString ParaModel::GetUnitType(int nUnitType)
{
	if (nUnitType == 1)
	{
		return "柱";
	}
	else if (nUnitType == 2)
	{
		return "梁";
	}
	else if (nUnitType == 3)
	{
		return "板";
	}
	else if (nUnitType == 4)
	{
		return  "墙";
	}
	else if (nUnitType == 5)
	{
		return "门";
	}
	else if (nUnitType == 6)
	{
		return "窗";
	}
	return "";
}
int ParaModel::GetUnitTypeCode(QString unitTypeStr)
{
	if (unitTypeStr == "柱")
	{
		return 1;
	}
	else if (unitTypeStr == "梁")
	{
		return 2;
	}
	else if (unitTypeStr == "板")
	{
		return 3;
	}
	else if (unitTypeStr == "墙")
	{
		return 4;
	}
	else if (unitTypeStr == "门")
	{
		return 5;
	}
	else if (unitTypeStr == "窗")
	{
		return 6;
	}
	return 0;
}
int ParaModel::GetShapeTypeCode(QString shapeTypeStr)
{
	if (shapeTypeStr == "矩形")
	{
		return 1;
	}
	else if (shapeTypeStr == "圆形")
	{
		return 2;
	}
	else if (shapeTypeStr == "多边形")
	{
		return 3;
	}
	return 0;
}
#pragma endregion

#pragma region 释放资源

ParaModel::~ParaModel()
{
	ReleaseUISource();
	ReleaseSysModel();
}

// 释放各种界面资源
void ParaModel::ReleaseUISource()
{
	// 释放各种资源
	FREEPTR(pTipBar);
	FREEPTR(myLogOutLabel);
	FREEPTR(graphicsViewMain);
	FREEPTR(graphicsViewOgl);
	FREEPTR(graphicsViewX);
	FREEPTR(graphicsViewY);
	FREEPTR(graphicsViewZ);
	FREEPTR(graphicsViewMain);
	return;
}


// 释放系统模型库
void ParaModel::ReleaseSysModel()
{

	return;
}
#pragma endregion

#pragma region 画布操作

//画布移动元素
void ParaModel::SceneItemMoveAction(int nUnitType, int nUnitIdx, QPointF pos)
{
	int a = nUnitType;
	int v = nUnitIdx;
	QPointF pos1 = pos;
	int s = a + v;
	//QList<QGraphicsItem*> itemList = pSceneMain.selectedItems();
	//for (size_t i = 0; i < itemList.size(); i++)
	//{
	//	BGraphicsItem* proxyWidget = qgraphicsitem_cast<BGraphicsItem*>(itemList[i]);
	//	if (!proxyWidget->isAuxiliary)
	//	{
	//		//获取到当前选中的
	//		proxyWidget->nUnitIdx;
	//		proxyWidget->nUnitType;
	//	}
	//}
	return;
}
//画布菜单点击
void ParaModel::SceneMenuClickAction(int nUnitType, int nUnitIdx, int clickType)
{
	return;
}

//更新画布内容
void ParaModel::updateScene()
{
	if (if_data == 0)
		return;
	if (vModelTmpl.size() == 0)
		return;


	DimDataConvert* d = new DimDataConvert();
	VSHAPE viewShape;
	d->CalPlaneData(vModelTmpl, viewShape, vBaseUnit);
	//只绘制柱、墙、梁
	//for (size_t i = 0; i < vModelTmpl.size(); i++)
	//{
	//	//柱梁板墙门窗
	//	if (vModelTmpl[i].nUnitType == 1)//柱
	//	{
	//		BasicUnit unit = GetBaseUnit(vModelTmpl[i].nCenUnitIdx);
	//		int coordX = vModelTmpl[i].nCenPos[0] + unit.oShape.nShapeRange[0] + pSceneOffset;
	//		int coordY = vModelTmpl[i].nCenPos[1] + unit.oShape.nShapeRange[1] + pSceneOffset;
	//		//根据构建id找到对应是构件
	//		BRectangle* pillar = new BRectangle(
	//			coordX, coordY,
	//			unit.oShape.nShapeRange[2], unit.oShape.nShapeRange[3],
	//			BGraphicsItem::ItemType::Rectangle);
	//		pillar->nUnitType = vModelTmpl[i].nUnitType;
	//		pillar->nUnitIdx = vModelTmpl[i].nCenUnitIdx;
	//		pillar->setBrush(ColorHelper(vModelTmpl[i].nUnitType));
	//		pSceneMain.addItem(pillar);
	//	}
	//}
	//根据数据绘制图形
	for (size_t i = 0; i < viewShape.size(); i++)
	{
		//绘制柱、墙、门、窗
		if (viewShape[i].unitType == 1 || viewShape[i].unitType == 4 || viewShape[i].unitType == 5 || viewShape[i].unitType == 6)
		{
			int coordX = viewShape[i].nCen[0] + pSceneOffset;
			int coordY = viewShape[i].nCen[1] + pSceneOffset;
			BRectangle* viewItem = new BRectangle(
				coordX, coordY,
				viewShape[i].nWH[0], viewShape[i].nWH[1],
				BGraphicsItem::ItemType::Rectangle);
			viewItem->isAuxiliary = false;
			viewItem->nUnitType = viewShape[i].unitType;
			viewItem->nUnitIdx = viewShape[i].unitIdx;
			viewItem->setBrush(ColorHelper(viewShape[i].unitType));
			connect(viewItem, &BRectangle::SceneItemMove, this, &ParaModel::SceneItemMoveAction);
			connect(viewItem, &BRectangle::SceneMenuClick, this, &ParaModel::SceneMenuClickAction);
			pSceneMain.addItem(viewItem);
		}

	}
	for (size_t i = 0; i < viewShape.size(); i++)
	{
		//根据墙的角度判断线的方向
		if (viewShape[i].unitType == 4)
		{
			int coordX = viewShape[i].nCen[0] + pSceneOffset;
			int coordY = viewShape[i].nCen[1] + pSceneOffset;
			if (vModelTmpl[viewShape[i].unitIdx].nUnitAngle == 0)
			{
				BRectangle* divideLine = new BRectangle(
					1, coordY,
					8000, 2,
					BGraphicsItem::ItemType::Rectangle);
				divideLine->isAuxiliary = true;
				divideLine->nUnitIdx = viewShape[i].unitIdx;
				/*QBrush b = (Qt::DashLine);
				b.setColor(ColorHelper(viewShape[i].unitType));
				divideLine->setBrush(b);*/
				QPen pen = QPen(Qt::yellow);
				pen.setStyle(Qt::DashLine);
				divideLine->setPen(pen);
				pSceneMain.addItem(divideLine);
			}
			else
			{
				BRectangle* divideLine = new BRectangle(
					coordX, 1,
					2, 8000,
					BGraphicsItem::ItemType::Rectangle);
				divideLine->isAuxiliary = true;
				divideLine->nUnitIdx = viewShape[i].unitIdx;
				/*QBrush b = (Qt::DashLine);
				b.setColor(ColorHelper(viewShape[i].unitType));
				divideLine->setBrush(b);*/
				QPen pen = QPen(Qt::yellow);
				pen.setStyle(Qt::DashLine);
				divideLine->setPen(pen);
				pSceneMain.addItem(divideLine);
			}

		}
	}
}

void ParaModel::UpdataSceneItem(int nUnitIdx, int x, int y, int width, int height)
{
	bool isWall = false;
	QList<QGraphicsItem*> itemList = pSceneMain.items();
	for (size_t i = 0; i < itemList.size(); i++)
	{
		BGraphicsItem* proxyWidget = qgraphicsitem_cast<BGraphicsItem*>(itemList[i]);
		if (proxyWidget->nUnitIdx == nUnitIdx)
		{
			if (!proxyWidget->isAuxiliary)
			{
				proxyWidget->m_leftup = QPointF(pSceneOffset + x, pSceneOffset + y);
				proxyWidget->m_edge = QPointF(pSceneOffset + x + width, pSceneOffset + y + height);
			}
			else
			{
				pSceneMain.removeItem(proxyWidget);
				isWall = true;
			}
		}
	}
	if (isWall)
	{
		if (vModelTmpl[nUnitIdx].nUnitAngle == 0)
		{
			int center = (width / 2);
			BRectangle* divideLine = new BRectangle(
				1, pSceneOffset + x + center,
				8000, 2,
				BGraphicsItem::ItemType::Rectangle);
			divideLine->isAuxiliary = true;
			divideLine->nUnitIdx = nUnitIdx;
			divideLine->wallwidth = 0;
			QPen pen = QPen(Qt::yellow);
			pen.setStyle(Qt::DashLine);
			divideLine->setPen(pen);
			pSceneMain.addItem(divideLine);
		}
		else
		{
			int center = (height / 2);
			BRectangle* divideLine = new BRectangle(
				pSceneOffset + y + center, 1,
				2, 8000,
				BGraphicsItem::ItemType::Rectangle);
			divideLine->isAuxiliary = true;
			divideLine->nUnitIdx = nUnitIdx;
			QPen pen = QPen(Qt::yellow);
			pen.setStyle(Qt::DashLine);
			divideLine->setPen(pen);
			pSceneMain.addItem(divideLine);
		}
	}
}
void ParaModel::SceneMainClear()
{
	pSceneMain.clear();
	for (int x = 0; x <= 2000; x += 20)
		pSceneMain.addLine(0, x, 2000, x, QPen(Qt::red));
	for (int y = 0; y <= 2000; y += 20)
		pSceneMain.addLine(y, 0, y, 2000, QPen(Qt::red));
}
void ParaModel::SceneXClear()
{
	pSceneX.clear();
	for (int x = 0; x <= 2000; x += 20)
		pSceneX.addLine(0, x, 2000, x, QPen(Qt::red));
	for (int y = 0; y <= 2000; y += 20)
		pSceneX.addLine(y, 0, y, 2000, QPen(Qt::red));
}
void ParaModel::SceneYClear()
{
	pSceneY.clear();
	for (int x = 0; x <= 2000; x += 20)
		pSceneY.addLine(0, x, 2000, x, QPen(Qt::red));
	for (int y = 0; y <= 2000; y += 20)
		pSceneY.addLine(y, 0, y, 2000, QPen(Qt::red));
}
void ParaModel::SceneZClear()
{
	pSceneZ.clear();
	for (int x = 0; x <= 2000; x += 20)
		pSceneZ.addLine(0, x, 2000, x, QPen(Qt::red));
	for (int y = 0; y <= 2000; y += 20)
		pSceneZ.addLine(y, 0, y, 2000, QPen(Qt::red));
}
#pragma endregion
