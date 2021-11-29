#include "camera.h"

QMatrix4x4 Camera::getViewMatrix()
{
	QMatrix4x4 view;
	view.lookAt(this->position, this->position + this->front, this->up);
	return view;
}

void Camera::processKeyboard(ENCameraMovement direction, GLfloat deltaTime)
{
	GLfloat velocity = this->movementSpeed * deltaTime;
	if (direction == FORWARD)
		this->position += this->front * velocity;
	if (direction == BACKWARD)
		this->position -= this->front * velocity;
	if (direction == LEFT)
		this->position -= this->right * velocity;
	if (direction == RIGHT)
		this->position += this->right * velocity;
	if (direction == UP)
		this->position += this->worldUp * velocity;
	if (direction == DOWN)
		this->position -= this->worldUp * velocity;
}

void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch)
{
	xoffset *= this->mouseSensitivity;
	yoffset *= this->mouseSensitivity;

	this->yaw += xoffset;
	this->picth += yoffset;

	if (constraintPitch)
	{
		if (this->picth > 89.0f)
			this->picth = 89.0f;
		if (this->picth < -89.0f)
			this->picth = -89.0f;
	}

	this->updateCameraVectors();
}

void Camera::processMouseScroll(GLfloat yoffset)
{
	if (this->zoom >= 10 && this->zoom <= 100.0f)
		this->zoom -= yoffset;
	if (this->zoom > 100.0f)
		this->zoom = 100.0f;
	if (this->zoom < 10.0f)
		this->zoom = 10.0f;
}

void Camera::updateCameraVectors()
{
	//转换为弧度制Radians
	GLfloat yawR = qDegreesToRadians(this->yaw);
	GLfloat picthR = qDegreesToRadians(this->picth);

	QVector3D front3(cos(yawR) * cos(picthR), sin(picthR), sin(yawR) * cos(picthR));
	this->front = front3.normalized();
	this->right = QVector3D::crossProduct(this->front, this->worldUp).normalized();
	this->up = QVector3D::crossProduct(this->right, this->front).normalized();
}

