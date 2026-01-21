#pragma once

#include "Engine/Buffer.h"
#include "Engine/VertexLayout.h"

using namespace DirectX::SimpleMath;

class Cube {
	VertexBuffer<VertexLayout_PositionUV> vBuffer;
	IndexBuffer iBuffer;
	Matrix mModel;
public:
	Cube(Vector3 pos);

	void Generate(DeviceResources* deviceRes);
	void Draw(DeviceResources* deviceRes);
	const Matrix& GetLocalMatrix() const { return mModel; }

private:
	void PushFace(Vector3 pos, Vector3 up, Vector3 right, int texId);
};