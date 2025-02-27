#pragma once

#include <RenderObject.h>
#include <RenderThread.h>
#include <Vector.h>
#include <Input/InputHandler.h>
#include <Game.h>

namespace GameEngine
{

	class Component {
	protected:
		GameObject* gameObject = nullptr;
		Game* gameEngine = nullptr;
	public:	
		virtual void Update (float dt) {};

		virtual ~Component () {}
	};

	class GameObject final
	{
		std::vector<Component*> components; 
	public:
		GameObject() = default;

	public:
		Render::RenderObject** GetRenderObjectRef() { return &m_RenderObject; }

		void Update (float dt) {
			for (auto component : components) {
				component->Update(dt);
			}
		}

		void AddComponent (Component* component) {
			components.push_back(component);
		} 

		void SetPosition(Math::Vector3f position, size_t frame)
		{
			m_Position = position;

			if (m_RenderObject) [[likely]]
			{
				m_RenderObject->SetPosition(position, frame);
			}
		}

		Math::Vector3f GetPosition()
		{
			return m_Position;
		}

	protected:
		Render::RenderObject* m_RenderObject = nullptr;

		Math::Vector3f m_Position = Math::Vector3f::Zero();
	};

	class SimpleJumpBehavour : public Component {
		Math::Vector3f velocity = Math::Vector3f{0, 0, 0};
		Math::Vector3f axeleration = Math::Vector3f{0, -1, 0};
		float floorcoord = -5;
	public:
		SimpleJumpBehavour (Game* gameEngine, GameObject* gameObject) {
			this->gameEngine = gameEngine;
			this->gameObject = gameObject;
		}

		void Update (float dt) {
			auto pos = gameObject->GetPosition();
			
			if (pos.y <= floorcoord) velocity = -velocity;

			pos = pos + velocity * dt;
			velocity = velocity + axeleration * dt;

			gameObject->SetPosition(pos, gameEngine->RenderFrame());
		};
	};

	class SimpleMovementBehvour : public Component {
		float tmp = 0;
	public:
		SimpleMovementBehvour (Game* gameEngine, GameObject* gameObject) {
			this->gameEngine = gameEngine;
			this->gameObject = gameObject;
		}

		void Update (float dt) {
			tmp += dt;

			auto pos = gameObject->GetPosition();
			pos.y = std::sin(tmp);

			gameObject->SetPosition(pos, gameEngine->RenderFrame());
		}
	};

	class SimpleControledBehvour : public Component {
		float v = 0;
	public:
		SimpleControledBehvour (Game* gameEngine, GameObject* gameObject) {
			this->gameEngine = gameEngine;
			this->gameObject = gameObject;

			Core::g_InputHandler->RegisterCallback("GoUp"  , [&] () { v =  1; });
			Core::g_InputHandler->RegisterCallback("GoDown", [&] () { v = -1; });
		}

		void Update (float dt) {
			auto pos = gameObject->GetPosition();
			pos.y += v * dt;
			v = 0;

			gameObject->SetPosition(pos, gameEngine->RenderFrame());

		}
	};
}