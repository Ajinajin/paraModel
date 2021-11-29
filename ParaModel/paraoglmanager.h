#pragma once
#ifndef OGLMANAGER_H
#define OGLMANAGER_H

#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QTime>
#include <QOpenGLFunctions_4_3_Core>
#include <QMouseEvent>
#include "camera.h"




class ParaOGLManager : public QOpenGLWidget
{
public:
	explicit ParaOGLManager(QWidget* parent = 0);
	~ParaOGLManager();
	//键盘按下事件
	void handleKeyPressEvent(QKeyEvent* event);
	//键盘释放事件
	void handleKeyReleaseEvent(QKeyEvent* event);
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

	QTime time;
	Camera* camera;


	GLboolean isFirstMouse;
	GLboolean isLeftMousePress;
	GLint lastX;
	GLint lastY;
	GLuint uboMatrices;
};



#endif // OGLMANAGER_H
