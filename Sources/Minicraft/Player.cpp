#include "pch.h"
#include "Player.h"
#include "World.h"
#include <array>
#include <map>

using namespace DirectX;
using ButtonState = DirectX::Mouse::ButtonStateTracker::ButtonState;

std::vector<std::array<int, 3>> Raycast(Vector3 pos, Vector3 dir, float maxDistance) {
	std::map<float, std::array<int, 3>> cubes;

	if (dir.x != 0) {
		float deltaY = dir.y / dir.x;
		float deltaZ = dir.z / dir.x;
		float offsetY = pos.y - pos.x * deltaY;
		float offsetZ = pos.z - pos.x * deltaZ;

		float currentX = (dir.x > 0) ? ceil(pos.x) : floor(pos.x);
		do {
			Vector3 collision = Vector3(
				currentX,
				offsetY + currentX * deltaY,
				offsetZ + currentX * deltaZ
			);
			float dist = Vector3::Distance(pos, collision);
			if (dist > maxDistance) break;
			cubes[dist] = {
				(int)(currentX - ((dir.x < 0) ? 1 : 0)),
				(int)floor(collision.y),
				(int)ceil(collision.z)
			};
			currentX += (dir.x > 0) ? 1 : -1;
		} while (true);
	}

	if (dir.y != 0) {
		float deltaX = dir.x / dir.y;
		float deltaZ = dir.z / dir.y;
		float offsetX = pos.x - pos.y * deltaX;
		float offsetZ = pos.z - pos.y * deltaZ;

		float currentY = (dir.y > 0) ? ceil(pos.y) : floor(pos.y);
		do {
			Vector3 collision = Vector3(
				offsetX + currentY * deltaX,
				currentY,
				offsetZ + currentY * deltaZ
			);
			float dist = Vector3::Distance(pos, collision);
			if (dist > maxDistance) break;
			cubes[dist] = {
				(int)floor(collision.x),
				(int)(currentY - ((dir.y < 0) ? 1 : 0)),
				(int)ceil(collision.z)
			};
			currentY += (dir.y > 0) ? 1 : -1;
		} while (true);
	}

	if (dir.z != 0) {
		float deltaX = dir.x / dir.z;
		float deltaY = dir.y / dir.z;
		float offsetX = pos.x - pos.z * deltaX;
		float offsetY = pos.y - pos.z * deltaY;

		float currentZ = (dir.z > 0) ? ceil(pos.z) : floor(pos.z);
		do {
			Vector3 collision = Vector3(
				offsetX + currentZ * deltaX,
				offsetY + currentZ * deltaY,
				currentZ
			);
			float dist = Vector3::Distance(pos, collision);
			if (dist > maxDistance) break;
			cubes[dist] = {
				(int)floor(collision.x),
				(int)floor(collision.y),
				(int)(currentZ - ((dir.z > 0) ? 1 : 0)),
			};
			currentZ += (dir.z > 0) ? 1 : -1;
		} while (true);
	}

	std::vector<std::array<int, 3>> result;
	for (auto& cube : cubes)
		result.push_back(cube.second);
	return result;
}

std::vector<Vector3> CollisionPoints = {
	{-0.3f, 0.5f, 0},
	{0, 0.5f, -0.3f},
	{0.3f, 0.5f, 0},
	{0, 0.5f, 0.3f},
	{-0.3f, 1.5f, 0},
	{0, 1.5f, -0.3f},
	{0.3f, 1.5f, 0},
	{0, 1.5f, 0.3f}
};

void Player::Update(float dt, const Keyboard::State& kb, const Mouse::State& ms) {
	kbTracker.Update(kb);
	msTracker.Update(ms);

	Vector3 delta = Vector3::Zero;
	if (kb.Z) delta += camera.Forward();
	if (kb.S) delta -= camera.Forward();
	if (kb.Q) delta -= camera.Right();
	if (kb.D) delta += camera.Right();
	delta.y = 0.0f;
	delta.Normalize();
	position += delta * 10.0f * dt;

	// TODO physics
	velocity -= Vector3(0, 0.8f, 0) * dt;

	BlockId* block = world->GetCube(position.x, position.y + velocity.y, position.z);

	if (block) {
		auto& blockData = BlockData::Get(*block);
		if (!(blockData.flags & BF_NO_PHYSICS)) {
			velocity.y = 0.0f;
			position.y -= position.y - round(position.y);
			if (kbTracker.IsKeyPressed(DirectX::Keyboard::Keys::Space))
				velocity.y = 0.3f;
		}
		if (blockData.flags & BF_GRAVITY_WATER) {
			velocity.y *= 0.9f;
			if (kbTracker.IsKeyPressed(DirectX::Keyboard::Keys::Space))
				velocity.y = 0.3f;
		}
	}

	for (auto& collisionPoint : CollisionPoints) {
		Vector3 colPos = position + velocity + collisionPoint;

		BlockId* block = world->GetCube(colPos.x, colPos.y, colPos.z + 1);

		if (block) {
			auto& blockData = BlockData::Get(*block);
			if (!(blockData.flags & BF_NO_PHYSICS)) {
				if(collisionPoint.z == 0)
					position.x -= colPos.x - round(colPos.x);
				else if (collisionPoint.x == 0)
					position.z -= colPos.z - round(colPos.z);
				else
					position.y -= colPos.y - round(colPos.y);
			}
		}
	}

	position += velocity;


	if (msTracker.leftButton == ButtonState::PRESSED) {
		auto cubes = Raycast(camera.GetPosition(), camera.Forward(), 5);
		for (auto& cube : cubes) {
			BlockId* block = world->GetCube(cube[0], cube[1], cube[2]);
			if (block) {
				auto& blockData = BlockData::Get(*block);

				if (blockData.flags & BF_NO_RAYCAST)
					continue;

				world->SetCube(cube[0], cube[1], cube[2], EMPTY);
				break;
			}
		}
	}


	//delta = Vector3::TransformNormal(delta, camera.GetInverseViewMatrix());

	pitch -= ms.x * dt * 0.2f;
	yaw -= ms.y * dt * 0.2f;
	yaw = std::clamp(yaw, -1.4f, 1.4f);
	Quaternion rot = Quaternion::CreateFromAxisAngle(Vector3::Right, yaw);
	rot *= Quaternion::CreateFromAxisAngle(Vector3::Up, pitch);


	camera.SetPosition(position + Vector3(0, 1.5, 0));
	camera.SetRotation(rot);
}
