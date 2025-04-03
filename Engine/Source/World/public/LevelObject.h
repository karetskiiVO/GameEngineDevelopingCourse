#pragma once

#include <map>
#include <World/export.h>

using Id = size_t;
namespace GameEngine::World
{
	class WORLD_API LevelObject final
	{
	public:
		using ComponentName = std::string;
		using ComponentDesc = std::string;
		struct Component {
			
			Component (const ComponentName& name = "UnnamedComponent", const ComponentDesc& desc = "") 
				: first(name), second(desc)
			{
				static Id idcnt = 0;
				id = idcnt++;
			}

			ComponentName first;
			ComponentDesc second;

			Id GetId() const { return id; }
		private:
			Id id;
		};
		using ComponentList = std::map<Id, Component>;

	public:
		LevelObject() {
			static Id idcnt = 0;
			id = idcnt++;
		}

		~LevelObject() = default;

	public:
		void SetName(const char* name) { m_Name = name; }
		const std::string& GetName() const { return m_Name; }
		Id AddComponent(const ComponentName& name, const ComponentDesc& desc) {
			auto newComponent = Component(name, desc);
			m_ComponentList[newComponent.GetId()] = newComponent;
			return newComponent.GetId();
		}
		const ComponentList& GetComponents() const { return m_ComponentList; }
		ComponentList& GetComponents() { return m_ComponentList; }
		Id GetId() const { return id; }
		Component* GetComponent(Id id) {
			auto it = m_ComponentList.find(id);
			return (it == m_ComponentList.end()) ? nullptr : &(it->second);
		}

	private:
		Id id;
		std::string m_Name = "None";
		ComponentList m_ComponentList;
	};
}
