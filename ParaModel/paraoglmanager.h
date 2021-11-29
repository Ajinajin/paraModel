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
	//���̰����¼�
	void handleKeyPressEvent(QKeyEvent* event);
	//�����ͷ��¼�
	void handleKeyReleaseEvent(QKeyEvent* event);
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

	QTime time;
	Camera* camera;


	GLboolean isFirstMouse;
	GLboolean isLeftMousePress;
	GLint lastX;
	GLint lastY;
	GLuint uboMatrices;
};



#endif // OGLMANAGER_H
