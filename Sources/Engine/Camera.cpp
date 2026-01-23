#include "pch.h"

#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera(float fov, float aspectRatio) : fov(fov) {
	UpdateAspectRatio(aspectRatio);
	UpdateViewMatrix();
}

void Camera::Create(DeviceResources* deviceRes) {
	cbCamera.Create(deviceRes);
}

void Camera::Apply(DeviceResources* deviceRes) {
	cbCamera.ApplyToVS(deviceRes, 1);
	cbCamera.data.mView = view.Transpose();
	cbCamera.data.mProj = proj.Transpose();
	cbCamera.Update(deviceRes);
}

void Camera::UpdateAspectRatio(float aspectRatio) {
	proj = Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(fov),
		aspectRatio,
		nearPlane,
		farPlane
	);
}

void Camera::UpdateViewMatrix() {
	Vector3 forward = Vector3::Transform(Vector3::Forward, rotation);
	Vector3 up = Vector3::Transform(Vector3::Up, rotation);

	view = Matrix::CreateLookAt(
		position,
		position + forward,
		up
	);

	BoundingFrustum::CreateFromMatrix(bounds, proj, true);
	bounds.Transform(bounds, view.Invert());
}
