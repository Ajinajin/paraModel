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
	QWidget* mytreewidget = new QWidget();
	pModelTreeWidget = new QTreeWidget(mytreewidget);
	pModelTreeWidget->setHeaderHidden(true);
	from->setWidget(mytreewidget);
	from->setFixedWidth(300);
	from->setWindowTitle("ģ�����/�����б�");


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
		if (iter->oShape.nShapeType == 1)
		{
			childItem->setText(0, "����");
		}
		else if (iter->oShape.nShapeType == 2)
		{
			childItem->setText(0, "Բ");
		}
		else if (iter->oShape.nShapeType == 3)
		{
			childItem->setText(0, "�����");
		}
		else if (iter->oShape.nShapeType == 0)
		{
			childItem->setText(0, QString::number(iter->oShape.nThickNess));
		}
	}

	//����ť��Ӧ
	connect(pModelTreeWidget, &QTreeWidget::itemDoubleClicked, this, [=](QTreeWidgetItem* item, int column) {
		QTreeWidgetItem* parent = item->parent();
		if (NULL == parent) //ע�⣺�������û�и��ڵ�ģ�˫����Щ��ʱע��(����)
			return;
		int col = parent->indexOfChild(item); //item�ڸ����еĽڵ��к�(��0��ʼ)
		//1Բ 2��Բ 3������ 4����
		char* msg = "";
		if (0 == col) {
			//pSceneMain.clear();

			/*foreach(QGraphicsItem * item, pSceneMain.items())
			{
				pSceneMain.removeItem(item);
			} */

			msg = "Բģ�ͼ������";
			BCircle* m_circle = new BCircle(1000, 1000, 50, BGraphicsItem::ItemType::Circle);
			pSceneMain.addItem(m_circle);
		}
		else if (1 == col) {
			msg = "��Բģ�ͼ������";
			BEllipse* m_ellipse = new BEllipse(1000, 1000, 120, 80, BGraphicsItem::ItemType::Ellipse);
			pSceneMain.addItem(m_ellipse);
		}
		else if (2 == col) {
			msg = "������ģ�ͼ������";
			BSquare* m_square = new BSquare(1000, 1000, 60, BGraphicsItem::ItemType::Square);
			pSceneMain.addItem(m_square);
		}
		else if (3 == col) {
			msg = "����ģ�ͼ������";
			BRectangle* m_rectangle = new BRectangle(1000, 1000, 240, 120, BGraphicsItem::ItemType::Rectangle);
			m_rectangle->wallwidth = 120;
			pSceneMain.addItem(m_rectangle);
		}
		MyLogOutput(msg);
		});
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

	//��ά����
	oglmanager = new ParaOGLManager;
	graphicsViewOgl->setViewport(oglmanager);
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

	MainDockWidget->setWindowTitle("��ǰ�༭��ͼ ����άX��");
	/*oglManager = new OGLManager;*/
	MainDockWidget->setWidget(graphicsViewMain);
	MainDockWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);


	//QTimer* timer = new QTimer(this);
	//connect(timer, &QTimer::timeout, this, &ParaModel::updateOGL);
	////ÿ40msˢ��һ��OpenGL���� 25FPS
	//timer->start(40);

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

ParaModel::ParaModel(QWidget* parent)
	: SARibbonMainWindow(parent)
{
	//��ʼ��ϵͳ����
	InitBaseUnit();

	//��ʼ������
	InitWindow();
	InitTipWindow();
}

