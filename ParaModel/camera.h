#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QOpenGLShader>
#include <QDebug>
#include <QtMath>


enum ENCameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

//Ԥ����״̬����
const GLfloat YAW = -115.0f;
const GLfloat PITCH = -15.0f;
const GLfloat SPEED = 0.25;
const GLfloat SENSITIVITY = 0.1f;
const GLfloat ZOOM = 60.0f;

class Camera
{
public:
	Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f),
		GLfloat yaw = YAW, GLfloat pitch = PITCH) : front(QVector3D(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
		mouseSensitivity(SENSITIVITY), zoom(ZOOM) {

		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->picth = pitch;
		this->updateCameraVectors();
	}

	//����lookat����
	QMatrix4x4 getViewMatrix();
	//��Ӧ����������
	void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch = true);
	//�������¼�
	void processMouseScroll(GLfloat yoffset);
	//���̴����¼�����
	void processKeyboard(ENCameraMovement direction, GLfloat deltaTime);
	//void processInput(GLfloat dt);//����ѭ������

	QVector3D position;
	QVector3D worldUp;
	QVector3D front;

	//view����ϵ ���Ϸ���
	QVector3D up;
	// view����ϵ�� �ҷ���
	QVector3D right;

	//���ŷ����
	GLfloat picth;
	GLfloat yaw;

	//�������
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

private:
	void updateCameraVectors();

};
#endif // CAMERA_H
