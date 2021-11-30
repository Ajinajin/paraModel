#include <QTextCodec>

#include "ParaModel.h"
#include <qtreeview.h>
#include<qtreewidget.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsView>
#include <qtreewidget.h>
#include <QDateTime>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>


#include "bqgraphicsitem.h"
#include "bqgraphicsscene.h"

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
		if (iter->oShape.nShapeType == 1)
		{
			childItem->setText(0, "矩形");
		}
		else if (iter->oShape.nShapeType == 2)
		{
			childItem->setText(0, "圆");
		}
		else if (iter->oShape.nShapeType == 3)
		{
			childItem->setText(0, "多边形");
		}
		else if (iter->oShape.nShapeType == 0)
		{
			childItem->setText(0, QString::number(iter->oShape.nThickNess));
		}
	}

	//树按钮响应
	connect(pModelTreeWidget, &QTreeWidget::itemDoubleClicked, this, [=](QTreeWidgetItem* item, int column) {
		QTreeWidgetItem* parent = item->parent();
		if (NULL == parent) //注意：最顶端项是没有父节点的，双击这些项时注意(陷阱)
			return;
		int col = parent->indexOfChild(item); //item在父项中的节点行号(从0开始)
		//1圆 2椭圆 3正方形 4矩形
		char* msg = "";
		if (0 == col) {
			//pSceneMain.clear();

			/*foreach(QGraphicsItem * item, pSceneMain.items())
			{
				pSceneMain.removeItem(item);
			} */

			msg = "圆模型加载完成";
			BCircle* m_circle = new BCircle(1000, 1000, 50, BGraphicsItem::ItemType::Circle);
			pSceneMain.addItem(m_circle);
		}
		else if (1 == col) {
			msg = "椭圆模型加载完成";
			BEllipse* m_ellipse = new BEllipse(1000, 1000, 120, 80, BGraphicsItem::ItemType::Ellipse);
			pSceneMain.addItem(m_ellipse);
		}
		else if (2 == col) {
			msg = "正方形模型加载完成";
			BSquare* m_square = new BSquare(1000, 1000, 60, BGraphicsItem::ItemType::Square);
			pSceneMain.addItem(m_square);
		}
		else if (3 == col) {
			msg = "矩形模型加载完成";
			BRectangle* m_rectangle = new BRectangle(1000, 1000, 240, 120, BGraphicsItem::ItemType::Rectangle);
			m_rectangle->wallwidth = 120;
			pSceneMain.addItem(m_rectangle);
		}
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

	//三维窗口
	oglmanager = new ParaOGLManager;
	graphicsViewOgl->setViewport(oglmanager);
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

	MainDockWidget->setWindowTitle("当前编辑视图 （二维X）");
	/*oglManager = new OGLManager;*/
	MainDockWidget->setWidget(graphicsViewMain);
	MainDockWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);


	//QTimer* timer = new QTimer(this);
	//connect(timer, &QTimer::timeout, this, &ParaModel::updateOGL);
	////每40ms刷新一次OpenGL界面 25FPS
	//timer->start(40);

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

	QDockWidget* loadModelWidget = new QDockWidget(this);
	InitLoadModelWidget(loadModelWidget);

	QDockWidget* propertyWidget = new QDockWidget(this);
	InitPropertyWidget(propertyWidget);

	QDockWidget* logWidget = new QDockWidget(this);
	InitLogWidget(logWidget);

	QDockWidget* oglWidget = new QDockWidget(this);
	InitOglManagerWidget(oglWidget);

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
	connect(act, &QAction::toggled, this, [act](bool b) {
		if (b) {
			act->setIcon(QIcon(":/qss/res/qss/White/success.png"));
		}
		else {
			act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
		}
		});
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

ParaModel::ParaModel(QWidget* parent)
	: SARibbonMainWindow(parent)
{
	//初始化系统数据
	InitBaseUnit();

	//初始化界面
	InitWindow();
	InitTipWindow();
}

void ParaModel::InitBaseUnit()
{
	QString Path = "D:/Works/paramodel/x64/Debug/buildinglib.txt";
	QFile file(Path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(NULL, "信息提示", "文件解析失败！");
		return;
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

#pragma region 形状识别
		if (list[2] == "矩形")
		{
			shape.nShapeType = 1;
			if (list[1] == "门" || list[1] == "窗")
			{
				shape.nShapeRange[0] = list[3].toInt();
				shape.nShapeRange[3] = list[4].toInt();
			}
			else
			{
				shape.nShapeRange[0] = list[3].toInt();
				shape.nShapeRange[1] = list[4].toInt();
				shape.nShapeRange[2] = list[5].toInt();
				shape.nShapeRange[3] = list[6].toInt();
			}
		}
		else if (list[2] == "圆形")
		{
			shape.nShapeType = 2;
			shape.nCen[0] = list[3].toInt();
			shape.nCen[1] = list[4].toInt();
			shape.nNumOrRadius = list[5].toInt();
		}
		else if (list[2] == "多边形")
		{
			shape.nShapeType = 3;
			shape.nNumOrRadius = list[3].toInt();
			for (size_t i = 0; i < shape.nNumOrRadius; i++)
			{
				v.push_back(list[4 + i].toInt());
			}
			shape.vPolyPt = v;
		}
#pragma endregion

#pragma region UnitType识别
		if (list[1] == "柱")
		{
			basic.nUnitType = 1;
		}
		else if (list[1] == "梁")
		{
			basic.nUnitType = 2;
		}
		else if (list[1] == "楼板")
		{
			shape.nThickNess = list[2].toInt();
			basic.nUnitType = 3;
		}
		else if (list[1] == "墙")
		{
			shape.nThickNess = list[2].toInt();
			basic.nUnitType = 4;
		}
		else if (list[1] == "门")
		{
			basic.nUnitType = 5;
		}
		else if (list[1] == "窗")
		{
			basic.nUnitType = 6;
		}
#pragma endregion

		basic.oShape = shape;
		vBaseUnit.push_back(basic);
	}
}
void ParaModel::NewFileAction()
{
	if_data = 1;
}
void ParaModel::CloseFileAction()
{
	if_data = 0;
}
void ParaModel::OpenFileAction()
{
	if (if_data == 1)
	{
		QMessageBox::information(NULL, "信息提示", "当前已有文件，请关闭后在打开");
	}
	if_data = 1;
}
void ParaModel::GraphicsViewXFocus(bool b)
{
	MainDockWidget->setWindowTitle("当前编辑视图 （二维X）");
	MainDockState = 0;
	MainDockWidget->setWidget(graphicsViewMain);
	pSceneMain.setBackgroundBrush(Qt::darkGray);
	return;
}

void ParaModel::GraphicsViewYFocus(bool b)
{
	MainDockWidget->setWindowTitle("当前编辑视图 （二维Y）");
	MainDockState = 1;
	MainDockWidget->setWidget(graphicsViewMain);
	pSceneMain.setBackgroundBrush(Qt::lightGray);

	return;
}

void ParaModel::GraphicsViewZFocus(bool b)
{
	MainDockWidget->setWindowTitle("当前编辑视图 （二维Z）");
	MainDockState = 2;
	MainDockWidget->setWidget(graphicsViewMain);
	pSceneMain.setBackgroundBrush(Qt::gray);
	return;
}

void ParaModel::GraphicsViewOgl(bool b)
{
	MainDockWidget->setWindowTitle("当前编辑视图 （三维模型）");
	oglmanagerMain = new ParaOGLManager();
	MainDockWidget->setWidget(oglmanagerMain);
	MainDockState = 3;

	return;
}


void ParaModel::MyLogOutput(const char* myLogout)
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

// 打开 槽函数
void ParaModel::openAndLoadPic() {
	//选择文件对话框/

	QFileDialog* f = new QFileDialog(this);
	f->setWindowTitle("选择数据文件*.txt");
	f->setNameFilter("*.txt");
	f->setViewMode(QFileDialog::Detail);
	QString filePath;
	if (f->exec() == QDialog::Accepted) {
		filePath = f->selectedFiles()[0];
	}
	// 获取文件内容
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		myLogOutLabel->clear();
		myLogOutLabel->setText("文件打开失败");
		return;
	}
	QTextStream readStream(&file);

	while (!readStream.atEnd()) {
		QString content = readStream.readLine();
		QStringList list = content.split(' ');
		if (list[0] == "wall") {
			std::vector<float> points;
			bool ok;
			for (int i = 1; i < list.size(); i++) {
				points.push_back(list[i].toFloat(&ok));
			}
			drawWall(points);
		}
	}
}

