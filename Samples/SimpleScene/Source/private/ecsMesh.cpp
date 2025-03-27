#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ECS/ecsSystems.h>
#include <flecs.h>
#include <Geometry.h>
#include <RenderEngine.h>
#include <RenderThread.h>
#include <RenderObject.h>

using namespace GameEngine;

void RegisterEcsMeshSystems(flecs::world& world)
{
	static const EntitySystem::ECS::RenderThreadPtr* renderThread = world.get<EntitySystem::ECS::RenderThreadPtr>();

	world.system<EntitySystem::ECS::RenderObjectPtr, const Position>()
		.each([&](EntitySystem::ECS::RenderObjectPtr& renderObject, const Position& position)
		{
			renderObject.ptr->SetPosition(Math::Vector3f(position.x, position.y, position.z), renderThread->ptr->GetMainFrame());
		});

	world.system<EntitySystem::ECS::RenderObjectPtr, const Enable>()
		.each([&](flecs::entity e, EntitySystem::ECS::RenderObjectPtr& renderObject, const Enable& enable)
		{
			if (!enable.enable) GameEngine::Render::RenderEngine::renderer->RemoveRenderObject(renderObject.ptr);
		});
}


