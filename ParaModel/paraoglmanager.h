#pragma once
#ifndef PARAOGLMANAGER_H
#define PARAOGLMANAGER_H

#include <QDockWidget>
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QTime>
#include <QOpenGLFunctions_4_3_Core>
#include <QMouseEvent>

#include<resourcemanager.h>
#include "camera.h"
#include <ShaderProc.h>



class ParaOGLManager : public QOpenGLWidget
{
public:
	explicit ParaOGLManager(QWidget* parent = 0);
	~ParaOGLManager();
	
	void handleKeyPressEvent(QKeyEvent* event);
	//void keyPressEvent(QKeyEvent* event)override;			//���̰����¼�
	
	void handleKeyReleaseEvent(QKeyEvent* event);
	//void keyReleaseEvent(QKeyEvent* event)override;		//�����ͷ��¼�


	GLboolean keys[1024];			// ��ȡ���̰�����ʵ�ֶ������

	enum SwitchView //����ӽ�ö�٣�������ͼ��һ��Ĭ����ͼ��������׼��ͼ��
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

	void InitAndDrawColumn(float radius,float height);


	//�������������ز���
	Camera* camera;
	GLboolean isFirstMouse;

	GLboolean isLeftMousePress;
	GLboolean isRightMousePress;
	
	GLint lastX;
	GLint lastY;
	QTime time;
	GLfloat deltaTime;
	GLfloat lastFrame;											//��һ֡

	GLfloat rotateRaw;											//ÿ�ζ�ģ�͵�raw��ת
	GLfloat rotatePitch;										//ÿ�ζ�ģ�͵�pitch��ת


protected:
	void mouseMoveEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event) override;			//�����¼�
	void mousePressEvent(QMouseEvent* event) override;		//��갴���¼�
	void mouseReleaseEvent(QMouseEvent* event) override;	//����ͷ��¼�
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();
	

	
private:
	//��������̴�������
	void processInput(GLfloat dt);
	//opengl ���º���
	void updateGL();

	QOpenGLFunctions_4_3_Core* pCore;

	
	


	

	GLuint uboMatrices;
	GLuint VBO,EBO;
};



#endif // PARAOGLMANAGER_H