void ParaModel::drawWall(const std::vector<float>& points) {
	MyItem* item = new MyItem;
	item->points = points;

	item->setPos(0, 0);
	item->setColor(QColor(Qt::red));

	pSceneMain.addItem(item);
	//pSceneMain->setBackgroundBrush(Qt::gray);
	graphicsViewX->setSceneRect(-100, -100, 670, 370);
	graphicsViewX->setScene(&pSceneMain);
}

// 图像缩放
void ParaModel::getPara() {
	//// 获取文本输入框内容
	//QString strText = scalemyLogOutLabel->toPlainText();
	//float width = 0, height = 0;
	//bool ok = false;
	//// 分割并转成float, 从而获得宽 高
	//QStringList sList = strText.split(",");
	//if (sList.size() != 2) {
	//    myLogOutLabel->clear();
	//    myLogOutLabel->setText("缩放失败，参数错误");
	//    return;
	//}

	//width = sList[0].toFloat(&ok);
	//if (!ok) {
	//    myLogOutLabel->clear();
	//    myLogOutLabel->setText("缩放失败，参数错误");
	//    return;
	//}

	//height = sList[1].toFloat(&ok);
	//if (!ok) {
	//    myLogOutLabel->clear();
	//    myLogOutLabel->setText("缩放失败，参数错误");
	//    return;
	//}

	//if (height <= 0 || width <= 0) {
	//    myLogOutLabel->clear();
	//    myLogOutLabel->setText("缩放失败，参数错误");
	//    return;
	//}
	//// 如果mainLabel尚未载入图像，即img为空，则不能进行缩放
	////mainlabel->clear();
	//if (img == nullptr) {
	//    myLogOutLabel->clear();
	//    myLogOutLabel->setText("缩放失败，图像打开错误");
	//    return;
	//}
	//// 获取原图像的大小，乘以缩放系数，就是新的图像大小
	//int nowWidth = mainlabel->pixmap()->toImage().width();
	//int nowHeight = mainlabel->pixmap()->toImage().height();
	//QSize picSize(int(width * nowWidth), int(height * nowHeight));

	//QImage timage = mainlabel->pixmap()->toImage().scaled(picSize);
	//mainlabel->setScaledContents(false); // 取消自适应
	//// 把新图像添加到mainLabel上
	//QPixmap pmap = QPixmap::fromImage(timage);
	//QPixmap tpmap = pmap.scaled(picSize, Qt::KeepAspectRatio);
	//mainlabel->setPixmap(tpmap);

	//myLogOutLabel->clear();
	//myLogOutLabel->setText("缩放成功");
}

