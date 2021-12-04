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

#pragma region ��ʼ������
//��ʼ�����Դ���
void ParaModel::InitPropertyWidget(QDockWidget* from)
{

	from->setMaximumWidth(300);
	from->setMaximumHeight(390);
	from->setWindowTitle("���Դ���");
	// ��ǰѡ�񵯵�����
	QLabel* lbl1 = new QLabel("����");
	QLabel* lblvalue = new QLabel("ֵ");
	QLabel* lbl2 = new QLabel("λ��XYZ");
	pModelEdit[0] = new QLineEdit;
	pModelEdit[1] = new QLineEdit;
	pModelEdit[2] = new QLineEdit;
	pModelEdit[0]->setText("0");
	pModelEdit[1]->setText("0");
	pModelEdit[2]->setText("0");
	QLabel* lbl3 = new QLabel("�ٶ�");
	pModelEdit[3] = new QLineEdit;
	pModelEdit[3]->setText("1000");
	QLabel* lbl4 = new QLabel("����XYZ");
	pModelEdit[4] = new QLineEdit;
	pModelEdit[5] = new QLineEdit;
	pModelEdit[6] = new QLineEdit;
	pModelEdit[4]->setText("0");
	pModelEdit[5]->setText("-1");
	pModelEdit[6]->setText("0");

	// ��ǰѡ�񵯵�ս����
	QLabel* lbl5 = new QLabel("����Kg");
	pArmHeadEdit[0] = new QLineEdit;
	pArmHeadEdit[0]->setText("100");
	QLabel* lbl6 = new QLabel("��Ƭ����");
	pArmHeadEdit[1] = new QLineEdit;
	pArmHeadEdit[1]->setText("1000");
	QLabel* lbl7 = new QLabel(QWidget::tr("��Ƭ����g"));
	pArmHeadEdit[2] = new QLineEdit;
	pArmHeadEdit[2]->setText("20");
	QLabel* lbl8 = new QLabel(QWidget::tr("�ֲ���"));
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
	//ͨ���������������һ��
	//QLabel* lbl9 = new QLabel(QWidget::tr(""));
	//lbl9->hide();
	//myVBoxLayout->setAlignment(Qt::AlignTop);
	// 	mygridLayout->setRowStretch(1, 1);
	// 	mygridLayout->setRowStretch(5, 10);
	//myVBoxLayout->setMargin(15);
	//��С���
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


	QPushButton* pApplyBtn = new QPushButton("Ӧ��", this);
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

//��ʼ��ϵͳģ�ʹ���
void ParaModel::InitSysWidget(QDockWidget* from)
{
	//��ȡ�ļ����µ�ģ���ļ� ��ģ���ļ����Ʊ�����ṹ��ŵ� pModelTreeWidget ��
	//˫��ʱ����ģ���ļ����ƶ�Ӧ���ļ�
	QWidget* mytreewidget = new QWidget();
	pModelTreeWidget = new QTreeWidget(mytreewidget);
	pModelTreeWidget->setHeaderHidden(true);


	QTreeWidgetItem* rootItemPillar = new QTreeWidgetItem(pModelTreeWidget);
	rootItemPillar->setText(0, "��");
	QTreeWidgetItem* rootItemBeam = new QTreeWidgetItem(pModelTreeWidget);
	rootItemBeam->setText(0, "��");
	QTreeWidgetItem* rootItemBoard = new QTreeWidgetItem(pModelTreeWidget);
	rootItemBoard->setText(0, "��");
	QTreeWidgetItem* rootItemWall = new QTreeWidgetItem(pModelTreeWidget);
	rootItemWall->setText(0, "ǽ");
	QTreeWidgetItem* rootItemDoor = new QTreeWidgetItem(pModelTreeWidget);
	rootItemDoor->setText(0, "��");
	QTreeWidgetItem* rootItemWindow = new QTreeWidgetItem(pModelTreeWidget);
	rootItemWindow->setText(0, "��");

	for (vector<BasicUnit>::const_iterator iter = vBaseUnit.begin(); iter != vBaseUnit.end(); iter++)
	{

		QTreeWidgetItem* childItem;
#pragma region nUnitType��ֵ
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
		//���� 1 Բ 2 �����3

		childItem->setText(0, iter->oShape.nShapeName);

		childItem->setData(0, Qt::UserRole, iter->nUnitIdx);
	}

	//����ť��Ӧ
	connect(pModelTreeWidget, &QTreeWidget::itemDoubleClicked, this, [=](QTreeWidgetItem* item, int column) {
		if (if_data == 0)
		{
			MyLogOutput("��ǰ�޻�����Ϣ�����½����ߴ򿪺��ڲ���");
			return;
		}

		QTreeWidgetItem* parent = item->parent();
		if (NULL == parent) //ע�⣺�������û�и��ڵ�ģ�˫����Щ��ʱע��(����)
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

				//��ǽ�Ŵ� ���Ϊ1
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
		msg = item->text(0) + "�����������";
		MyLogOutput(msg);
		});

	from->setWidget(mytreewidget);
	from->setFixedWidth(300);
	from->setWindowTitle("ģ�����/�����б�");
	from->setWidget(pModelTreeWidget);
	pModelTreeWidget->expandAll();
}

//��ʼ���Ѽ��ص�ģ�ʹ���
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


	from->setWindowTitle("����ͼ+��ά");
	//layout����

	QGridLayout* myLayout = new QGridLayout();
	QWidget* temp = new QWidget();


	//myHBoxLayout->addWidget(graphicsViewX);
	myLayout->addWidget(graphicsViewX, 0, 0);
	myLayout->addWidget(graphicsViewY, 0, 1);
	myLayout->addWidget(graphicsViewZ, 1, 0);


	//���½�С��ά����

	paraOglmanager = new ParaOGLManager();
	graphicsViewOgl->setViewport(paraOglmanager);
	myLayout->addWidget(graphicsViewOgl, 1, 1);



	temp->setLayout(myLayout);
	from->setWidget(temp);

}

