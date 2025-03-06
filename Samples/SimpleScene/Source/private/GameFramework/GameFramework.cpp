#include <Camera.h>
#include <DefaultGeometry.h>
#include <ecsControl.h>
#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ecsFighting.h>
#include <GameFramework/GameFramework.h>
#include <Input/Controller.h>
#include <RenderObject.h>
#include <deltatime.h>
#include <Timer.h>
#include <RenderEngine.h>
#include <chrono>

using namespace GameEngine;

void GameFramework::Init()
{
	RegisterEcsMeshSystems(m_World);
	RegisterEcsControlSystems(m_World);
	RegisterEcsFightingSystems(m_World);
	RegisterEcsPhysSystems(m_World);

	flecs::entity cubeControl = m_World.entity()
		.set(Position{ Math::Vector3f(-2.f, 0.f, 0.f) })
		.set(Velocity{ Math::Vector3f(0.f, 0.f, 0.f) })
		.set(Speed{ 10.f })
		.set(FrictionAmount{ 0.9f })
		.set(JumpSpeed{ 10.f })
		.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
		.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
		.set(Bounciness{ 0.3f })
		.set(HitSphere{ 1.0f })
		.set(Enemy{})
		.set(GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(RenderObjectPtr{ new Render::RenderObject() })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) });

	flecs::entity cubeMoving = m_World.entity()
		.set(Position{ Math::Vector3f(2.f, 0.f, 0.f) })
		.set(Velocity{ Math::Vector3f(0.f, 3.f, 0.f) })
		.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
		.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
		.set(Bounciness{ 1.f })
		.set(HitSphere{ 1.0f })
		.set(Enemy{})
		.set(Bonus{})
		.set(GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(RenderObjectPtr{ new Render::RenderObject() });

	flecs::entity camera = m_World.entity()
		.set(Position{ Math::Vector3f(0.0f, 12.0f, -10.0f) })
		.set(Speed{ 10.f })
		.set(CameraPtr{ Core::g_MainCamera })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) })
		.set(Cannon{&m_World, 6, 6});
}

void GameFramework::Update(float dt)
{

}

void RegisterEcsFightingSystems(flecs::world& world) {
	world.system<Cannon, const CameraPtr, const ControllerPtr>()
		.each([&](flecs::entity e, Cannon& canon, const CameraPtr& camera, const ControllerPtr& controller) {
		static auto prev = std::chrono::high_resolution_clock::now();
		auto now = std::chrono::high_resolution_clock::now();
		auto dt = now - prev;
		prev = now;

		canon.currCooldown = std::max(canon.currCooldown - duration_cast<std::chrono::milliseconds>(dt).count(), 0.0f);

		if (controller.ptr->IsPressed("Jump") && (canon.currCooldown < 1e-8)) {
			flecs::entity bullet = world.entity()
				.set(Position{ camera.ptr->GetPosition() })
				.set(Velocity{ camera.ptr->GetViewDir().Normalized() * 50 })
				.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
				.set(Bullet{ e, std::chrono::high_resolution_clock::now() + std::chrono::seconds(10) })
				.set(GeometryPtr{ RenderCore::DefaultGeometry::SmallCube() })
				.set(HitSphere{ 0.1f })
				.set(RenderObjectPtr{ new Render::RenderObject() });

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

	world.system<Bullet, Position, HitSphere>()
	.each([&](flecs::entity ebullet, Bullet& bullet, Position& bulletpos, HitSphere& bulletSphere) {
		auto newdeathtime = std::chrono::high_resolution_clock::now() + std::chrono::seconds(1);
		
		world.each([&](flecs::entity eobj, Enemy&, Position& pos, HitSphere& objSphere, RenderObjectPtr& render) {
			if (eobj.has<Bonus>()) {
				flecs::ref cannon = bullet.owner.get_ref<Cannon>();
				cannon->magasineCount = std::min(cannon->magasineCount + 3, cannon->magasineCapacity);
			}

			if ((pos.value - bulletpos.value).GetLength() < bulletSphere.radius + objSphere.radius) {
				bullet.deathtime = std::min(bullet.deathtime, newdeathtime);

				eobj.destruct();
				GameEngine::Render::RenderEngine::renderer->RemoveRenderObject(render.ptr);
			}
		});
	});

	world.system<Bullet, RenderObjectPtr>()
	.each([&](flecs::entity e, Bullet& bullet, RenderObjectPtr& render) {
		auto now = std::chrono::high_resolution_clock::now();

		if (now >= bullet.deathtime) {
			e.destruct();
			GameEngine::Render::RenderEngine::renderer->RemoveRenderObject(render.ptr);
		}
	});
}