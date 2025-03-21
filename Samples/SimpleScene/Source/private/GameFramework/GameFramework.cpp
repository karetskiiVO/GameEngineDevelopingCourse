// This define is essential for the scripts to expose the ECS components to lua syntax
// There is a task to rework this behavior
#define GAME_FRAMEWORK

#include <Camera.h>
#include <DefaultGeometry.h>
#include <ecsControl.h>
#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ecsFighting.h>
#include <GameFramework/GameFramework.h>
#include <Input/Controller.h>
#include <RenderObject.h>
#include <Timer.h>
#include <RenderEngine.h>
#include <chrono>
#include <ECS/ecsSystems.h>
#include <flecs.h>

using namespace GameEngine;

void GameFramework::Init()
{
	RegisterComponents();
	RegisterSystems();

	flecs::entity cubeControlClone = m_World.entity()
		.set(Position{ -2.f, 0.f, 0.f })
		.set(HitSphere{ 1.0f, 0.f })
		.set(OriginFlag{ false });
	flecs::entity cubeControl = m_World.entity()
		.set(Position{ -2.f, 0.f, 0.f })
		.set(Velocity{ 0.f, 0.f, 0.f })
		.set(Speed{ 10.f })
		.set(FrictionAmount{ 0.9f })
		.set(JumpSpeed{ 10.f })
		.set(Gravity{ 0.f, -9.8065f, 0.f })
		.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
		.set(Bounciness{ 0.3f })
		.set(Enemy{ 100 })
		.set(HitClone{ cubeControlClone })
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) });

	flecs::entity cubeMovingClone = m_World.entity()
		.set(Position{ 2.f, 0.f, 0.f })
		.set(HitSphere{ 1.0f, 0.f })
		.set(OriginFlag{ false });
	flecs::entity cubeMoving = m_World.entity()
		.set(Position{ 2.f, 0.f, 0.f })
		.set(Velocity{ 0.f, 3.f, 0.f })
		.set(Gravity{ 0.f, -9.8065f, 0.f })
		.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
		.set(Bounciness{ 1.f })
		.set(Enemy{ 100 })
		.set(HitClone{ cubeMovingClone })
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() });


	flecs::entity camera = m_World.entity()
		.set(Position{ 0.0f, 12.0f, -10.0f })
		.set(Speed{ 10.f })
		.set(CameraPtr{ Core::g_MainCamera })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) })
		.set(Cannon{ &m_World, 6, 6, 0, 500, 1500 });
}

void GameFramework::RegisterComponents()
{
	// Exposing these components for the lua system
	ECS_META_COMPONENT(m_World, Position);
	ECS_META_COMPONENT(m_World, Velocity);
	ECS_META_COMPONENT(m_World, Gravity);
	ECS_META_COMPONENT(m_World, BouncePlane);
	ECS_META_COMPONENT(m_World, Bounciness);
	ECS_META_COMPONENT(m_World, ShiverAmount);
	ECS_META_COMPONENT(m_World, FrictionAmount);
	ECS_META_COMPONENT(m_World, Speed);
	ECS_META_COMPONENT(m_World, Bullet);
	ECS_META_COMPONENT(m_World, HitSphere);
	ECS_META_COMPONENT(m_World, OriginFlag);
}

void GameFramework::RegisterSystems()
{
	RegisterEcsMeshSystems(m_World);
	RegisterEcsControlSystems(m_World);
	RegisterEcsFightingSystems(m_World);
}

void GameFramework::Update(float dt)
{

}

void RegisterEcsFightingSystems(flecs::world& world) {
	world.system<HitClone, Position>()
	.each([&](flecs::entity, HitClone& hitclone, Position& position) {
		flecs::ref pos = hitclone.clone.get_ref<Position>();
		
		pos->x = position.x;
		pos->y = position.y;
		pos->z = position.z;
	});

	world.system<Cannon, const CameraPtr, const ControllerPtr>()
	.each([&](flecs::entity e, Cannon& canon, const CameraPtr& camera, const ControllerPtr& controller) {
		static auto prev = std::chrono::high_resolution_clock::now();
		auto now = std::chrono::high_resolution_clock::now();
		auto dt = now - prev;
		prev = now;

		canon.currCooldown = std::max(canon.currCooldown - duration_cast<std::chrono::milliseconds>(dt).count(), 0.0f);

		if (controller.ptr->IsPressed("Jump") && (canon.currCooldown < 1e-8)) {
			Math::Vector3f pos = camera.ptr->GetPosition();
			Math::Vector3f vel = camera.ptr->GetViewDir().Normalized() * 30;
			flecs::entity bulletClone = world.entity()
				.set(Position{ pos.x, pos.y, pos.z })
				.set(HitSphere{ 0.1f, 0.f })
				.set(OriginFlag{ true });
			flecs::entity bullet = world.entity()
				.set(Position{ pos.x, pos.y, pos.z })
				.set(Velocity{ vel.x, vel.y, vel.z })
				.set(Gravity{ 0.f, -9.8065f, 0.f })
				.set(Bullet{ 10.f, 1000 })
				.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::SmallCube() })
				.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() })
				.set(HitClone{ bulletClone });
			

			if (canon.magasineCount == 1) {
				canon.currCooldown = canon.bigCooldown;
				canon.magasineCount = canon.magasineCapacity;
			}
			else {
				canon.currCooldown = canon.smallCooldown;
				canon.magasineCount--;
			}
		}
	});

	world.system<HitClone, Enemy, EntitySystem::ECS::RenderObjectPtr>()
	.each([&](flecs::entity entity, HitClone& hitClone, const Enemy&, EntitySystem::ECS::RenderObjectPtr& renderer) {
		flecs::ref hitSphere = hitClone.clone.get_ref<HitSphere>();

		if (hitSphere->hited == 0) return;

		entity.destruct();
		GameEngine::Render::RenderEngine::renderer->RemoveRenderObject(renderer.ptr);

		entity.destruct();
	});

	world.system<Bullet, EntitySystem::ECS::RenderObjectPtr>()
	.each([&](flecs::entity entity, const Bullet& bullet, EntitySystem::ECS::RenderObjectPtr& render) {
		if (bullet.deathtime > 0) return;

		entity.destruct();
		GameEngine::Render::RenderEngine::renderer->RemoveRenderObject(render.ptr);
	});

	/*world.system<Bullet, EntitySystem::ECS::RenderObjectPtr>()
	.each([&](flecs::entity e, Bullet& bullet, EntitySystem::ECS::RenderObjectPtr& render) {
		auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();

		if (now >= bullet.deathtime) {
			e.destruct();
			GameEngine::Render::RenderEngine::renderer->RemoveRenderObject(render.ptr);
		}
	});*/
}