//��ʼ����־����
void ParaModel::InitLogWidget(QDockWidget* from)
{
	from->setMaximumHeight(100);
	from->setFixedHeight(100);
	from->setWindowTitle("��Ϣ���");
	if (myLogOutLabel == nullptr)
		myLogOutLabel = new QTextEdit();
	from->setWidget(myLogOutLabel);
}

//��ʼ��OpenGL����
void ParaModel::InitOglManagerWidget(QDockWidget* from)
{
	MainDockWidget = from;

	graphicsViewMain = new BQGraphicsView();

	graphicsViewMain->setScene(&pSceneMain);
	pSceneMain.setBackgroundBrush(Qt::darkGray);

	MainDockWidget->setWindowTitle("��ǰ�༭��ͼ ����ά��");


	//�м������λ����

	MainDockState = 3;
	paraOglmanagerMain = new ParaOGLManager();
	MainDockWidget->setWidget(paraOglmanagerMain);
	MainDockWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);


	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &ParaModel::updateOGL);
	//ÿ20msˢ��һ��OpenGL���� 50FPS
	timer->start(20);



	//QTimer* timerSceneMain = new QTimer(this);
	//connect(timerSceneMain, &QTimer::timeout, this, &ParaModel::updateScene);
	//timerSceneMain->start(20);
}

//����OpenGL����
void ParaModel::updateOGL()
{
	paraOglmanager->update();
	paraOglmanagerMain->update();


	//������ά����Ҫͬ��
	paraOglmanager->camera = paraOglmanagerMain->camera;
	paraOglmanager->isFirstMouse = paraOglmanagerMain->isFirstMouse;
	paraOglmanager->lastX = paraOglmanagerMain->lastX;
	paraOglmanager->lastY = paraOglmanagerMain->lastY;
	paraOglmanager->rotateRaw = paraOglmanagerMain->rotateRaw;
	paraOglmanager->rotatePitch = paraOglmanagerMain->rotatePitch;

	//�������еĽ�������

	paraOglmanager->oglTopTable = this->vModelTmpl;
	paraOglmanager->oglUnitTable = this->vBaseUnit;

	paraOglmanagerMain->oglTopTable = this->vModelTmpl;
	paraOglmanagerMain->oglUnitTable = this->vBaseUnit;



}

