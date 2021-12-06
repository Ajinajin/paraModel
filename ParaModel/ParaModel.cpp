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
void ParaModel::InitLayerWidget(QDockWidget* from)
{
	from->setMaximumWidth(300);
	from->setWindowTitle("建筑楼层");
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
	connect(pModelTreeWidget, &QTreeWidget::itemDoubleClicked, this, [=](QTreeWidgetItem* item, int column)
		{
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

	//ParaModel::RefreshScene();

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

	layerWidget = new QDockWidget(this);
	InitLayerWidget(layerWidget);

	QDockWidget* logWidget = new QDockWidget(this);
	InitLogWidget(logWidget);



	addDockWidget(Qt::LeftDockWidgetArea, sysWidget);

	splitDockWidget(sysWidget, oglWidget, Qt::Horizontal);
	splitDockWidget(oglWidget, logWidget, Qt::Vertical);
	addDockWidget(Qt::RightDockWidgetArea, loadModelWidget);
	splitDockWidget(loadModelWidget, layerWidget, Qt::Vertical);
	//tabifyDockWidget(loadModelWidget, propertyWidget);
	loadModelWidget->raise();


	SelectLayer = 0;
	RefreshLayerWidget();
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
	act->setObjectName(("导出K文件"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("导出K文件")); 
	pannel->addLargeAction(act);
	connect(act, &QAction::triggered, this, &ParaModel::ExportFileAction);

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
//显示选择构件窗口
void ParaModel::ShowUnitSelectWindow()
{
	QWidget* unitSelectWidget = new QWidget();
	unitSelectWidget->setFixedWidth(300);
	unitSelectWidget->setFixedHeight(700);
	QTreeWidget* pTreeWidget = new QTreeWidget(unitSelectWidget);
	pTreeWidget->setFixedWidth(300);
	pTreeWidget->setFixedHeight(700);
	pTreeWidget->setHeaderHidden(true);

	for (vector<BasicUnit>::const_iterator iter = vBaseUnit.begin(); iter != vBaseUnit.end(); iter++)
	{
		if (iter->nUnitType == SelectUnitType)
		{
			QTreeWidgetItem* rootItem = new QTreeWidgetItem(pTreeWidget);
			rootItem->setText(0, iter->oShape.nShapeName);
			rootItem->setData(0, Qt::UserRole, iter->nUnitIdx);
		}
	}
	//树按钮响应
	connect(pTreeWidget, &QTreeWidget::itemDoubleClicked, this, [=](QTreeWidgetItem* item, int column) {
		if (if_data == 0)
		{
			MyLogOutput("当前无画布信息，请新建或者打开后在操作");
			return;
		}

		QVariant variant = item->data(0, Qt::UserRole);
		int nUnitIdx = variant.value<int>();
		QString msg;
		for (vector<BasicUnit>::const_iterator iter = vBaseUnit.begin(); iter != vBaseUnit.end(); iter++)
		{
			if (iter->nUnitIdx != nUnitIdx)
				continue;

			if (iter->oShape.nShapeType == 1)
			{

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
		msg = item->text(0) + "构件更换成功";
		MyLogOutput(msg);

		unitSelectWidget->hide();
		});
	unitSelectWidget->setWindowModality(Qt::ApplicationModal);
	unitSelectWidget->setWindowFlags(Qt::WindowCloseButtonHint);
	unitSelectWidget->setWindowTitle("请选择构件");
	unitSelectWidget->setWindowIcon(QIcon(":/qss/res/qss/White/icon2.png"));
	unitSelectWidget->show();
}

//显示选择构件窗口
void ParaModel::ShowAllUnitSelectWindow()
{
	QWidget* unitSelectWidget = new QWidget();
	unitSelectWidget->setFixedWidth(300);
	unitSelectWidget->setFixedHeight(700);

	QTreeWidget* punitSelectTreeWidget = new QTreeWidget(unitSelectWidget);
	punitSelectTreeWidget->setFixedWidth(300);
	punitSelectTreeWidget->setFixedHeight(700);
	punitSelectTreeWidget->setHeaderHidden(true);
	QTreeWidgetItem* rootItemPillar = new QTreeWidgetItem(punitSelectTreeWidget);
	rootItemPillar->setText(0, "柱");
	QTreeWidgetItem* rootItemBeam = new QTreeWidgetItem(punitSelectTreeWidget);
	rootItemBeam->setText(0, "梁");
	QTreeWidgetItem* rootItemBoard = new QTreeWidgetItem(punitSelectTreeWidget);
	rootItemBoard->setText(0, "板");
	QTreeWidgetItem* rootItemWall = new QTreeWidgetItem(punitSelectTreeWidget);
	rootItemWall->setText(0, "墙");
	QTreeWidgetItem* rootItemDoor = new QTreeWidgetItem(punitSelectTreeWidget);
	rootItemDoor->setText(0, "门");
	QTreeWidgetItem* rootItemWindow = new QTreeWidgetItem(punitSelectTreeWidget);
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
	connect(punitSelectTreeWidget, &QTreeWidget::itemDoubleClicked, this, [=](QTreeWidgetItem* item, int column) {
		if (if_data == 0)
		{
			MyLogOutput("当前无画布信息，请新建或者打开后在操作");
			return;
		}
		QTreeWidgetItem* parent = item->parent();
		if (NULL == parent) //注意：最顶端项是没有父节点的，双击这些项时注意(陷阱)
			return;

		QVariant variant = item->data(0, Qt::UserRole);
		int nUnitIdx1 = variant.value<int>();



		//待添加的构建信息
		BasicUnit oAddUnit = findUnit(nUnitIdx1, vBaseUnit);
		//坐标
		PixelPos pos;
		pos.nXY[0] = moveXY[0] - pSceneOffset, pos.nXY[1] = moveXY[1] - pSceneOffset;

		pCalShapeData->selectUnitIdx = SelectUnitIdx;
		//添加构建入拓扑图
		pCalShapeData->AddBaseUnit(oAddUnit, pos, vBaseUnit, vModelTmpl, viewShape);

		unitSelectWidget->hide();
		});


	unitSelectWidget->setWindowModality(Qt::ApplicationModal);
	unitSelectWidget->setWindowFlags(Qt::WindowCloseButtonHint);
	unitSelectWidget->setWindowTitle("请选择构件");
	unitSelectWidget->setWindowIcon(QIcon(":/qss/res/qss/White/icon2.png"));
	unitSelectWidget->show();
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
/// <summary>
/// 新建场景
/// </summary>
void ParaModel::NewFileAction()
{
	if (if_data == 1)
	{
		QMessageBox::information(NULL, "信息提示", "当前已有加载数据，请关闭后在新建");
		MyLogOutput("当前已有加载数据，请关闭后在新建");
		return;
	}
	if_data = 1;
	//给画布中心绘制十字线， 点击后可以添加
	QPen pen = QPen(Qt::yellow);
	pen.setStyle(Qt::DashLine);

	//初始化水平线
	for (size_t i = 0; i < 2; i++)
	{
		BRectangle* divideLineH = new BRectangle(
			1, 1000,
			8000, 2,
			BGraphicsItem::ItemType::Rectangle);
		divideLineH->isAuxiliary = true;
		divideLineH->nUnitType = 0;
		divideLineH->nUnitIdx = 0;
		divideLineH->setPen(pen);
		connect(divideLineH, &BRectangle::SceneMenuAddClick, this, &ParaModel::SceneMenuAddClickAction);
		if (i == 0)
		{
			pSceneMain.addItem(divideLineH);
		}
		if (i == 1)
		{
			pSceneX.addItem(divideLineH);
		}
	}
	//初始化垂直线
	for (size_t i = 0; i < 2; i++)
	{

		BRectangle* divideLineV = new BRectangle(
			1000, 1,
			2, 8000,
			BGraphicsItem::ItemType::Rectangle);
		divideLineV->isAuxiliary = true;
		divideLineV->nUnitType = 0;
		divideLineV->nUnitIdx = 0;
		divideLineV->setPen(pen);
		connect(divideLineV, &BRectangle::SceneMenuAddClick, this, &ParaModel::SceneMenuAddClickAction);
		if (i == 0)
		{
			pSceneMain.addItem(divideLineV);
		}
		if (i == 1)
		{
			pSceneX.addItem(divideLineV);
		}
	}


	MyLogOutput("新建场景文件成功");
}
/// <summary>
/// 导出k文件
/// </summary>
void ParaModel::ExportFileAction()
{
	//TODO::让用户选择文件夹，在选择的文件夹中导出k文件
	return;
}

/// <summary>
/// 关闭场景
/// </summary>
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

/// <summary>
/// 打开场景
/// </summary>
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
		for (size_t i = 0; i < 12; i++)
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

	pCalShapeData = new DimDataConvert();
	pCalShapeData->CalPlaneData(vModelTmpl, viewShape, vBaseUnit);
	vLoadModelData.push_back(vModelTmpl);
	SelectLayer = 1;
	ParaModel::AddSceneData();
	ParaModel::AddSceneXData();
	ParaModel::RefreshLayerWidget();
}

void ParaModel::GraphicsViewXFocus(bool b)
{
	QString titleStr;
	if (SelectLayer == 0)
	{
		titleStr = QString("当前编辑视图 （二维X）楼层");
	}
	else
	{
		titleStr = QString("当前编辑视图 （二维X）楼层：%1").arg(SelectLayer);
	}
	MainDockWidget->setWindowTitle(titleStr);
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

	QString titleStr;
	if (SelectLayer == 0)
	{
		titleStr = QString("当前编辑视图 （二维Y）楼层");
	}
	else
	{
		titleStr = QString("当前编辑视图 （二维Y）楼层：%1").arg(SelectLayer);
	}
	MainDockWidget->setWindowTitle(titleStr);
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
	QString titleStr;
	if (SelectLayer == 0)
	{
		titleStr = QString("当前编辑视图 （二维Z）楼层");
	}
	else
	{
		titleStr = QString("当前编辑视图 （二维Z）楼层：%1").arg(SelectLayer);
	}
	MainDockWidget->setWindowTitle(titleStr);
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
	QString titleStr;
	if (SelectLayer == 0)
	{
		titleStr = QString("当前编辑视图 （三维模型）楼层");
	}
	else
	{
		titleStr = QString("当前编辑视图 （三维模型）楼层：%1").arg(SelectLayer);
	}
	MainDockWidget->setWindowTitle(titleStr);
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
//刷新楼层窗口
void ParaModel::RefreshLayerWidget()
{
	QWidget* temp = new QWidget();
	QPushButton* pCopyLayerBtn = new QPushButton("复制当前楼层", this);
	pCopyLayerBtn->setIcon(QIcon(":/shaders/res/ToolIcon/run.png"));
	pCopyLayerBtn->setFixedWidth(200);
	connect(pCopyLayerBtn, &QPushButton::clicked, this, &ParaModel::CopyLayerAction);

	QString layerDetail;
	if (SelectLayer == 0)
	{
		layerDetail = QString("已有楼层：%1").arg(vLoadModelData.size());
	}
	else
	{
		layerDetail = QString("已有楼层：%1  当前编辑楼层：%2").arg(vLoadModelData.size()).arg(SelectLayer);
	}
	QLabel* layerlbl = new QLabel(layerDetail);
	layerlbl->setFixedWidth(300);

	QFormLayout* pLayout = new QFormLayout();
	pLayout->addRow(pCopyLayerBtn);
	pLayout->addRow(layerlbl);
	for (size_t i = 0; i < vLoadModelData.size(); i++)
	{

		QString layerStr = QString("%1楼").arg(i + 1);
		QPushButton* pLayerBtn = new QPushButton(layerStr, this);
		pLayerBtn->setToolTip(QString("%1").arg(i));
		pLayerBtn->setIcon(QIcon(":/shaders/res/ToolIcon/run.png"));
		pLayerBtn->setFixedWidth(100);
		connect(pLayerBtn, &QPushButton::clicked, this, [=]()
			{
				ParaModel::ChangeLayerAction(i);
			});

		QPushButton* pDelLayerBtn = new QPushButton("删除", this);
		pDelLayerBtn->setIcon(QIcon(":/shaders/res/ToolIcon/run.png"));
		pDelLayerBtn->setFixedWidth(100);
		connect(pDelLayerBtn, &QPushButton::clicked, this, [=]()
			{
				ParaModel::DeleteLayerAction(i);
			});
		pLayout->addRow(pLayerBtn, pDelLayerBtn);
	}

	temp->setLayout(pLayout);
	layerWidget->setWidget(temp);
}
// 复制楼层数据
void ParaModel::CopyLayerAction()
{
	if (SelectLayer == 0)
	{ 
		MyLogOutput("当前没有选中的楼层请加载数据后在选择");
		return;
	}
	SelectLayer = SelectLayer + 1;
	VTOPOTABLE x = vModelTmpl;
	vLoadModelData.push_back(x);
	ParaModel::RefreshLayerWidget();
	return;
}
// 将界面上的所有数据切换到其他选中楼层
void ParaModel::ChangeLayerAction(int layer)
{
	SelectLayer = layer;
	vModelTmpl = vLoadModelData[layer];
	RefreshSceneData();
	ParaModel::RefreshLayerWidget();
	return;
}
// 删除当前层数据
void ParaModel::DeleteLayerAction(int layer)
{
	vLoadModelData.erase(vLoadModelData.begin() + layer);
	if (vLoadModelData.size() == 0)
	{
		SelectLayer = 0;
		if_data == 0;
		vModelTmpl.clear();
	}
	ParaModel::RefreshLayerWidget();
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
	// 0-6 柱梁板墙门窗
	QString sType = "线柱梁板墙门窗";
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
//根据构件类型查找颜色
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
//根据构件类型id查找对应的中文名
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
//根据构件中文名查找对应的id
int ParaModel::GetUnitTypeCode(QString unitTypeStr)
{
	if (unitTypeStr.compare("柱") == 0)
	{
		return 1;
	}
	else if (unitTypeStr.compare("梁") == 0)
	{
		return 2;
	}
	else if (unitTypeStr.compare("板") == 0)
	{
		return 3;
	}
	else if (unitTypeStr.compare("墙") == 0)
	{
		return 4;
	}
	else if (unitTypeStr.compare("门") == 0)
	{
		return 5;
	}
	else if (unitTypeStr.compare("窗") == 0)
	{
		return 6;
	}
	return 0;
}
//根据形状中文名查找对应的id
int ParaModel::GetShapeTypeCode(QString shapeTypeStr)
{
	if (shapeTypeStr.compare("矩形") == 0)
	{
		return 1;
	}
	else if (shapeTypeStr.compare("圆形") == 0)
	{
		return 2;
	}
	else if (shapeTypeStr.compare("多边形") == 0)
	{
		return 3;
	}
	return 0;
}

//更新画布中单独的元素
QList<QGraphicsItem*> ParaModel::SelectSceneItem(int nUnitIdx)
{
	bool isWall = false;
	QList<QGraphicsItem*> itemList = pSceneMain.items();
	QList<QGraphicsItem*> returnList;
	for (size_t i = 0; i < itemList.size(); i++)
	{
		BGraphicsItem* proxyWidget = qgraphicsitem_cast<BGraphicsItem*>(itemList[i]);
		if (proxyWidget->nUnitIdx == nUnitIdx)
		{
			if (proxyWidget->nOriPos[0] >= 0)
			{
				returnList.append(proxyWidget);
			}
		}
	}
	return returnList;
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

// 刷新画布数据
void ParaModel::RefreshSceneData()
{
	if (if_data == 0)
		return;
	// 计算移动后的新坐标
	pCalShapeData->MoveBaseUnit(SelectUnitIdx, nMoveXY[0], nMoveXY[1], vModelTmpl, viewShape, vBaseUnit);
	nMoveXY[0] = 0;
	nMoveXY[1] = 0;
	// 转为绘图坐标
	pCalShapeData->CalPlaneData(vModelTmpl, viewShape, vBaseUnit);
	AddSceneData();
	AddSceneXData();
	return;
}
//画布移动元素
void ParaModel::SceneItemMoveAction(int nUnitType, int nUnitIdx, QPointF pos)
{
	// 发送绝对位置信息时 按 相对左下角计算位移
// 	nMoveXY[0] = pos.x() - pSceneOffset - (oShape.nCen[0] - oShape.nWH[0] / 2); 
// 	nMoveXY[1] = pos.y() - pSceneOffset - (oShape.nCen[1] - oShape.nWH[1] / 2); 
	// 发送位移时 直接赋值
	//如果构件是水平的修改x
	//如果构件是垂直的修改y
	SelectUnitIdx = nUnitIdx;
	SelectUnitType = nUnitType;
	nMoveXY[0] = pos.x();
	nMoveXY[1] = pos.y();
	QString sInfo = QString("移动%1构建 %2 %3").arg(nUnitIdx).arg(pos.x()).arg(pos.y());
	MyLogOutput(sInfo);
	ParaModel::RefreshSceneData();
	return;
}
//画布菜单点击
void ParaModel::SceneMenuClickAction(int nUnitType, int nUnitIdx)
{
	SelectUnitIdx = nUnitIdx;
	SelectUnitType = nUnitType;
	ShowUnitSelectWindow();
	return;
}
//删除
void ParaModel::SceneMenuDeleteClickAction(int nUnitType, int nUnitIdx)
{
	SelectUnitIdx = nUnitIdx;
	SelectUnitType = nUnitType;

	pCalShapeData->DelBaseUnit(nUnitIdx, vModelTmpl, 0);
	return;
}
//增加
void ParaModel::SceneMenuAddClickAction(int nUnitType, int nUnitIdx)
{
	SelectUnitIdx = nUnitIdx;
	SelectUnitType = nUnitType;

	QList<QGraphicsItem*> x = SelectSceneItem(nUnitIdx);
	BGraphicsItem* proxyWidget = qgraphicsitem_cast<BGraphicsItem*>(x[0]);
	moveXY[0] = proxyWidget->nOriPos[0];
	moveXY[1] = proxyWidget->nOriPos[1];
	ShowAllUnitSelectWindow();
	return;
}
//画布增加数据
void ParaModel::AddSceneData()
{
	if (if_data == 0)
		return;
	if (vModelTmpl.size() == 0)
		return;
	//清除画布
	SceneMainClear();

	//加载标准线
	for (size_t i = 0; i < viewShape.size(); i++)
	{
		//如果当前要渲染的是强
		if (viewShape[i].unitType == 1)
		{
			int coordX = viewShape[i].nCen[0] + pSceneOffset;
			int coordY = viewShape[i].nCen[1] + pSceneOffset;

			QPen pen = QPen(Qt::blue);
			pen.setStyle(Qt::DashLine);
			BRectangle* divideLineH = new BRectangle(
				1, coordY,
				8000, 2,
				BGraphicsItem::ItemType::Rectangle);
			divideLineH->isAuxiliary = true;
			divideLineH->nUnitType = 0;
			divideLineH->nUnitIdx = viewShape[i].unitIdx;
			divideLineH->setPen(pen);
			connect(divideLineH, &BRectangle::SceneMenuAddClick, this, &ParaModel::SceneMenuAddClickAction);
			connect(divideLineH, &BRectangle::SceneItemMove, this, &ParaModel::SceneItemMoveAction);
			pSceneMain.addItem(divideLineH);

			BRectangle* divideLineV = new BRectangle(
				coordX, 1,
				2, 8000,
				BGraphicsItem::ItemType::Rectangle);
			divideLineV->isAuxiliary = true;
			divideLineV->nUnitType = 0;
			divideLineV->nUnitIdx = viewShape[i].unitIdx;
			divideLineV->setPen(pen);
			connect(divideLineV, &BRectangle::SceneMenuAddClick, this, &ParaModel::SceneMenuAddClickAction);
			connect(divideLineV, &BRectangle::SceneItemMove, this, &ParaModel::SceneItemMoveAction);
			pSceneMain.addItem(divideLineV);
		}
		//如果当前要渲染的是强
		if (viewShape[i].unitType == 4)
		{
			int coordX = viewShape[i].nCen[0] + pSceneOffset;
			int coordY = viewShape[i].nCen[1] + pSceneOffset;
			//判断墙的角度加标准线
			if (vModelTmpl[viewShape[i].unitIdx].nUnitAngle == 0)
			{
				BRectangle* divideLine = new BRectangle(
					1, coordY,
					8000, 2,
					BGraphicsItem::ItemType::Rectangle);
				divideLine->isAuxiliary = true;
				divideLine->nUnitType = 0;
				divideLine->nUnitIdx = viewShape[i].unitIdx;
				/*QBrush b = (Qt::DashLine);
				b.setColor(ColorHelper(viewShape[i].unitType));
				divideLine->setBrush(b);*/
				QPen pen = QPen(Qt::yellow);
				pen.setStyle(Qt::DashLine);
				divideLine->setPen(pen);
				connect(divideLine, &BRectangle::SceneMenuAddClick, this, &ParaModel::SceneMenuAddClickAction);
				connect(divideLine, &BRectangle::SceneItemMove, this, &ParaModel::SceneItemMoveAction);
				pSceneMain.addItem(divideLine);
			}
			else
			{
				BRectangle* divideLine = new BRectangle(
					coordX, 1,
					2, 8000,
					BGraphicsItem::ItemType::Rectangle);
				divideLine->isAuxiliary = true;
				divideLine->nUnitType = 0;
				divideLine->nUnitIdx = viewShape[i].unitIdx;
				/*QBrush b = (Qt::DashLine);
				b.setColor(ColorHelper(viewShape[i].unitType));
				divideLine->setBrush(b);*/
				QPen pen = QPen(Qt::yellow);
				pen.setStyle(Qt::DashLine);
				divideLine->setPen(pen);
				connect(divideLine, &BRectangle::SceneMenuAddClick, this, &ParaModel::SceneMenuAddClickAction);
				connect(divideLine, &BRectangle::SceneItemMove, this, &ParaModel::SceneItemMoveAction);
				pSceneMain.addItem(divideLine);
			}

		}
	}
	//根据数据绘制图形
	for (size_t i = 0; i < viewShape.size(); i++)
	{
		if (viewShape[i].unitIdx == 15)
		{
			int k = 0;
		}
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
			connect(viewItem, &BRectangle::SceneMenuAddClick, this, &ParaModel::SceneMenuAddClickAction);
			connect(viewItem, &BRectangle::SceneMenuDeleteClick, this, &ParaModel::SceneMenuDeleteClickAction);
			connect(viewItem, &BRectangle::SceneMenuClick, this, &ParaModel::SceneMenuClickAction);
			pSceneMain.addItem(viewItem);
		}
	}

	if (MainDockState != 3)
	{
		graphicsViewMain->hide();
		graphicsViewMain->show();
	}
}

//画布X增加数据
void ParaModel::AddSceneXData()
{
	if (if_data == 0)
		return;
	if (vModelTmpl.size() == 0)
		return;
	//清除画布
	SceneXClear();

	//加载标准线
	for (size_t i = 0; i < viewShape.size(); i++)
	{
		//如果当前要渲染的是强
		if (viewShape[i].unitType == 4)
		{
			int coordX = viewShape[i].nCen[0] + pSceneOffset;
			int coordY = viewShape[i].nCen[1] + pSceneOffset;
			//判断墙的角度加标准线
			if (vModelTmpl[viewShape[i].unitIdx].nUnitAngle == 0)
			{
				BRectangle* divideLine = new BRectangle(
					1, coordY,
					8000, 2,
					BGraphicsItem::ItemType::Rectangle);
				divideLine->isAuxiliary = true;
				divideLine->nUnitType = 0;
				divideLine->nUnitIdx = viewShape[i].unitIdx;
				/*QBrush b = (Qt::DashLine);
				b.setColor(ColorHelper(viewShape[i].unitType));
				divideLine->setBrush(b);*/
				QPen pen = QPen(Qt::yellow);
				pen.setStyle(Qt::DashLine);
				divideLine->setPen(pen);
				connect(divideLine, &BRectangle::SceneMenuAddClick, this, &ParaModel::SceneMenuAddClickAction);
				connect(divideLine, &BRectangle::SceneItemMove, this, &ParaModel::SceneItemMoveAction);
				pSceneX.addItem(divideLine);
			}
			else
			{
				BRectangle* divideLine = new BRectangle(
					coordX, 1,
					2, 8000,
					BGraphicsItem::ItemType::Rectangle);
				divideLine->isAuxiliary = true;
				divideLine->nUnitType = 0;
				divideLine->nUnitIdx = viewShape[i].unitIdx;
				/*QBrush b = (Qt::DashLine);
				b.setColor(ColorHelper(viewShape[i].unitType));
				divideLine->setBrush(b);*/
				QPen pen = QPen(Qt::yellow);
				pen.setStyle(Qt::DashLine);
				divideLine->setPen(pen);
				connect(divideLine, &BRectangle::SceneMenuAddClick, this, &ParaModel::SceneMenuAddClickAction);
				connect(divideLine, &BRectangle::SceneItemMove, this, &ParaModel::SceneItemMoveAction);
				pSceneX.addItem(divideLine);
			}

		}
	}
	//根据数据绘制图形
	for (size_t i = 0; i < viewShape.size(); i++)
	{
		if (viewShape[i].unitIdx == 15)
		{
			int k = 0;
		}
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
			connect(viewItem, &BRectangle::SceneMenuAddClick, this, &ParaModel::SceneMenuAddClickAction);
			connect(viewItem, &BRectangle::SceneMenuDeleteClick, this, &ParaModel::SceneMenuDeleteClickAction);
			connect(viewItem, &BRectangle::SceneMenuClick, this, &ParaModel::SceneMenuClickAction);
			pSceneX.addItem(viewItem);
		}
	}

	if (MainDockState != 3)
	{
		graphicsViewX->hide();
		graphicsViewX->show();
	}
}

//更新画布中单独的元素
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
				// 				proxyWidget->m_leftup = QPointF(pSceneOffset + x, pSceneOffset + y);
				// 				proxyWidget->m_edge = QPointF(pSceneOffset + x + width, pSceneOffset + y + height);
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

	if (MainDockState != 3)
	{
		graphicsViewMain->hide();
		graphicsViewMain->show();
	}
}
//清除画布数据
void ParaModel::SceneMainClear()
{
	pSceneMain.clear();
	for (int x = 0; x <= 2000; x += 10)
		pSceneMain.addLine(0, x, 2000, x, QPen(Qt::red));
	for (int y = 0; y <= 2000; y += 10)
		pSceneMain.addLine(y, 0, y, 2000, QPen(Qt::red));
}
void ParaModel::SceneXClear()
{
	pSceneX.clear();
	for (int x = 0; x <= 2000; x += 10)
		pSceneX.addLine(0, x, 2000, x, QPen(Qt::red));
	for (int y = 0; y <= 2000; y += 10)
		pSceneX.addLine(y, 0, y, 2000, QPen(Qt::red));
}
void ParaModel::SceneYClear()
{
	pSceneY.clear();
	for (int x = 0; x <= 2000; x += 10)
		pSceneY.addLine(0, x, 2000, x, QPen(Qt::red));
	for (int y = 0; y <= 2000; y += 10)
		pSceneY.addLine(y, 0, y, 2000, QPen(Qt::red));
}
void ParaModel::SceneZClear()
{
	pSceneZ.clear();
	for (int x = 0; x <= 2000; x += 10)
		pSceneZ.addLine(0, x, 2000, x, QPen(Qt::red));
	for (int y = 0; y <= 2000; y += 10)
		pSceneZ.addLine(y, 0, y, 2000, QPen(Qt::red));
}
#pragma endregion
