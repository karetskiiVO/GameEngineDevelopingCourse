#pragma once

#include <flecs.h>
#include <functional>
#include <Vector.h>
#include <chrono>

struct Cannon {
	flecs::world* world;

	unsigned magasineCount;
	unsigned magasineCapacity;

	float currCooldown = 0;

	float smallCooldown = 100;
	float bigCooldown = 1500;
};

struct Bullet {
	flecs::entity owner;
	std::chrono::steady_clock::time_point deathtime;
	unsigned dng = 100;
};

struct Enemy {
	unsigned hp = 100;
};

struct HitSphere {
	float radius;
};

struct Bonus {
private:
	uint8_t val;
};

void RegisterEcsFightingSystems(flecs::world& world);