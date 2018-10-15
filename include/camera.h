#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//default value
const float FOV = 45.f;
const float PITCH = 0.f;
const float YAW = -90.f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.01f;

class Camera
{
public:
	glm::vec3 Pos;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	//Euler angles
	float Yaw;
	float Pitch;
	float Speed;
	float Fov;
	float Sensitivity;

	Camera(glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f),
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f),
		float yaw = YAW,
		float pitch = PITCH
	) : Front(glm::vec3(0.f, 0.f, -1.f)), 
		Speed(SPEED), 
		Sensitivity(SENSITIVITY), 
		Fov(FOV)
	{
		Pos = pos;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
		Front(glm::vec3(0.f, 0.f, -1.f)),
		Speed(SPEED),
		Sensitivity(SENSITIVITY),
		Fov(FOV)
	{
		Pos = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Pos, this->Pos + this->Front, this->Up);
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = Speed * deltaTime;
		switch (direction)
		{
		case FORWARD:
			Pos += Front * velocity;
			break;
		case BACKWARD:
			Pos -= Front * velocity;
			break;
		case LEFT:
			Pos -= Right * velocity;
			break;
		case RIGHT:
			Pos += Right * velocity;
			break;
		default:
			break;
		}
	}

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= Sensitivity;
		yoffset *= Sensitivity;
		Yaw += xoffset;
		Pitch += yoffset;
		if (constrainPitch)
		{
			if (Pitch > 89.9f)
			{
				Pitch = 89.9f;
			}
			if (Pitch < -89.9f)
			{
				Pitch = -89.9f;
			}
		}
		updateCameraVectors();
	}

	void ProcessMouseScroll(float yoffset)
	{
		if (Fov >= 1.f && Fov <= 45.f)
		{
			Fov -= yoffset;
		}
		if (Fov <= 1.f)
		{
			Fov = 1.f;
		}
		if (Fov >= 45.f)
		{
			Fov = 45.f;
		}
	}

	~Camera() = default;

private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		this->Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};


#endif