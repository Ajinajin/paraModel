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

#include "DimDataConvert.h"
#include <QFormLayout>

class QMenu;
class QListWidget;


class SARibbonCategory;
class WarheadParaModel : public SARibbonMainWindow
{
	Q_OBJECT
		//构造函数
public:
	explicit WarheadParaModel(QWidget* parent = 0);
	~WarheadParaModel();

private:
	QStatusBar* winStatus;							// 状态栏
	QLabel* pTipBar;								// 状态栏中文字显示
	QLineEdit* pModelEdit[6];						// 模型信息
	QDockWidget* MainDockWidget;					// 界面显示窗口 
	BQGraphicsView* graphicsViewMain;				// 二维模型三维视图
	BQGraphicsScene pSceneMain;						// 二维模型Z视图大屏幕主画布 

	ParaOGLManager* paraOglmanagerMain;				// 三维显示


	int if_data;									//0是未加载数据。1是有数据
	SysPath oPath;									//系统路径
	QString existPath;								//当前程序正在打开的路径
	int pSceneOffset;								//绘制图像偏移
	int pAuxiliaryLine;								//辅助线长度

	QTextEdit* myLogOutLabel;						// 日志窗口输出的文本
 
	// mainLabel显示的图像
	QMenu* popMenu_In_ListWidget_;					/* 弹出菜单被使用无法删除*/
	 


public:
	VUNITTABLE vBaseUnit;					// 系统基本构件库 
	int InitSysData();						// 初始化系统变量
	int InitPath();							// 初始化路径
	int InitUnitLib();						// 初始化基本构件库  
	int InitParaTmpl();						// 初始化参数化生成模板

private:
	//初始化窗口
	void InitWindow();
	//初始化内容区域
	void InitCentralWidget();
	//初始化中间独立窗口
	void InitEditManagerWidget(QDockWidget* from);

	//初始化三维显示窗口
	void InitOpenGlWidget(QDockWidget* from);

	//初始化日志窗口
	void InitLogWidget(QDockWidget* from);
	//初始化已加载的模型窗口
	void InitLoadModelWidget(QDockWidget* from);
	//初始化系统构件模型窗口
	void InitSysUnitWidget(QDockWidget* from);
	//初始化系统平面模型窗口
	void InitSysLayerWidget(QDockWidget* from);

	//初始化楼层选择
	void InitWarheadWidget(QDockWidget* from);

	//初始化状态栏文字提示
	void InitStatusWidget();

	//初始化Ribbon
	void InitSARibbon();
	//初始化Ribbon中的文件菜单
	void InitCategoryMain(SARibbonCategory* page);
	//初始化Ribbon中的画布大小
	void InitCategoryScene(SARibbonCategory* page);

	//初始化弹出窗口
	void InitTipWindow();
	
	// 释放各种界面资源
	void ReleaseUISource();
	// 释放系统模型库
	void ReleaseSysModel();

	//清除画布
	void SceneMainClear(); 
	 
public slots:
	void MyLogOutput(QString myLogout);         //输出日志
	
	void updateOGL();				//更新三维窗口内容
	void AddSceneData();				//更新画布内容 

	void NewFileAction();			//新建楼层户型数据
	void OpenFileAction();			//外部打开楼层户型数据
	void CloseFileAction();			//清空所有数据
	void SaveFileAction();			//保存当前数据
	void SaveasFileAction();		//另存当前数据
	void ExportFileAction();		//导出成k文件
};