// 取消缩放,主要逻辑就是保存缩放前的图像，重新载入即可
void ParaModel::resetPara() {
	//mainlabel->clear();
	//mainlabel->setScaledContents(true); // 自适应
	//mainlabel->setPixmap(QPixmap::fromImage(*img));
	//myLogOutLabel->clear();
	//myLogOutLabel->setText("取消成功");
}

// 加载图像列表
void ParaModel::loadViews() {

	//定义文件对话框类
	QFileDialog* fileDialog = new QFileDialog(this);
	//定义文件对话框标题
	fileDialog->setWindowTitle(tr("打开图片"));
	//设置默认文件路径
	fileDialog->setDirectory(".");
	//设置文件过滤器
	fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
	//设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	//设置视图模式
	fileDialog->setViewMode(QFileDialog::Detail);
	//打印所有选择的文件的路径
	if (fileDialog->exec()) {
		QStringList fileNames = fileDialog->selectedFiles();
		showImageList(fileNames);
	}
}

// 显示图像列表
void ParaModel::showImageList(QStringList& fileNames) {
	if (fileNames.size() == 0) {
		myLogOutLabel->clear();
		myLogOutLabel->setText("文件列表空");
		return;
	}
	// 遍历所有图像路径并添加
	for (auto tmp : fileNames) {
		QPixmap pixmap(tmp);
		//定义QListWidgetItem对象
		QListWidgetItem* imageItem = new QListWidgetItem;
		// 定义 QWidget 和 QListWidgetItem 一起添加到 QListWidget
		QWidget* widget = new QWidget;
		// 设置 widget 布局方式为 box
		QVBoxLayout* widgetLayout = new QVBoxLayout;
		// 图像和图像名的label
		QLabel* imageLabel = new QLabel;
		QLabel* txtLabel = new QLabel(tr("Browse"));
		// 设置widget的布局
		widget->setLayout(widgetLayout);
		widgetLayout->setMargin(0);
		widgetLayout->setSpacing(0);
		widgetLayout->addWidget(imageLabel);
		widgetLayout->addWidget(txtLabel);

		// 把图像和图像名添加到对应的label
		pixmap = pixmap.scaled(200, 250, Qt::KeepAspectRatio);
		// 修改label的信息
		imageLabel->setPixmap(pixmap);
		txtLabel->setFixedHeight(20);
		txtLabel->setWordWrap(true);
		imageItem->setSizeHint(QSize(200, 250));
		// 把 imageItem, widget添加到 imageList
		//imageList->addItem(imageItem);
		//imageList->setItemWidget(imageItem, widget);
	}
}

