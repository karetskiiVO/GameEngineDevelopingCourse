#pragma once

#include <Level.h>
#include <flecs.h>

#include <EditorECS/ecsEditor.h>
#include <LevelObject.h>

namespace GameEngine::EntitySystem::LevelEditorECS
{
	struct PositionDesc
	{
		Id objId;
		Id cmpId;
	};

	void RegisterLevelEditorEcsSystems(flecs::world& world, World::Level& level);
}