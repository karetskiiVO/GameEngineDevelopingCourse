#include <LevelEditor/LevelEditor.h>
#include <LevelEditor/LevelSerializer.h>
#include <LevelEditor/ECS/ecsLevelEditor.h>

#include <Filesystem.h>
#include <ECS/ecsSystems.h>
#include <Parser/WorldParser.h>

#include <imgui.h>
#include <imgui_stdlib.h>

namespace GameEngine
{
	namespace Editor
	{
		LevelEditor::LevelEditor(flecs::world& world) : world(&world)
		{
			m_Level = LevelSerializer::Deserialize(Core::g_FileSystem->GetFilePath("Levels/Main.xml").generic_string());

			for (auto& [_, levelObject] : m_Level->GetLevelObjects())
			{
				flecs::entity entity = world.entity(levelObject.GetName().c_str());

				World::LevelObject::ComponentList& componentList = levelObject.GetComponents();

				auto positionIt = std::ranges::find_if(componentList,
					[](const std::pair<Id, World::LevelObject::Component>& component) { return component.second.first == "Position"; }
				);
				auto geometryIt = std::ranges::find_if(componentList,
					[](const std::pair<Id, World::LevelObject::Component>& component) { return component.second.first == "GeometryPtr"; }
				);

				if (positionIt != componentList.end() && geometryIt != componentList.end()) {
					entity.set(EntitySystem::LevelEditorECS::PositionDesc{ levelObject.GetId(), positionIt->second.GetId() });
					entity.set(EntitySystem::EditorECS::Position{ 0.0f, 0.0f, 0.0f });
					entity.set(GeometryPtr{
						reinterpret_cast<RenderCore::Geometry*>(
							World::WorldParser::GetCustomComponents()[geometryIt->second.second]
							)
						});
				}
			}

			EntitySystem::LevelEditorECS::RegisterLevelEditorEcsSystems(world, *m_Level);
		}

		void LevelEditor::Draw()
		{
			std::map<std::string*, Math::Vector3f> vectorMappers{};
			ImGui::Begin(GetName());

			if (m_Level.has_value()) [[likely]]
			{
				for (auto& [_, levelObject] : m_Level->GetLevelObjects())
				{
					if (ImGui::TreeNode(levelObject.GetName().c_str()))
					{
						for (auto& [_, component] : levelObject.GetComponents())
						{
							if (component.first == "Position") {
								Math::Vector3f& vector = vectorMappers[&component.second];
								sscanf(component.second.c_str(), "%f,%f,%f", &vector.x, &vector.y, &vector.z);
								ImGui::InputFloat3(component.first.c_str(), &vector.x);
							} else {
								ImGui::InputText(component.first.c_str(), &component.second);
							}
						}

						ImGui::TreePop();
					}
				}
			}

			if (ImGui::Button("New obj")) NewObj();

			if (ImGui::Button("Save"))
			{
				m_SaveButtonMessageTimer.Reset();
				m_SaveButtonPressed = true;

				Save();
			}

			if (m_SaveButtonPressed)
			{
				ImGui::SameLine();
				ImGui::Text("Saved!");
			}

			ImGui::End();
			for (auto& [descr, v] : vectorMappers) {
				*descr = std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z);
			}
		}

		void LevelEditor::Update(float dt)
		{
			m_SaveButtonMessageTimer.Tick();

			if (m_SaveButtonMessageTimer.GetTotalTime() > m_TimeToShowSaveButtonMessage)
			{
				m_SaveButtonPressed = false;
			}
		}

		void LevelEditor::Save()
		{
			assert(m_Level.has_value());
			LevelSerializer::Serialize(Core::g_FileSystem->GetFilePath("Levels/Main.xml").generic_string(), m_Level.value());
		}

		void LevelEditor::NewObj() {
			static size_t cnt = 0;
			World::LevelObject obj;

			obj.SetName(("Cube" + std::to_string(cnt++)).c_str());
			obj.AddComponent("Position", "0.0,0.0,0.0");
			obj.AddComponent("GeometryPtr", "Cube");

			m_Level->AddLevelObject(obj);

			flecs::entity entity = world->entity(obj.GetName().c_str());
			World::LevelObject::ComponentList& componentList = obj.GetComponents();

			auto positionIt = std::ranges::find_if(componentList,
				[](auto& componentPair) { return componentPair.second.first == std::string("Position"); }
			);
			auto geometryIt = std::ranges::find_if(componentList,
				[](auto& componentPair) { return componentPair.second.first == std::string("GeometryPtr"); }
			);

			if (positionIt != componentList.end() && geometryIt != componentList.end()) {
				entity.set(EntitySystem::LevelEditorECS::PositionDesc{ obj.GetId(), positionIt->second.GetId() });
				entity.set(EntitySystem::EditorECS::Position{ 0.0f, 0.0f, 0.0f });
				entity.set(GeometryPtr{
					reinterpret_cast<RenderCore::Geometry*>(World::WorldParser::GetCustomComponents()[geometryIt->second.second])
					});
			}
		}
	}
}