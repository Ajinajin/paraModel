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

//预定义状态数据
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

	//返回lookat函数
	QMatrix4x4 getViewMatrix();
	//相应鼠标操作函数
	void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch = true);
	//鼠标滚轮事件
	void processMouseScroll(GLfloat yoffset);
	//键盘处理事件函数
	void processKeyboard(ENCameraMovement direction, GLfloat deltaTime);
	//void processInput(GLfloat dt);//键盘循环函数

	QVector3D position;
	QVector3D worldUp;
	QVector3D front;

	//view坐标系 的上方向
	QVector3D up;
	// view坐标系的 右方向
	QVector3D right;

	//相机欧拉角
	GLfloat picth;
	GLfloat yaw;

	//相机参数
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

private:
	void updateCameraVectors();

};
#endif // CAMERA_H
