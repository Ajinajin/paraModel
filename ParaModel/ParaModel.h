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
class ParaModel : public SARibbonMainWindow
{
	Q_OBJECT
		//构造函数
public:
	explicit ParaModel(QWidget* parent = 0);
	~ParaModel();

private:
	QWidget* GenerateLayerTipWidget;					// 层
	QStatusBar* winStatus;							// 状态栏
	QLabel* pTipBar;								// 状态栏中文字显示
	QLineEdit* pModelEdit[2];						// 模型信息
	QLineEdit* pRoomEdit[4];						// 房间信息
	QTreeWidget* pModelTreeWidget;					// 系统模型树
	QTreeWidget* pLayerModelTreeWidget;				// 系统模型树
	QTreeWidget* pModelUnitTreeWidget;				// 可替换的模型树
	QDockWidget* MainDockWidget;					// 界面显示窗口 
	BQGraphicsView* graphicsViewMain;				// 二维模型三维视图
	BQGraphicsScene pSceneMain;						// 二维模型Z视图大屏幕主画布 

	QAction* pSceneBtn10;							// 10米画布按钮
	QAction* pSceneBtn20;							// 20米画布按钮
	QAction* pSceneBtn50;							// 50米画布按钮
	QAction* pSceneBtn100;							// 100米画布按钮
	QAction* pSceneBtn200;							// 200米画布按钮

	ParaOGLManager* paraOglmanagerMain;				// 三维显示
	QDockWidget* layerWidget;						// 层
	QLabel* pXImg;									// 二维显示窗口俯视角
	QLabel* pYImg;									// 二维显示窗口正视角

	int if_data;									//0是未加载数据。1是有数据
	SysPath oPath;									//系统路径
	QString existPath;								//当前程序正在打开的路径
	int pSceneOffset;								//绘制图像偏移
	int pAuxiliaryLine;								//辅助线长度

	QTextEdit* myLogOutLabel;						// 日志窗口输出的文本
	int MainDockState;								// 0是X视图 1是Y视图 2是Z视图 3是三维视图

	// mainLabel显示的图像
	QMenu* popMenu_In_ListWidget_;					/* 弹出菜单被使用无法删除*/

	int SelectUnitIdx;								// 当前选中的构件id
	int SelectUnitType;								// 当前选中的构件类型
	int nMoveXY[2];									// 记录鼠标移动的位置
	int moveXY[2];									// 记录鼠标点击的位置
	int SelectLayer;								// 当前选中的楼层




public:
	VUNITTABLE vBaseUnit;					// 系统基本构件库
	VTOPOTABLE vModelTmpl;					// 当前绘制计算用的平面图
	VBUILDTOPO vBuildTopo;					// 系统平面图库
	VBUILDTOPO vLoadModelData;				// 当前绘制计算的模型数据 多层级
	int InitSysData();						// 初始化系统变量
	int InitPath();							// 初始化路径
	int InitUnitLib();						// 初始化基本构件库 
	int InitLayerUnitLib();					// 初始化系统平面图库  
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
	void InitLayerWidget(QDockWidget* from);

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
	//初始化创建图层窗口
	void InitTipCreateLayer();

	//构件选择窗口
	void ShowUnitSelectWindow();
	void ShowAllUnitSelectWindow();

	//获取系统构建库中的构建
	BasicUnit GetBaseUnit(int idx);
	//获取加载构建集中的构建
	TopoUnit GetTopoUnit(int idx);
	//根据绘制构件类别识别颜色
	QColor ColorHelper(int nUnitType);
	//根据构件类别编码转换成对应的字符描述
	QString GetUnitType(int nUnitType);
	//根据构建类别的字符描述转换成对应的类别编码
	int GetUnitTypeCode(QString unitTypeStr);
	//根据构建形状的字符描述转换成对应的类别编码
	int GetShapeTypeCode(QString shapeTypeStr);
	//获取画布上选中的item
	QList<QGraphicsItem*> SelectSceneItem(int nUnitIdx);

	// 释放各种界面资源
	void ReleaseUISource();
	// 释放系统模型库
	void ReleaseSysModel();

	//清除画布
	void SceneMainClear(); 
	//刷新画布
	void RefreshSceneData();
	//刷新楼层数据
	void RefreshLayerWidget();
	//更新画布元素
	void UpdataSceneItem(int nUnitIdx, int x, int y, int width, int height);
public:
	VSHAPE viewShape;
	DimDataConvert* pCalShapeData;
public slots:
	void MyLogOutput(QString myLogout);         //输出日志
	void drawWall(const std::vector<float>& points);


	void GraphicsViewXFocus(bool b); // 加载旁侧图X
	void GraphicsViewYFocus(bool b); // 加载旁侧图Y
	void GraphicsViewZFocus(bool b); // 加载旁侧图Z

	void GraphicsViewOgl(bool b);	//opengl旁侧图
	void updateOGL();				//更新三维窗口内容
	void AddSceneData();				//更新画布内容 

	void NewFileAction();			//新建楼层户型数据
	void OpenFileAction();			//外部打开楼层户型数据
	void OpenLayerDataAction();		//外部打开建筑数据
	void CreateLayerAction();		//创建平面图
	void CloseFileAction();			//清空所有数据
	void SaveFileAction();			//保存当前数据
	void SaveasFileAction();		//另存当前数据
	void ExportFileAction();		//导出成k文件

	void SceneItemMoveAction(int nUnitType, int nUnitIdx, QPointF pos);		//画布移动元素
	void SceneMenuClickAction(int nUnitType, int nUnitIdx);	//画布菜单点击
	void SceneMenuDeleteClickAction(int nUnitType, int nUnitIdx);	//画布菜单点击
	void SceneMenuAddClickAction(int nUnitType, int nUnitIdx);	//画布菜单点击



	void CopyLayerAction();					//复制当前层数据
	void SaveLayerInfoAction();				//保存楼层信息
	void DeleteLayerAction(int layer);		//删除当前层数据
	void ChangeLayerAction(int layer);		//修改画布显示层数据

	void ChangeTopImgAction();				//切换顶视图
	void ChangeBottomImgAction();			//切换底视图
	void ChangeFrontImgAction();			//切换前视图图片
	void ChangeBackImgAction();				//切换后视图图片
	void ChangeLeftImgAction();				//切换左视图图片
	void ChangeRightImgAction();			//切换右视图图片

	void GenerateLayerAction();				//生成楼层房间
};