void ParaModel::InitBaseUnit()
{
	QString Path = "D:/Works/paramodel/x64/Debug/buildinglib.txt";
	QFile file(Path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(NULL, "��Ϣ��ʾ", "�ļ�����ʧ�ܣ�");
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

#pragma region ��״ʶ��
		if (list[2] == "����")
		{
			shape.nShapeType = 1;
			if (list[1] == "��" || list[1] == "��")
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
		else if (list[2] == "Բ��")
		{
			shape.nShapeType = 2;
			shape.nCen[0] = list[3].toInt();
			shape.nCen[1] = list[4].toInt();
			shape.nNumOrRadius = list[5].toInt();
		}
		else if (list[2] == "�����")
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

#pragma region UnitTypeʶ��
		if (list[1] == "��")
		{
			basic.nUnitType = 1;
		}
		else if (list[1] == "��")
		{
			basic.nUnitType = 2;
		}
		else if (list[1] == "¥��")
		{
			shape.nThickNess = list[2].toInt();
			basic.nUnitType = 3;
		}
		else if (list[1] == "ǽ")
		{
			shape.nThickNess = list[2].toInt();
			basic.nUnitType = 4;
		}
		else if (list[1] == "��")
		{
			basic.nUnitType = 5;
		}
		else if (list[1] == "��")
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
		QMessageBox::information(NULL, "��Ϣ��ʾ", "��ǰ�����ļ�����رպ��ڴ�");
	}
	if_data = 1;
}
void ParaModel::GraphicsViewXFocus(bool b)
{
	MainDockWidget->setWindowTitle("��ǰ�༭��ͼ ����άX��");
	MainDockState = 0;
	MainDockWidget->setWidget(graphicsViewMain);
	pSceneMain.setBackgroundBrush(Qt::darkGray);
	return;
}

void ParaModel::GraphicsViewYFocus(bool b)
{
	MainDockWidget->setWindowTitle("��ǰ�༭��ͼ ����άY��");
	MainDockState = 1;
	MainDockWidget->setWidget(graphicsViewMain);
	pSceneMain.setBackgroundBrush(Qt::lightGray);

	return;
}

void ParaModel::GraphicsViewZFocus(bool b)
{
	MainDockWidget->setWindowTitle("��ǰ�༭��ͼ ����άZ��");
	MainDockState = 2;
	MainDockWidget->setWidget(graphicsViewMain);
	pSceneMain.setBackgroundBrush(Qt::gray);
	return;
}

void ParaModel::GraphicsViewOgl(bool b)
{
	MainDockWidget->setWindowTitle("��ǰ�༭��ͼ ����άģ�ͣ�");
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

// �� �ۺ���
void ParaModel::openAndLoadPic() {
	//ѡ���ļ��Ի���/

	QFileDialog* f = new QFileDialog(this);
	f->setWindowTitle("ѡ�������ļ�*.txt");
	f->setNameFilter("*.txt");
	f->setViewMode(QFileDialog::Detail);
	QString filePath;
	if (f->exec() == QDialog::Accepted) {
		filePath = f->selectedFiles()[0];
	}
	// ��ȡ�ļ�����
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		myLogOutLabel->clear();
		myLogOutLabel->setText("�ļ���ʧ��");
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

// ͼ������
void ParaModel::getPara() {
	//// ��ȡ�ı����������
	//QString strText = scalemyLogOutLabel->toPlainText();
	//float width = 0, height = 0;
	//bool ok = false;
	//// �ָת��float, �Ӷ���ÿ� ��
	//QStringList sList = strText.split(",");
	//if (sList.size() != 2) {
	//    myLogOutLabel->clear();
	//    myLogOutLabel->setText("����ʧ�ܣ���������");
	//    return;
	//}

	//width = sList[0].toFloat(&ok);
	//if (!ok) {
	//    myLogOutLabel->clear();
	//    myLogOutLabel->setText("����ʧ�ܣ���������");
	//    return;
	//}

	//height = sList[1].toFloat(&ok);
	//if (!ok) {
	//    myLogOutLabel->clear();
	//    myLogOutLabel->setText("����ʧ�ܣ���������");
	//    return;
	//}

	//if (height <= 0 || width <= 0) {
	//    myLogOutLabel->clear();
	//    myLogOutLabel->setText("����ʧ�ܣ���������");
	//    return;
	//}
	//// ���mainLabel��δ����ͼ�񣬼�imgΪ�գ����ܽ�������
	////mainlabel->clear();
	//if (img == nullptr) {
	//    myLogOutLabel->clear();
	//    myLogOutLabel->setText("����ʧ�ܣ�ͼ��򿪴���");
	//    return;
	//}
	//// ��ȡԭͼ��Ĵ�С����������ϵ���������µ�ͼ���С
	//int nowWidth = mainlabel->pixmap()->toImage().width();
	//int nowHeight = mainlabel->pixmap()->toImage().height();
	//QSize picSize(int(width * nowWidth), int(height * nowHeight));

	//QImage timage = mainlabel->pixmap()->toImage().scaled(picSize);
	//mainlabel->setScaledContents(false); // ȡ������Ӧ
	//// ����ͼ����ӵ�mainLabel��
	//QPixmap pmap = QPixmap::fromImage(timage);
	//QPixmap tpmap = pmap.scaled(picSize, Qt::KeepAspectRatio);
	//mainlabel->setPixmap(tpmap);

	//myLogOutLabel->clear();
	//myLogOutLabel->setText("���ųɹ�");
}

// ȡ������,��Ҫ�߼����Ǳ�������ǰ��ͼ���������뼴��
void ParaModel::resetPara() {
	//mainlabel->clear();
	//mainlabel->setScaledContents(true); // ����Ӧ
	//mainlabel->setPixmap(QPixmap::fromImage(*img));
	//myLogOutLabel->clear();
	//myLogOutLabel->setText("ȡ���ɹ�");
}

// ����ͼ���б�
void ParaModel::loadViews() {

	//�����ļ��Ի�����
	QFileDialog* fileDialog = new QFileDialog(this);
	//�����ļ��Ի������
	fileDialog->setWindowTitle(tr("��ͼƬ"));
	//����Ĭ���ļ�·��
	fileDialog->setDirectory(".");
	//�����ļ�������
	fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
	//���ÿ���ѡ�����ļ�,Ĭ��Ϊֻ��ѡ��һ���ļ�QFileDialog::ExistingFiles
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	//������ͼģʽ
	fileDialog->setViewMode(QFileDialog::Detail);
	//��ӡ����ѡ����ļ���·��
	if (fileDialog->exec()) {
		QStringList fileNames = fileDialog->selectedFiles();
		showImageList(fileNames);
	}
}

// ��ʾͼ���б�
void ParaModel::showImageList(QStringList& fileNames) {
	if (fileNames.size() == 0) {
		myLogOutLabel->clear();
		myLogOutLabel->setText("�ļ��б��");
		return;
	}
	// ��������ͼ��·�������
	for (auto tmp : fileNames) {
		QPixmap pixmap(tmp);
		//����QListWidgetItem����
		QListWidgetItem* imageItem = new QListWidgetItem;
		// ���� QWidget �� QListWidgetItem һ����ӵ� QListWidget
		QWidget* widget = new QWidget;
		// ���� widget ���ַ�ʽΪ box
		QVBoxLayout* widgetLayout = new QVBoxLayout;
		// ͼ���ͼ������label
		QLabel* imageLabel = new QLabel;
		QLabel* txtLabel = new QLabel(tr("Browse"));
		// ����widget�Ĳ���
		widget->setLayout(widgetLayout);
		widgetLayout->setMargin(0);
		widgetLayout->setSpacing(0);
		widgetLayout->addWidget(imageLabel);
		widgetLayout->addWidget(txtLabel);

		// ��ͼ���ͼ������ӵ���Ӧ��label
		pixmap = pixmap.scaled(200, 250, Qt::KeepAspectRatio);
		// �޸�label����Ϣ
		imageLabel->setPixmap(pixmap);
		txtLabel->setFixedHeight(20);
		txtLabel->setWordWrap(true);
		imageItem->setSizeHint(QSize(200, 250));
		// �� imageItem, widget��ӵ� imageList
		//imageList->addItem(imageItem);
		//imageList->setItemWidget(imageItem, widget);
	}
}

// �һ��滻
void ParaModel::replaceSlot() {
	//// ����ѡ�е�item������ֻ��һ��
	//QList<QListWidgetItem*> items = imageList->selectedItems(); 
	//if (items.count() != 1) { // ��ѡ
	//    return;
	//}
	//QListWidgetItem* item = items[0];

	//// ͨ�� QListWidgetItem �ҵ� QWidget �����ȡ��Ԫ��    
	//QWidget* pw = imageList->itemWidget(item); 
	//if (pw == nullptr) {
	//    return;
	//}
	//// Ĭ�ϻ�ȡ��һ��
	//QLabel* imageLabel = pw->findChild<QLabel*>(); 
	//// ��ȡͼ��
	//QImage img = imageLabel->pixmap()->toImage(); 

	//// ����
	//mainlabel->clear();
	//mainlabel->setScaledContents(true); // ����Ӧ
	//mainlabel->setPixmap(QPixmap::fromImage(img));
	//myLogOutLabel->clear();
	//myLogOutLabel->setText("�滻�ɹ�");
}

// �Զ����Ҽ��˵�����
void ParaModel::onCustomContextMenuRequested(const QPoint& pos) {
	/*�����Ҽ��˵�*/
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

//�洢view����

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
	// 0-5 ������ǽ�Ŵ�
	QString sType = "������ǽ�Ŵ�";
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
// ��ʼ������������ 
int ParaModel::InitUnitLib()
{
	QString sUnitFile("d:/����������");
	QFile fileScn(sUnitFile);
	if (!fileScn.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return 1;
	}
	QTextStream inScn(&fileScn);

	QTextCodec* pCode = QTextCodec::codecForName("UTF-8");

	// ���ж����ļ�����
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

