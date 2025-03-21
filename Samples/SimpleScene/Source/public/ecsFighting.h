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
	float deathtime;
	float dmg;
});

ECS_STRUCT(Owned, {
	flecs::entity owner;
});

ECS_STRUCT(Enemy, {
	unsigned hp;
});

ECS_STRUCT(HitSphere, {
	float r;
	float hited;
});

ECS_STRUCT(HitClone, {
	flecs::entity clone;
});

ECS_STRUCT(OriginFlag, { 
	float value;
});

void RegisterEcsFightingSystems(flecs::world& world);