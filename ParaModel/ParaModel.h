#pragma once
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

//#include "GLWidget.h"
#include <QtWidgets/QMainWindow>
#include <qfiledialog.h>
#include "ui_ParaModel.h"
#include "GLWidget.h"
#include <qmessagebox.h>
#include <qdebug.h>
#include <vector>
#include <qlistview.h>
#include <qlistwidget.h>
#include <qmenu.h>
#include <qlayout.h>

#include "bqgraphicsitem.h"
#include "bqgraphicsscene.h"

#include "paraoglmanager.h"
#include "SARibbonMainWindow.h"
#include <ParaType.h>


class QMenu;
class QListWidget;


class SARibbonCategory;
class ParaModel : public SARibbonMainWindow
{
    Q_OBJECT

//构造函数
public:
    ParaModel(QWidget* parent = 0);

private:
	QStatusBar* winStatus;							//状态栏
	QLabel* pTipBar;								// 状态栏中文字显示
	QLineEdit* pModelEdit[7];						// 战斗部属性编辑框 当量 弹片数量 弹片质量 分布角度1 分布角度2
	QLineEdit* pArmHeadEdit[5];						// 算法信息
	QTreeWidget* pModelTreeWidget;					// 系统模型树
	QDockWidget* MainDockWidget;					// 界面显示窗口
	BQGraphicsView* graphicsViewX;					// 二维模型X视图
	BQGraphicsView* graphicsViewY;					// 二维模型Y视图
	BQGraphicsView* graphicsViewZ;					// 二维模型Z视图
	BQGraphicsView* graphicsViewMain;				// 二维模型 三维视图
	BQGraphicsView* graphicsViewOgl;				// 三位模型展示视图

	BQGraphicsScene pSceneX;						// 二维模型X视图画布
	BQGraphicsScene pSceneY;						// 二维模型Y视图画布
	BQGraphicsScene pSceneZ;						// 二维模型Z视图画布
	BQGraphicsScene pSceneMain;						// 二维模型Z视图大屏幕主画布

	ParaOGLManager* oglmanager;							// 三维显示窗口类
	ParaOGLManager* oglmanagerMain;						// 三维显示窗口大屏幕主类
	 
	int if_data;									//0是未加载数据。1是有数据

	QTextEdit* myLogOutLabel;						// 日志窗口输出的文本
	int MainDockState;								// 0是X视图 1是Y视图 2是Z视图 3是三维视图
    // mainLabel显示的图像
    QMenu* popMenu_In_ListWidget_; /*弹出菜单*/
    QAction * action_Replace_ListWidget_;/*菜单上的Action*/
    //QGraphicsScene* m_scene = nullptr;

	// 初始化系统库
public:
	VUNITTABLE vBaseUnit;					// 系统基本构件库
	VTOPOTABLE vModelTmpl;					// 系统平面图库
	int InitPath();							// 初始化路径
	int InitUnitLib();						// 初始化基本构件库 
	int InitPlaneDrawLib();					// 初始化平面图库
	int InitParaTmpl();						// 初始化参数化生成模板
private:
	//初始化窗口
	void InitWindow();
	//初始化内容区域
	void InitCentralWidget();
	//初始化中间独立窗口
	void InitOglManagerWidget(QDockWidget* from);
	//初始化日志窗口
	void InitLogWidget(QDockWidget* from);
	//初始化已加载的模型窗口
	void InitLoadModelWidget(QDockWidget* from);
	//初始化系统模型窗口
	void InitSysWidget(QDockWidget* from);
	//初始化属性窗口
	void InitPropertyWidget(QDockWidget* from);


	//初始化状态栏文字提示
	void InitStatusWidget(); 

	//初始化Ribbon
	void InitSARibbon();
	//初始化Ribbon中的文件菜单
	void InitCategoryMain(SARibbonCategory* page); 

	//初始化弹出窗口
	void InitTipWindow(); 


	//初始化构建
	void InitBaseUnit();

public slots:
	void MyLogOutput(const char* myLogout);         //输出日志
	void ApplyDataAction();		//保存属性输入的数据
    void openAndLoadPic(); // 打开
    void getPara(); // 更改参数
    void resetPara(); // 取消更改
    void loadViews(); // 加载旁侧图
    void showImageList(QStringList& fileNames); // 显示右侧图像列表
    void replaceSlot(); // 右击替换
    void drawWall(const std::vector<float>& points);

	void GraphicsViewXFocus(bool b); // 加载旁侧图
	void GraphicsViewYFocus(bool b); // 加载旁侧图
	void GraphicsViewZFocus(bool b); // 加载旁侧图
	void GraphicsViewOgl(bool b);	//opengl旁侧图

private slots:
	void onCustomContextMenuRequested(const QPoint& pos);
	void on_polygonBtn_clicked(); 
	void NewFileAction();
	void OpenFileAction();
	void CloseFileAction();
};
