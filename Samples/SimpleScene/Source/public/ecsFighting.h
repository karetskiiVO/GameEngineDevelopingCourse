#pragma once

#include <flecs.h>
#include <functional>
#include <Vector.h>
#include <chrono>

// This define is essential for the scripts to expose the ECS components to lua syntax
// There is a task to rework this behavior
#undef ECS_META_IMPL
#ifndef GAME_FRAMEWORK
#define ECS_META_IMPL EXTERN // Ensure meta symbols are only defined once
#endif

ECS_STRUCT(Cannon, {
	flecs::world* world;

	unsigned magasineCount;
	unsigned magasineCapacity;

	float currCooldown;

	float smallCooldown;
	float bigCooldown;
});

ECS_STRUCT(Bullet, {
	flecs::entity owner;
	float deathtime;
	unsigned dng;
});

ECS_STRUCT(Enemy, {
	unsigned hp;
});

ECS_STRUCT(HitSphere, {
	float radius;
});

ECS_STRUCT(Bonus, {
	uint8_t _;
});

void RegisterEcsFightingSystems(flecs::world& world);