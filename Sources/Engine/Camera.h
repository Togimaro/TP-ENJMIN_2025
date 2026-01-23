#pragma once

#include "Buffer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Camera {
	float nearPlane = 0.01f;
	float farPlane = 500.0f;
	float fov = 60.0f;
	Quaternion rotation = Quaternion::Identity;
	Vector3 position = Vector3::Backward * 5;

	Matrix view;
	Matrix proj;
	BoundingFrustum bounds;

	struct CameraData {
		Matrix mView;
		Matrix mProj;
	};
	ConstantBuffer<CameraData> cbCamera;
public:
	Camera(float fov, float aspectRatio);

	void SetPosition(const Vector3& pos) { position = pos; UpdateViewMatrix(); }
	void SetRotation(const Quaternion& rot) { rotation = rot; UpdateViewMatrix(); }
	Vector3 GetPosition() const { return position; }
	Quaternion GetRotation() const { return rotation; }

	Vector3 Forward() const { return Vector3::TransformNormal(Vector3::Forward, view.Invert()); }
	Vector3 Right() const { return Vector3::TransformNormal(Vector3::Right, view.Invert()); }
	Vector3 Up() const { return Vector3::TransformNormal(Vector3::Up, view.Invert()); }
	Matrix GetInverseViewMatrix() const { return view.Invert(); }
	const BoundingFrustum& GetBounds() const { return bounds; }

	void Create(DeviceResources* deviceRes);
	void Apply(DeviceResources* deviceRes);

	void UpdateAspectRatio(float aspectRatio);
private:
	void UpdateViewMatrix();
};