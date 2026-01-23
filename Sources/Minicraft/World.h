#pragma once

#include "Block.h"
#include "Cube.h"
#include "Chunk.h"
#include <array>

class Camera;

class World {
	constexpr static int WORLD_SIZE = 64;
	std::array<Chunk, WORLD_SIZE * WORLD_SIZE * WORLD_SIZE> chunks;

	struct CubeData {
		Matrix mModel;
	};
	ConstantBuffer<CubeData> cbModel;
public:
	void Generate();
	void CreateMesh(DeviceResources* res);
	void Draw(DeviceResources* res, Camera* camera, ShaderPass pass);

	BlockId* GetCube(int gx, int gy, int gz);
	void SetCube(int gx, int gy, int gz, BlockId id);

	Chunk* GetChunk(int gx, int gy, int gz);
	void MarkChunkDirty(int gx, int gy, int gz);
	void MarkCubeDirty(int gx, int gy, int gz);

	void ShowImGui(DeviceResources* res);
};