#pragma once

#include "Block.h"
#include "Cube.h"
#include "Chunk.h"
#include <array>

class World {
	constexpr static int WORLD_SIZE = 16;
	std::array<Chunk, WORLD_SIZE * WORLD_SIZE * WORLD_SIZE> chunks;

	struct CubeData {
		Matrix mModel;
	};
	ConstantBuffer<CubeData> cbModel;
public:
	void Generate(DeviceResources* res);
	void Draw(DeviceResources* res);

	BlockId* GetCube(int gx, int gy, int gz);
	void SetCube(int gx, int gy, int gz, BlockId id);
};