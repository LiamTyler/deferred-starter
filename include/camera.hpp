#pragma once

#include "common.hpp"
#include "transform.hpp"

class Camera {
	public:
		Camera(
			const Transform& t = Transform(),
			float fovy = glm::radians(45.0f),
			float a = 16.0f / 9.0f,
			float np = .1f,
			float fp = 100.0f);

		void Update(float dt);

		float GetFOV() const { return fieldOfView_; }
		float GetAspectRatio() const { return aspectRatio_; }
		float GetNearPlane() const { return nearPlane_; }
		float GetFarPlane() const { return farPlane_; }
		glm::mat4 GetV() const { return viewMatrix_; }
		glm::mat4 GetP() const { return projectionMatrix_; }

		// Get the current forward/up/right vectors of the camera
		// These are really convenient when you have something like a player
		// controlled camera
		glm::vec3 GetForwardDir() const { return currDir_; }
		glm::vec3 GetUpDir() const { return currUp_; }
		glm::vec3 GetRightDir() const { return currRight_; }

		void SetFOV(float f);
		void SetAspectRatio(float a);
		void SetNearPlane(float p);
		void SetFarPlane(float p);
		void UpdateOrientationVectors();
		void UpdateViewMatrix();

		Transform transform;
		glm::vec3 velocity;

	protected:
		void UpdateProjectionMatrix();

		float fieldOfView_;
		float aspectRatio_;
		float nearPlane_;
		float farPlane_;
		glm::mat4 viewMatrix_;
		glm::mat4 projectionMatrix_;
		glm::vec3 currDir_;
		glm::vec3 currUp_;
		glm::vec3 currRight_;
};