#pragma once

#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "transformations.h"

namespace kmb {
	struct Camera {
		ew::Vec3 position; //Camera body position
		ew::Vec3 target; //Position to look at
		float fov; //Vertical field of view in degrees
		float aspectRatio; //Screen width / Screen height
		float nearPlane; //Near plane distance (+Z)
		float farPlane; //Far plane distance (+Z)
		bool orthographic; //Perspective or orthographic?
		float orthoSize; //Height of orthographic frustum
		ew::Mat4 ViewMatrix() //World->View
		{
			ew::Vec3 up(0, 1, 0);
			return kmb::LookAt(position, target, up);
		}
		ew::Mat4 ProjectionMatrix() //View->Clip
		{
			if (orthographic)
			{
				return kmb::Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
			}
			else
			{
				return kmb::Perspective(fov, aspectRatio, nearPlane, farPlane);
			}
		}
	};
}