//��ʼ����������
void ParaModel::InitCentralWidget()
{
	//ɾ�����봰��
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

//��ʼ��״̬��������ʾ
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


//��ʼ��Ribbon�е��ļ��˵�
void ParaModel::InitCategoryMain(SARibbonCategory* page)
{
	SARibbonPannel* pannel = page->addPannel(("�ļ�"));

	QAction* act = new QAction(this);
	act->setObjectName(("�½�"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("�½�"));
	act->setShortcut(QKeySequence(QLatin1String("Ctrl+N")));
	act->setCheckable(true);
	pannel->addLargeAction(act);
	connect(act, &QAction::triggered, this, &ParaModel::NewFileAction);

	act = new QAction(this);
	act->setObjectName(("��"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("��"));
	act->setShortcut(QKeySequence(QLatin1String("Ctrl+O")));
	pannel->addLargeAction(act);
	connect(act, &QAction::triggered, this, &ParaModel::OpenFileAction);

	act = new QAction(this);
	act->setObjectName(("����"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("����"));
	act->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel->addLargeAction(act);
	//connect(act, &QAction::triggered, this, &ParaModel::SaveFileAction);


	act = new QAction(this);
	act->setObjectName(("���Ϊ"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("���Ϊ"));
	pannel->addLargeAction(act);
	//connect(act, &QAction::triggered, this, &ParaModel::SaveasFileAction);


	act = new QAction(this);
	act->setObjectName(("�ر�"));
	act->setIcon(QIcon(":/qss/res/qss/White/save.png"));
	act->setText(("�ر�"));
	pannel->addLargeAction(act);
	connect(act, &QAction::triggered, this, &ParaModel::CloseFileAction);
}
//��ʼ��Ribbon
void ParaModel::InitSARibbon()
{
	SARibbonBar* ribbon = ribbonBar();

	//Ӧ�ð�ť ��ǰ����
	ribbon->applicationButton()->hide();
	ribbon->applicationButton()->setText(("��ҳ"));

	//��ʼ���ļ�ribbon
	SARibbonCategory* categoryMain = new SARibbonCategory();
	categoryMain->setCategoryName(("���˵�"));
	categoryMain->setObjectName(("categoryMain"));
	ribbon->addCategoryPage(categoryMain);
	InitCategoryMain(categoryMain);

	//��ʼ��������ݲ˵�
	//InitQuickAccess(ribbon);
}

//��ʼ������
void ParaModel::InitWindow()
{
	setWindowTitle(("������ģ��"));
	//���ý���Ribbon
	InitSARibbon();
	ribbonBar()->setRibbonStyle(SARibbonBar::RibbonStyle::OfficeStyleTwoRow);

	//��ʼ����������
	InitCentralWidget();

	//��ʼ������״̬��
	InitStatusWidget();
	//������С���
	setMinimumWidth(1000);
	//�������
	showMaximized();
	//����ͼ��
	setWindowIcon(QIcon(":/qss/res/qss/White/icon2.png"));
	//��������϶�
	//SAFramelessHelper* helper = framelessHelper();
	//helper->setRubberBandOnResize(false);
	//QElapsedTimer cost;
	//int lastTimes = 0;
	//cost.start();

}

//��ʼ����������
void ParaModel::InitTipWindow()
{
}
//���������˵�����
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
	//����ť��Ӧ
	connect(pTreeWidget, &QTreeWidget::itemDoubleClicked, this, [=](QTreeWidgetItem* item, int column) {
		if (if_data == 0)
		{
			MyLogOutput("��ǰ�޻�����Ϣ�����½����ߴ򿪺��ڲ���");
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

				//��ǽ�Ŵ� ���Ϊ1
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
		msg = item->text(0) + "�����������";
		MyLogOutput(msg);
		});
	unitSelectWidget->setWindowModality(Qt::ApplicationModal);
	unitSelectWidget->setWindowFlags(Qt::WindowCloseButtonHint);
	unitSelectWidget->setWindowTitle("��ѡ�񹹼�");
	unitSelectWidget->setWindowIcon(QIcon(":/qss/res/qss/White/icon2.png"));
	unitSelectWidget->show();
}
#pragma endregion

ParaModel::ParaModel(QWidget* parent)
	: SARibbonMainWindow(parent)
{
	myLogOutLabel = new QTextEdit();
	pSceneOffset = 700;
	//��ʼ��ϵͳ·��
	InitPath();
	//��ʼ��ϵͳ����
	InitUnitLib();

	//��ʼ������
	InitWindow();
	InitTipWindow();
}

#pragma region ���潻��
void ParaModel::NewFileAction()
{
	if (if_data == 1)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "��ǰ���м������ݣ���رպ����½�");
		MyLogOutput("��ǰ���м������ݣ���رպ����½�");
		return;
	}
	if_data = 1;
	MyLogOutput("�½������ļ��ɹ�");
}

void ParaModel::CloseFileAction()
{
	if_data = 0;
	vModelTmpl.clear();
	SceneMainClear();
	SceneXClear();
	SceneYClear();
	SceneZClear();

	MyLogOutput("������ݳɹ�");
}

void ParaModel::OpenFileAction()
{
	if (if_data == 1)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "��ǰ���м������ݣ���رպ��ڴ�");
		MyLogOutput("��ǰ���м������ݣ���رպ��ڴ�");
		return;
	}

	QFileDialog* f = new QFileDialog(this);
	f->setWindowTitle("ѡ�������ļ�*.txt");
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
	// ��ȡ�ļ�����
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "���ݼ���ʧ��,�����ļ���");
		MyLogOutput("���ݼ���ʧ��,�����ļ���");
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

	ParaModel::updateScene();
}

