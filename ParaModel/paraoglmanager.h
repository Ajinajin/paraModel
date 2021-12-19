#pragma once
#ifndef PARAOGLMANAGER_H
#define PARAOGLMANAGER_H

#include <QDockWidget>
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QTime>
#include <QOpenGLFunctions_4_3_Core>
#include <QMouseEvent>


#include"ParaType.h"
#include<resourcemanager.h>
#include "camera.h"
#include <ShaderProc.h>

typedef vector<QImage> vQImage;

struct Point		//点结构(二维)
{
	int x;
	int y;
	int z;
};
typedef vector<Point> vPoint;

struct Solid
{
	int idx[2][4];
};
typedef vector<Solid> vSolid;

//根据中心构件单元Id去构件库里查具体模型参数
BasicUnit findUnit(int idx, VUNITTABLE oglUnitTable);

class ParaOGLManager : public QOpenGLWidget
{
public:
	explicit ParaOGLManager(QWidget* parent = 0);
	~ParaOGLManager();

	void handleKeyPressEvent(QKeyEvent* event);
	//void keyPressEvent(QKeyEvent* event)override;			//键盘按下事件

	void handleKeyReleaseEvent(QKeyEvent* event);
	//void keyReleaseEvent(QKeyEvent* event)override;		//键盘释放事件


	GLboolean keys[1024];			// 获取键盘按键，实现多键触控

	enum SwitchView //相机视角枚举，七种视图（一个默认视图加六个标准视图）
	{
		NONE,
		FRONT_VIEW,
		BACK_VIEW,
		LEFT_VIEW,
		RIGHT_VIEW,
		TOP_VIEW,
		BOTTOM_VIEW
	};
	SwitchView switchView;


public:
	//绘制
	// 
	//柱
	void InitAndDrawColumn(int x, int y, int z, int radius, int height);//参数：底面圆心xz，半径，高度
	
																		
	//多边形体(竖着),多边形绘制算法有待完善(opengl只能绘制凸多边形)
	void InitAndDrawPolygonPortrait(VINT data, int begin_y, int height);
	//多边形体(横着)
	void InitAndDrawPolygonHorizontal(VINT data, int length);
	
	//长方体（墙、板等）x\y\z 为长方体的中心点
	void InitAndDrawCuboid(int x, int y, int z, int length, int thickness, int height, int type);
	
	void outputKFile();
	//返回六个视图图片
	
	VTOPOTABLE oglTopTable;		//绘制所需的拓扑结构表
	VUNITTABLE oglUnitTable;	//绘制所需的结构单元表
	
	vPoint allNodes;			//存储三维模型的所有节点信息
	vSolid allSolids;			//存储三维模型内的所有solid信息
	int outFlag = 0;			//暂时变量，用于只输出一次K文件


public:
	//相机、鼠标键入相关参数
	Camera* camera;
	GLboolean isFirstMouse;

	GLboolean isLeftMousePress;
	GLboolean isRightMousePress;

	GLint lastX;
	GLint lastY;
	QTime time;
	GLfloat deltaTime;
	GLfloat lastFrame;											//上一帧

	//模型旋转相关参数
	QMatrix4x4 targetModel;										//模型的模型GLSL矩阵
	QMatrix4x4 targetModelsave;
	QMatrix4x4 targetModeluse;
	QMatrix4x4 view;											//内部的总体渲染观察矩阵
	GLboolean if_change_view;									//从外部更改三维显示观察矩阵 标志位

	GLfloat rotateRaw;											//每次对模型的raw旋转
	GLfloat rotatePitch;										//每次对模型的pitch旋转


protected:
	void mouseMoveEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event) override;			//滚轮事件
	void mousePressEvent(QMouseEvent* event) override;		//鼠标按下事件
	void mouseReleaseEvent(QMouseEvent* event) override;	//鼠标释放事件
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();



private:
	//摄像机键盘处理函数，
	void processInput(GLfloat dt);
	//opengl 更新函数
	void updateGL();

	QOpenGLFunctions_4_3_Core* pCore;







	GLuint uboMatrices;
	GLuint VBO, EBO;
};



#endif // PARAOGLMANAGER_H
