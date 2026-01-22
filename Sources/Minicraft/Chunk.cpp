#include "pch.h"
#include "Chunk.h"
#include "World.h"

void Chunk::SetPosition(World* world, int cx, int cy, int cz) {
	mModel = Matrix::CreateTranslation(Vector3(cx, cy, cz) * Chunk::CHUNK_SIZE);
	this->cx = cx;
	this->cy = cy;
	this->cz = cz;
	this->world = world;
}

void Chunk::Generate(DeviceResources* deviceRes) {
	for (int z = 0; z < CHUNK_SIZE; z++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int x = 0; x < CHUNK_SIZE; x++) {
				PushCube(x, y, z);
			}
		}
	}

	vBuffer.Create(deviceRes);
	iBuffer.Create(deviceRes);
}

void Chunk::Draw(DeviceResources* deviceRes) {
	if (iBuffer.Size() == 0) return;
	vBuffer.Apply(deviceRes);
	iBuffer.Apply(deviceRes);
	deviceRes->GetD3DDeviceContext()->DrawIndexed(iBuffer.Size(), 0, 0);
}

bool Chunk::ShouldRenderFace(int lx, int ly, int lz, int dx, int dy, int dz) {
	auto blockId = world->GetCube(
		cx * CHUNK_SIZE + lx + dx, 
		cy * CHUNK_SIZE + ly + dy, 
		cz * CHUNK_SIZE + lz + dz);
	if (!blockId || *blockId == EMPTY) return true;
	return false;
}

BlockId* Chunk::GetChunkCube(int lx, int ly, int lz) {
	if (lx < 0) return nullptr;
	if (ly < 0) return nullptr;
	if (lz < 0) return nullptr;
	if (lx >= CHUNK_SIZE) return nullptr;
	if (ly >= CHUNK_SIZE) return nullptr;
	if (lz >= CHUNK_SIZE) return nullptr;
	return &data[lx + ly * CHUNK_SIZE + lz * CHUNK_SIZE * CHUNK_SIZE];
}

void Chunk::PushCube(int lx, int ly, int lz) {
	auto blockId = GetChunkCube(lx, ly, lz);
	if (!blockId || *blockId == EMPTY) return;
	auto& blockData = BlockData::Get(*blockId);

	Vector3 offset = Vector3(lx, ly, lz);
	if (ShouldRenderFace(lx, ly, lz, 0, 0, 1)) PushFace(offset + Vector3::Zero, Vector3::Up, Vector3::Right, blockData.texIdSide);
	if (ShouldRenderFace(lx, ly, lz, 1, 0, 0)) PushFace(offset + Vector3::Right, Vector3::Up, Vector3::Forward, blockData.texIdSide);
	if (ShouldRenderFace(lx, ly, lz, 0, 0, -1)) PushFace(offset + Vector3::Right + Vector3::Forward, Vector3::Up, Vector3::Left, blockData.texIdSide);
	if (ShouldRenderFace(lx, ly, lz, -1, 0, 0)) PushFace(offset + Vector3::Forward, Vector3::Up, Vector3::Backward, blockData.texIdSide);
	if (ShouldRenderFace(lx, ly, lz, 0, 1, 0)) PushFace(offset + Vector3::Up, Vector3::Forward, Vector3::Right, blockData.texIdTop);
	if (ShouldRenderFace(lx, ly, lz, 0, -1, 0)) PushFace(offset + Vector3::Right + Vector3::Forward, Vector3::Left, Vector3::Backward, blockData.texIdBottom);
}

void Chunk::PushFace(Vector3 pos, Vector3 up, Vector3 right, int texId) {
	Vector2 uv(
		texId % 16,
		texId / 16
	);
	uint32_t bottomLeft = vBuffer.PushVertex(VertexLayout_PositionUV(pos, (uv + Vector2::UnitY) / 16.0f));
	uint32_t bottomRight = vBuffer.PushVertex(VertexLayout_PositionUV(pos + right, (uv + Vector2::One) / 16.0f));
	uint32_t upLeft = vBuffer.PushVertex(VertexLayout_PositionUV(pos + up, uv / 16.0f));
	uint32_t upRight = vBuffer.PushVertex(VertexLayout_PositionUV(pos + up + right, (uv + Vector2::UnitX) / 16.0f));
	iBuffer.PushTriangle(bottomLeft, upLeft, upRight);
	iBuffer.PushTriangle(bottomLeft, upRight, bottomRight);
}