void ParaModel::GraphicsViewXFocus(bool b)
{
	MainDockWidget->setWindowTitle("��ǰ�༭��ͼ ����άX��");
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
	MainDockWidget->setWindowTitle("��ǰ�༭��ͼ ����άY��");
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
	MainDockWidget->setWindowTitle("��ǰ�༭��ͼ ����άZ��");
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
	MainDockWidget->setWindowTitle("��ǰ�༭��ͼ ����άģ�ͣ�");
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

// �����޸ĺ����
void ParaModel::ApplyDataAction()
{
	// ����δ û������
	// if (pEnvir2 == NULL)
	// {
	// 	QMessageBox::information(NULL, "��Ϣ��ʾ", "��δ���س������ݣ��޷����棡");
	// 	return;
	// }


	//��֯���������������
	//λ��
	bool verify;
	QString str = pModelEdit[0]->text();
	float  locationX = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "λ��X��������ֵ������������");
	}
	if (locationX > 180 || locationX < -180)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "λ��X��Χ����������180");
	}
	str = pModelEdit[1]->text();
	float locationY = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "λ��Y��������ֵ������������");
	}
	if (locationY > 180 || locationY < -180)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "λ��Y��Χ����������180");
	}
	str = pModelEdit[2]->text();
	float locationZ = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "λ��Z��������ֵ������������");
	}
	if (locationZ > 180 || locationZ < -180)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "λ��Z��Χ����������180");
	}
	//�ٶ� 
	str = pModelEdit[3]->text();
	float speed = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "�ٶȱ�������ֵ������������");
	}
	//����XYZ
	str = pModelEdit[4]->text();
	float angleToX = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "����X��������ֵ������������");
	}
	if (angleToX > 180 || angleToX < -180)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "����X��Χ����������180");
	}
	str = pModelEdit[5]->text();
	float angleToY = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "����Y��������ֵ������������");
	}
	if (angleToY > 180 || angleToY < -180)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "����Y��Χ����������180");
	}
	str = pModelEdit[6]->text();
	float angleToZ = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "����Z��������ֵ������������");
	}
	if (angleToZ > 180 || angleToZ < -180)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "����Z��Χ����������180");
	}
	//����Kg	  
	str = pArmHeadEdit[0]->text();
	float tnt = str.toFloat(&verify);

	if (!verify)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "TNT���������������ֵ������������");
	}
	//��Ƭ����	  
	str = pArmHeadEdit[1]->text();
	int fragNum = str.toInt(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "λ��X�����������ֵ������������");
	}
	//��Ƭ����g 
	str = pArmHeadEdit[2]->text();
	float fragQuality = str.toFloat(&verify);
	if (!verify)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "��Ƭ������������ֵ������������");
	}
	// �ֲ��� ����ɢ��  

	str = pArmHeadEdit[3]->text();
	float majorScatteringAngle = str.toFloat(&verify);

	if (!verify)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "����ɢ�Ǳ�������ֵ������������");
	}
	if (majorScatteringAngle > 90 || majorScatteringAngle < -90)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "����ɢ�Ƿ�Χ����������90");
	}
	// �ֲ��� ����ɢ��  
	str = pArmHeadEdit[4]->text();
	float secondScatteringAngle = str.toFloat(&verify);

	if (!verify)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "����ɢ�Ǳ�������ֵ������������");
	}
	if (secondScatteringAngle > 90 || secondScatteringAngle < -90)
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "����ɢ�Ƿ�Χ����������90");
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

#pragma region ��ʼ������



