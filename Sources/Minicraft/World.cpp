#include "pch.h"

#include "World.h"
#include "PerlinNoise.hpp"

using namespace DirectX::SimpleMath;

void World::Generate(DeviceResources* res) {
	siv::BasicPerlinNoise<float> perlin;
	const int GLOBAL_SIZE = WORLD_SIZE * Chunk::CHUNK_SIZE;
	for (int z = 0; z < GLOBAL_SIZE; z++) {
		for (int y = 0; y < GLOBAL_SIZE; y++) {
			for (int x = 0; x < GLOBAL_SIZE; x++) {
				float test = perlin.octave3D_01(x / (float)GLOBAL_SIZE * 0.8f, y / (float)GLOBAL_SIZE * 0.8f, z / (float)GLOBAL_SIZE * 0.8f, 5);
				if (test >  0.3f && test < 0.6f) {
					*GetCube(x, y, z) = LOG;
				}
			}
		}
	}

	/*for (int z = 0; z < WORLD_SIZE; z++) {
		for (int x = 0; x < WORLD_SIZE; x++) {
			for (int y = 0; y < 3; y++)
				SetCube(x, y, z, STONE);
			for (int y = 3; y < 6; y++)
				SetCube(x, y, z, DIRT);
			SetCube(x, 6, z, GRASS);
		}
	}*/

	for (int z = 0; z < WORLD_SIZE; z++) {
		for (int y = 0; y < WORLD_SIZE; y++) {
			for (int x = 0; x < WORLD_SIZE; x++) {
				chunks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_SIZE].SetPosition(this, x, y, z);
				chunks[x + y * WORLD_SIZE + z * WORLD_SIZE * WORLD_SIZE].Generate(res);
			}
		}
	}
	cbModel.Create(res);
}

void World::Draw(DeviceResources* res) {
	cbModel.ApplyToVS(res, 0);

	for (auto& chunk : chunks) {
		Matrix model = chunk.GetLocalMatrix();
		cbModel.data.mModel = model.Transpose();
		cbModel.Update(res);
		chunk.Draw(res);
	}
}

BlockId* World::GetCube(int gx, int gy, int gz) {
	int cx = gx / Chunk::CHUNK_SIZE;
	int cy = gy / Chunk::CHUNK_SIZE;
	int cz = gz / Chunk::CHUNK_SIZE;
	int lx = gx % Chunk::CHUNK_SIZE;
	int ly = gy % Chunk::CHUNK_SIZE;
	int lz = gz % Chunk::CHUNK_SIZE;

	if (cx < 0) return nullptr;
	if (cy < 0) return nullptr;
	if (cz < 0) return nullptr;
	if (cx >= WORLD_SIZE) return nullptr;
	if (cy >= WORLD_SIZE) return nullptr;
	if (cz >= WORLD_SIZE) return nullptr;

	return chunks[cx + cy * WORLD_SIZE + cz * WORLD_SIZE * WORLD_SIZE].GetChunkCube(lx, ly, lz);
}


void World::SetCube(int gx, int gy, int gz, BlockId id) {
	/*if (gx < 0) return;
	if (gy < 0) return;
	if (gz < 0) return;
	if (gx >= WORLD_SIZE) return;
	if (gy >= WORLD_SIZE) return;
	if (gz >= WORLD_SIZE) return;
	data[gx + gy * WORLD_SIZE + gz * WORLD_SIZE * WORLD_SIZE] = id;*/
}