#pragma once
#ifndef WARHEADOGLMANAGER_H
#define WARHEADOGLMANAGER_H

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


class WarheadOGLManager : public QOpenGLWidget
{
public:
	explicit WarheadOGLManager(QWidget* parent = 0);
	~WarheadOGLManager();


public:
	//绘制
	
	//柱
	void InitAndDrawColumn(Ver3D center, float radius, float height);		//参数：中心先下点，半径，高度

	//圆环柱
	//第二个参数是大圈半径，第四个参数是小圈半径
	void DrawColumnRing(Ver3D center, float radius, float height,float radius2);		

	//圆柱侧面(第四个参数为设置是否透明)
	void DrawColumnSide(Ver3D center, float radius, float height,GLboolean ifTrans);

	//绘制圆台侧面
	//第二个参数是底面圆半径，第四个参数是顶面圆半径
	void DrawRoundPConeSide(Ver3D center, float radius, float height,float radius2);

	//绘制圆环
	//第一个参数大圈半径，第二、三个参数大圈、小圈半径
	void DrawRing(Ver3D center,float r1,float r2);

	//引信绘制
	//第一个参数是下底中心点，第二、四个参数大圈、小圈半径，第三、五个参数是大柱高度与小柱高度
	void DrawFuse(Ver3D center,float bigR,float height1,float smallR,float height2);

	//绘制球(参数1是所有球的球心位置集合)
	void DrawSphere(VFLOAT centers, float radius);


	vPoint allNodes;			//存储三维模型的所有节点信息


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

	//opengl 更新函数
	void updateGL();

	QOpenGLFunctions_4_3_Core* pCore;


	GLuint uboMatrices;
	GLuint VBO, EBO, instanceVBO;
};



#endif // PARAOGLMANAGER_H