// 右击替换
void ParaModel::replaceSlot() {
	//// 所有选中的item，这里只有一个
	//QList<QListWidgetItem*> items = imageList->selectedItems(); 
	//if (items.count() != 1) { // 单选
	//    return;
	//}
	//QListWidgetItem* item = items[0];

	//// 通过 QListWidgetItem 找到 QWidget 方便获取子元素    
	//QWidget* pw = imageList->itemWidget(item); 
	//if (pw == nullptr) {
	//    return;
	//}
	//// 默认获取第一张
	//QLabel* imageLabel = pw->findChild<QLabel*>(); 
	//// 获取图像
	//QImage img = imageLabel->pixmap()->toImage(); 

	//// 更新
	//mainlabel->clear();
	//mainlabel->setScaledContents(true); // 自适应
	//mainlabel->setPixmap(QPixmap::fromImage(img));
	//myLogOutLabel->clear();
	//myLogOutLabel->setText("替换成功");
}

// 自定义右键菜单动作
void ParaModel::onCustomContextMenuRequested(const QPoint& pos) {
	/*弹出右键菜单*/
	popMenu_In_ListWidget_->exec(QCursor::pos());
}

void ParaModel::on_polygonBtn_clicked()
{
	pSceneMain.startCreate();
	//setBtnEnabled(false);
	BPolygon* m_polygon = new BPolygon(BGraphicsItem::ItemType::Polygon);
	pSceneMain.addItem(m_polygon);

	connect(&pSceneMain, SIGNAL(updatePoint(QPointF, QList<QPointF>, bool)), m_polygon, SLOT(pushPoint(QPointF, QList<QPointF>, bool)));
	connect(&pSceneMain, &BQGraphicsScene::createFinished, [=]() {
		//setBtnEnabled(true);
		});
}

void ParaModel::on_clearBtn_clicked()
{
	pSceneMain.clear();
}

//存储view画面

void ParaModel::my_save()
{
	//QFile file("D:\\csol\\untitled4\\DataFile\\test.txt");
	//if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
	//{
	//	QMessageBox::warning(this, "sdf", "can't open", QMessageBox::Yes);
	//}
	//QTextStream stream(&file);
	//QList<QGraphicsItem*> items = graphicsView->scene()->items();
	//foreach(QGraphicsItem * item, items) {
	//	BGraphicsItem* i = dynamic_cast<BGraphicsItem*>(item);
	//	if (!i) continue;
	//	stream << i->m_type << " ";
	//	stream << int(i->m_center.rx()) << " " << int(i->m_center.ry()) << " " << int(i->m_edge.rx()) << " " << int(i->m_edge.ry()) << endl;
	//	//
	//}
	//graphicsView_2->setScene(&pSceneMain);
	//graphicsView_3->setScene(&pSceneMain);
	//file.close();
}

void ParaModel::on_loadBtn_clicked()
{

}


//ParaModel::~ParaModel()
//{
//	delete ui;
//}

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
// 初始化基本构件库 
int ParaModel::InitUnitLib()
{
	QString sUnitFile("d:/基本构件库");
	QFile fileScn(sUnitFile);
	if (!fileScn.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return 1;
	}
	QTextStream inScn(&fileScn);

	QTextCodec* pCode = QTextCodec::codecForName("UTF-8");

	// 按行读入文件内容
	QString sScnLine;
	QStringList listWord;
	BasicUnit oUnit;

	vBaseUnit.clear();
	while (!fileScn.atEnd())
	{
		sScnLine = pCode->toUnicode(fileScn.readLine());
		listWord = sScnLine.split(" ");
		InitUnitPara(listWord, oUnit);
		vBaseUnit.push_back(oUnit);
	}
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	fileScn.close();

	return 0;
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