int InitUnitPara(QStringList listInfo, BasicUnit& oUnit)
{
	// 0-6 ������ǽ�Ŵ�
	QString sType = "��������ǽ�Ŵ�";
	oUnit.nUnitIdx = listInfo[0].toInt();
	int nType = sType.indexOf(listInfo[1]);
	if (nType < 0)
		return 1;
	oUnit.nUnitType = nType;
	// ����������   ������״ 0-2 ���� Բ�� �����
	QString sShapeType = "����Բ�ζ����";
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
	// ǽ�Ͱ�����
	if (nType == 2 || nType == 3)
		oUnit.oShape.nThickNess = listInfo[2].toInt();
	// �źʹ�
	else
	{
		oUnit.oShape.nShapeType = 0;
		oUnit.oShape.vPolyPt.push_back(listInfo[3].toInt());
		oUnit.oShape.vPolyPt.push_back(listInfo[4].toInt());
	}
	return 0;
}
// ��ʼ��·�� 
int ParaModel::InitPath()
{
	// Exe�ļ�����·��
	oPath.sExePath = qApp->applicationFilePath().toLocal8Bit().data();
	// Exe�ļ�����Ŀ¼
	oPath.sExeDir = qApp->applicationDirPath().toLocal8Bit().data();

	oPath.sBoomLibDir = oPath.sExeDir + "/BoomLib";
	oPath.sModelLibDir = oPath.sExeDir + "/ModelLib";
	oPath.sProcLibDir = oPath.sExeDir + "/ProcLib";
	oPath.sTmpDir = oPath.sExeDir + "/Tmp";
	oPath.sParaLibDir = oPath.sExeDir + "/ParaLib";

	QString sLog = "���ϵͳ·����ʼ��";
	MyLogOutput(sLog);

	return 1;
}
// ��ʼ������������ 
int ParaModel::InitUnitLib()
{
	QString Path = QString::fromStdString(oPath.sExeDir + "/buildinglib.txt");
	QFile file(Path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "ϵͳ�������������ʧ�ܣ�");
		MyLogOutput("ϵͳ�������������ʧ�ܣ�");
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

#pragma region UnitTypeʶ��
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

#pragma region ��״ʶ��

		shape.nShapeType = GetShapeTypeCode(list[2]);
		if (shape.nShapeType == 1)
		{
			int unitType = GetUnitTypeCode(list[1]);
			if (unitType > 4) //������źʹ������
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
// ��ʼ��ƽ��ͼ��
int ParaModel::InitPlaneDrawLib()
{
	return 0;
}
// ��ʼ������������ģ��
int ParaModel::InitParaTmpl()
{
	return 0;
}
#pragma endregion

#pragma region ����ʶ���������

//��ȡϵͳ�������еĹ���
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

//��ȡ���ع������еĹ���
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
//���ݹ������Ͳ�����ɫ
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
//���ݹ�������id���Ҷ�Ӧ��������
QString ParaModel::GetUnitType(int nUnitType)
{
	if (nUnitType == 1)
	{
		return "��";
	}
	else if (nUnitType == 2)
	{
		return "��";
	}
	else if (nUnitType == 3)
	{
		return "��";
	}
	else if (nUnitType == 4)
	{
		return  "ǽ";
	}
	else if (nUnitType == 5)
	{
		return "��";
	}
	else if (nUnitType == 6)
	{
		return "��";
	}
	return "";
}
//���ݹ������������Ҷ�Ӧ��id
int ParaModel::GetUnitTypeCode(QString unitTypeStr)
{
	if (unitTypeStr.compare("��") == 0)
	{
		return 1;
	}
	else if (unitTypeStr.compare("��") == 0)
	{
		return 2;
	}
	else if (unitTypeStr.compare("��") == 0)
	{
		return 3;
	}
	else if (unitTypeStr.compare("ǽ") == 0)
	{
		return 4;
	}
	else if (unitTypeStr.compare("��") == 0)
	{
		return 5;
	}
	else if (unitTypeStr.compare("��") == 0)
	{
		return 6;
	}
	return 0;
}
//������״���������Ҷ�Ӧ��id
int ParaModel::GetShapeTypeCode(QString shapeTypeStr)
{
	if (shapeTypeStr.compare("����") == 0)
	{
		return 1;
	}
	else if (shapeTypeStr.compare("Բ��") == 0)
	{
		return 2;
	}
	else if (shapeTypeStr.compare("�����") == 0)
	{
		return 3;
	}
	return 0;
}
//查找画布中的元素
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
			returnList.append(proxyWidget);
		}
	}
	return returnList;
}
#pragma endregion

#pragma region �ͷ���Դ

ParaModel::~ParaModel()
{
	ReleaseUISource();
	ReleaseSysModel();
}

