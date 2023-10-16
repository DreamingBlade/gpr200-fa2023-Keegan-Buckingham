#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"

namespace kmb {

	//Identity matrix
	inline ew::Mat4 Identity() {
		return ew::Mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};
	//Scale on x,y,z axes
	inline ew::Mat4 Scale(ew::Vec3 s) {
		return ew::Mat4(
			s.x, 0, 0, 0,
			0, s.y, 0, 0,
			0, 0, s.z, 0,
			0, 0, 0, 1
		);
	};
	//Rotation around X axis (pitch) in radians
	inline ew::Mat4 RotateX(float rad) {
		return ew::Mat4(
			1, 0, 0, 0,
			0, cos(rad), -sin(rad), 0,
			0, sin(rad), cos(rad), 0,
			0, 0, 0, 1
		);
	};
	//Rotation around Y axis (yaw) in radians
	inline ew::Mat4 RotateY(float rad) {
		return ew::Mat4(
			cos(rad), 0, sin(rad), 0,
			0, 1, 0, 0,
			-sin(rad), 0, cos(rad), 0,
			0, 0, 0, 1);
	};
	//Rotation around Z axis (roll) in radians
	inline ew::Mat4 RotateZ(float rad) {
		return ew::Mat4(
			cos(rad), -sin(rad), 0, 0,
			sin(rad), cos(rad), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	};
	//Translate x,y,z
	inline ew::Mat4 Translate(ew::Vec3 t){
		 return ew::Mat4(
			1, 0, 0, t.x,
			0, 1, 0, t.y,
			0, 0, 1, t.z,
			0, 0, 0, 1
		);
	};

	

	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f); //Euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
		ew::Mat4 getModelMatrix() const {

			return kmb::Translate(position) * kmb::Scale(scale) * RotateZ(rotation.z) *
				RotateX(rotation.x) * RotateY(rotation.y);
		};

		
	};

	//Creates a right handed view space
	//eye = eye (camera) position
	//target = position to look at
	//up = up axis, usually(0,1,0)
	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 up) {

		ew::Vec3 f = ew::Cross(eye, up);


		//use ew::Cross for cross product!
		return ;

	};
	//Orthographic projection
	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far) {

		float right = aspect / 2;
		float left = -right;
		float top = height / 2;
		float bottom = -top;

		return ew::Mat4{
			2 / (right - left),   0,   0,  -((right + left) / (right - left)),
			0,  2 / (top - bottom),  0,  -((top + bottom) / (top - bottom)),
			0,  0,   -(2 / (far - near)),  -((far + near) / (far - near)),
			0,  0,  0,  1
		};
	};
	//Perspective projection
	//fov = vertical aspect ratio (radians)
	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far) {
		return ew::Mat4(
			1 / ((tan(fov / 2)) * aspect), 0, 0, 0,
			0, 1 / (tan(fov / 2)), 0, 0,
			0, 0, (near + far) / (near - far), (2 * far * near) / (near - far),
			0, 0, -1, 0
		);
	};

}
