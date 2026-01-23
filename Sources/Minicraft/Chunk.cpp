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
	for (int pass = SP_OPAQUE; pass < SP_COUNT; pass++) {
		vBuffer[pass].Clear();
		iBuffer[pass].Clear();
	}
	for (int z = 0; z < CHUNK_SIZE; z++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int x = 0; x < CHUNK_SIZE; x++) {
				PushCube(x, y, z);
			}
		}
	}

	for (int pass = SP_OPAQUE; pass < SP_COUNT; pass++) {
		vBuffer[pass].Create(deviceRes);
		iBuffer[pass].Create(deviceRes);
	}
	dirty = false;
}

void Chunk::Draw(DeviceResources* deviceRes, ShaderPass pass) {
	if (dirty) Generate(deviceRes);
	if (iBuffer[pass].Size() == 0) return;
	vBuffer[pass].Apply(deviceRes);
	iBuffer[pass].Apply(deviceRes);
	deviceRes->GetD3DDeviceContext()->DrawIndexed(iBuffer[pass].Size(), 0, 0);
}

bool Chunk::ShouldRenderFace(int lx, int ly, int lz, int dx, int dy, int dz) {
	auto blockIdNeighbour = world->GetCube(
		cx * CHUNK_SIZE + lx + dx, 
		cy * CHUNK_SIZE + ly + dy, 
		cz * CHUNK_SIZE + lz + dz);
	if (!blockIdNeighbour || *blockIdNeighbour == EMPTY) return true;

	auto blockId = world->GetCube(
		cx * CHUNK_SIZE + lx,
		cy * CHUNK_SIZE + ly,
		cz * CHUNK_SIZE + lz);
	auto& blockData = BlockData::Get(*blockId);
	auto& blockDataNeighbour = BlockData::Get(*blockIdNeighbour);

	if (blockData.pass == SP_OPAQUE && blockDataNeighbour.pass == SP_TRANSPARENT) return true;

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

	float scaleY = 1.0f;
	if (*blockId == WATER) {
		auto blockIdNeighbour = world->GetCube(
			cx * CHUNK_SIZE + lx,
			cy * CHUNK_SIZE + ly + 1,
			cz * CHUNK_SIZE + lz);
		if (!blockIdNeighbour || *blockIdNeighbour == EMPTY)
			scaleY = 0.8f;
	}

	Vector3 offset = Vector3(lx, ly, lz);
	if (ShouldRenderFace(lx, ly, lz, 0, 0, 1)) PushFace(offset + Vector3::Zero, Vector3::Up * scaleY, Vector3::Right, blockData.texIdSide, blockData.pass);
	if (ShouldRenderFace(lx, ly, lz, 1, 0, 0)) PushFace(offset + Vector3::Right, Vector3::Up * scaleY, Vector3::Forward, blockData.texIdSide, blockData.pass);
	if (ShouldRenderFace(lx, ly, lz, 0, 0, -1)) PushFace(offset + Vector3::Right + Vector3::Forward, Vector3::Up * scaleY, Vector3::Left, blockData.texIdSide, blockData.pass);
	if (ShouldRenderFace(lx, ly, lz, -1, 0, 0)) PushFace(offset + Vector3::Forward, Vector3::Up * scaleY, Vector3::Backward, blockData.texIdSide, blockData.pass);
	if (ShouldRenderFace(lx, ly, lz, 0, 1, 0)) PushFace(offset + Vector3::Up * scaleY, Vector3::Forward, Vector3::Right, blockData.texIdTop, blockData.pass);
	if (ShouldRenderFace(lx, ly, lz, 0, -1, 0)) PushFace(offset + Vector3::Right + Vector3::Forward, Vector3::Left, Vector3::Backward, blockData.texIdBottom, blockData.pass);
}

void Chunk::PushFace(Vector3 pos, Vector3 up, Vector3 right, int texId, ShaderPass pass) {
	Vector2 uv(
		texId % 16,
		texId / 16
	);
	Vector3 normal = up.Cross(right);
	normal.Normalize();
	uint32_t bottomLeft = vBuffer[pass].PushVertex(VertexLayout_PositionNormalUV(pos, normal, (uv + Vector2::UnitY) / 16.0f));
	uint32_t bottomRight = vBuffer[pass].PushVertex(VertexLayout_PositionNormalUV(pos + right, normal, (uv + Vector2::One) / 16.0f));
	uint32_t upLeft = vBuffer[pass].PushVertex(VertexLayout_PositionNormalUV(pos + up, normal, uv / 16.0f));
	uint32_t upRight = vBuffer[pass].PushVertex(VertexLayout_PositionNormalUV(pos + up + right, normal, (uv + Vector2::UnitX) / 16.0f));
	iBuffer[pass].PushTriangle(bottomLeft, upLeft, upRight);
	iBuffer[pass].PushTriangle(bottomLeft, upRight, bottomRight);
}