// �ͷŸ��ֽ�����Դ
void ParaModel::ReleaseUISource()
{
	// �ͷŸ�����Դ
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


// �ͷ�ϵͳģ�Ϳ�
void ParaModel::ReleaseSysModel()
{

	return;
}
#pragma endregion

#pragma region ��������

//�����ƶ�Ԫ��
void ParaModel::SceneItemMoveAction(int nUnitType, int nUnitIdx, QPointF pos)

{
	int a = nUnitType;
	int v = nUnitIdx;
	SimpleShape oShape = viewShape[nUnitIdx]; 
	int nMoveXY[2]; 
	// 发送绝对位置信息时 按 相对左下角计算位移
// 	nMoveXY[0] = pos.x() - pSceneOffset - (oShape.nCen[0] - oShape.nWH[0] / 2); 
// 	nMoveXY[1] = pos.y() - pSceneOffset - (oShape.nCen[1] - oShape.nWH[1] / 2); 
	// 发送位移时 直接赋值
	nMoveXY[0] = pos.x() ; 
	nMoveXY[1] = pos.y() ; 

	// 计算移动后的新坐标
	pCalShapeData->MoveBaseUnit(nUnitIdx, nMoveXY, vModelTmpl, viewShape); 
	
	// 转为绘图坐标
	pCalShapeData->CalPlaneData(vModelTmpl, viewShape, vBaseUnit);

	for (size_t i = 0; i < viewShape.size(); i++)
	{
// 		if (viewShape[i].unitIdx != nUnitIdx)
// 			continue;
		//绘制柱、墙、门、窗
		if (viewShape[i].unitType == 1 || viewShape[i].unitType == 4 || viewShape[i].unitType == 5 || viewShape[i].unitType == 6)
		{
			//在画布中重新找到该元素
			QList<QGraphicsItem*> viewItem = SelectSceneItem(viewShape[i].unitIdx);
			for (size_t i = 0; i < viewItem.size(); i++)
			{
				BGraphicsItem* proxyWidget = qgraphicsitem_cast<BGraphicsItem*>(viewItem[i]);
				if (!proxyWidget->isAuxiliary)
				{
					int coordX = viewShape[i].nCen[0] + pSceneOffset;
					int coordY = viewShape[i].nCen[1] + pSceneOffset;

					proxyWidget->m_leftup =  QPointF(coordX, coordY );
					proxyWidget->m_edge = QPointF(viewShape[i].nWH[0], viewShape[i].nWH[1]);
				}
				else
				{
					//删除标准线
				}
			}
		}
	}

	int nCen[2]; 
	nCen[0] = nMoveXY[0] + vModelTmpl[nUnitIdx].nCenPos[0];
	nCen[1] = nMoveXY[1] + vModelTmpl[nUnitIdx].nCenPos[2];
	QString sInfo = QString("%1 %2").arg(pos.x()).arg(pos.y()); 
	myLogOutLabel->setText(sInfo); 
// 	QString sInfo1 = QString("%1 %2").arg(nCen[0]).arg(nCen[1]); 
// 	myLogOutLabel->setText(sInfo1); 

{ 
	if (MainDockState != 3)
	{
		graphicsViewMain->hide();
		graphicsViewMain->show();
	}

	return;
}
//�����˵����
void ParaModel::SceneMenuClickAction(int nUnitType, int nUnitIdx)
{
	SelectUnitIdx = nUnitIdx;
	SelectUnitType = nUnitType;
	ShowUnitSelectWindow();
	//����x�˵����ڵ�������ѡ�еĹ���������idxֵ
	return;
}
//���»�������
void ParaModel::AddSceneData()
{
	if (if_data == 0)
		return;
	if (vModelTmpl.size() == 0)
		return;

	SceneMainClear();
	DimDataConvert* d = new DimDataConvert();
	d->CalPlaneData(vModelTmpl, viewShape, vBaseUnit);

	//�������ݻ���ͼ��
	for (size_t i = 0; i < viewShape.size(); i++)
	{
		//��������ǽ���š���
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
	//���ر�׼��
	for (size_t i = 0; i < viewShape.size(); i++)
	{
		//�����ǰҪ��Ⱦ����ǿ
		if (viewShape[i].unitType == 4)
		{
			int coordX = viewShape[i].nCen[0] + pSceneOffset;
			int coordY = viewShape[i].nCen[1] + pSceneOffset;
			//�ж�ǽ�ĽǶȼӱ�׼��
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

	if (MainDockState != 3)
	{
		graphicsViewMain->hide();
		graphicsViewMain->show();
	}
}
//���»����е�����Ԫ��
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
//�����������
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
