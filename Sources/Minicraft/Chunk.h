#pragma once

#include "Engine/Buffer.h"
#include "Engine/VertexLayout.h"
#include "Block.h"
#include <array>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class World;

class Chunk {
public:
	constexpr static int CHUNK_SIZE = 8;
private:
	std::array<BlockId, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> data;
	VertexBuffer<VertexLayout_PositionNormalUV> vBuffer[SP_COUNT];
	IndexBuffer iBuffer[SP_COUNT];
	BoundingBox bounds;
	Matrix mModel;
	World* world;
	int cx, cy, cz;
	bool dirty = true;
public:
	Chunk() = default;

	void MarkDirty() { dirty = true; }
	void SetPosition(World* world, int cx, int cy, int cz);
	void Generate(DeviceResources* deviceRes);
	void Draw(DeviceResources* deviceRes, ShaderPass pass);
	const BoundingBox& GetBounds() const { return bounds; }
	const Matrix& GetLocalMatrix() const { return mModel; }

	BlockId* GetChunkCube(int cx, int cy, int cz);
private:
	bool ShouldRenderFace(int cx, int cy, int cz, int dx, int dy, int dz);
	void PushCube(int cx, int cy, int cz);
	void PushFace(Vector3 pos, Vector3 up, Vector3 right, int texId, ShaderPass pass);
};