#include "pch.h"

#include "World.h"
#include "PerlinNoise.hpp"

using namespace DirectX::SimpleMath;

float perlinScaleStone = 0.02f;
int perlinOctaveStone = 4;
float perlinHeightStone = 14.0f;
float perlinScaleDirt = 0.07f;
int perlinOctaveDirt = 2;
float perlinHeightDirt = 8.0f;
float waterHeight = 11.0f;

void World::Generate() {
	siv::BasicPerlinNoise<float> perlin;
	const int GLOBAL_SIZE = WORLD_SIZE * Chunk::CHUNK_SIZE;

	for (int z = 0; z < GLOBAL_SIZE; z++) {
		for (int x = 0; x < GLOBAL_SIZE; x++) {
			for (int y = 0; y < GLOBAL_SIZE; y++)
				SetCube(x, y, z, EMPTY);

			int yStone = perlin.octave2D_01(x * perlinScaleStone, z * perlinScaleStone, perlinOctaveStone) * perlinHeightStone;
			int yDirt = yStone + perlin.octave2D_01(x * perlinScaleDirt, z * perlinScaleDirt, perlinOctaveDirt) * perlinHeightDirt;

			for (int y = 0; y < yStone; y++) {
				SetCube(x, y, z, STONE);
			}

			for (int y = yStone; y < yDirt; y++) {
				SetCube(x, y, z, DIRT);
			}

			if ((yDirt + 1) < waterHeight) {
				for (int y = yDirt; y < waterHeight; y++) {
					SetCube(x, y, z, WATER);
				}
			}
			else {
				SetCube(x, yDirt, z, GRASS);
			}

			/*for (int y = 0; y < GLOBAL_SIZE; y++) {
				float test = perlin.octave3D_01(x / (float)GLOBAL_SIZE * 0.8f, y / (float)GLOBAL_SIZE * 0.8f, z / (float)GLOBAL_SIZE * 0.8f, 5);
				if (test >  0.3f && test < 0.6f) {
					SetCube(x, y, z, EMPTY);
				}
			}*/
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
}

void World::CreateMesh(DeviceResources * res) {
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

void World::Draw(DeviceResources* res, ShaderPass pass) {
	cbModel.ApplyToVS(res, 0);

	for (auto& chunk : chunks) {
		Matrix model = chunk.GetLocalMatrix();
		cbModel.data.mModel = model.Transpose();
		cbModel.Update(res);
		chunk.Draw(res, pass);
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
	auto cube = GetCube(gx, gy, gz);
	if (!cube) return;
	*cube = id;
	MarkCubeDirty(gx, gy, gz);
}

Chunk* World::GetChunk(int gx, int gy, int gz) {
	int cx = gx / Chunk::CHUNK_SIZE;
	int cy = gy / Chunk::CHUNK_SIZE;
	int cz = gz / Chunk::CHUNK_SIZE;

	if (cx < 0) return nullptr;
	if (cy < 0) return nullptr;
	if (cz < 0) return nullptr;
	if (cx >= WORLD_SIZE) return nullptr;
	if (cy >= WORLD_SIZE) return nullptr;
	if (cz >= WORLD_SIZE) return nullptr;

	return &chunks[cx + cy * WORLD_SIZE + cz * WORLD_SIZE * WORLD_SIZE];
}

void World::MarkChunkDirty(int gx, int gy, int gz) {
	Chunk* chunk = GetChunk(gx, gy, gz);
	if (chunk) chunk->MarkDirty();
}

void World::MarkCubeDirty(int gx, int gy, int gz) {
	MarkChunkDirty(gx, gy, gz);
	MarkChunkDirty(gx + 1, gy, gz);
	MarkChunkDirty(gx - 1, gy, gz);
	MarkChunkDirty(gx, gy - 1, gz);
	MarkChunkDirty(gx, gy + 1, gz);
	MarkChunkDirty(gx, gy, gz - 1);
	MarkChunkDirty(gx, gy, gz + 1);
}

void World::ShowImGui(DeviceResources* res) {
	ImGui::Begin("World gen");

	ImGui::DragFloat("perlinScaleStone", &perlinScaleStone, 0.01f);
	ImGui::DragInt("perlinOctaveStone", &perlinOctaveStone, 0.1f);
	ImGui::DragFloat("perlinHeightStone", &perlinHeightStone, 0.1f);
	ImGui::DragFloat("perlinScaleDirt", &perlinScaleDirt, 0.01f);
	ImGui::DragInt("perlinOctaveDirt", &perlinOctaveDirt, 0.1f);
	ImGui::DragFloat("perlinHeightDirt", &perlinHeightDirt, 0.1f);

	if (ImGui::Button("Generate!")) {
		Generate();
		CreateMesh(res);
	}

	ImGui::End();
}
