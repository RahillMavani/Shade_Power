#pragma once
// --- MODIFICATION LOG (ECS Core) ---
// 1. Decoupled Time: Component, Entity, and Manager 'update()' signatures accept 'float dt'.
// 2. Code Quality: Added initializer lists to the Entity constructor to ensure safe values.
// 3. Structural Fix: The implementation of 'addGroup' has been moved out of this header file
//    and placed in the ECS.cpp source file to resolve C2084 linker conflicts.

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>


class Component; // Forward declaration
class Entity;    // Forward declaration
class Manager;   // Forward declaration

// Defining a ID system.
using ComponentID = std::size_t; //unique number assigned to each component TYPE.
using Group = std::size_t;       //unique number assigned to each group (e.g., Players = 1).

// Give an ID to every component created.
inline ComponentID getNewComponentTypeId()
{
	static ComponentID lastID = 0u; // Static counter that increments every time a new component type is requested.
	return lastID++;
}

// Template function to get the unique ID for a specific component type (T).
template <typename T> inline ComponentID getComponentTypeId() noexcept
{
	static ComponentID typeID = getNewComponentTypeId();
	return typeID;
}

// Saving memory by adding a max limit to it (32 lol).
constexpr std::size_t maxComponents = 32; // Maximum number of unique component types allowed.
constexpr std::size_t maxGroups = 32;       // Maximum number of groups allowed.

// A row of 32 switches (bits) used for quick checking if an Entity has a component.
using ComponentBitSet = std::bitset<maxComponents>;
// A row of 32 switches (bits) used for quick checking which group an Entity belongs to.
using GroupBitSet = std::bitset<maxGroups>;
// An array for quick lookup of component pointers by their ID.
using ComponentArray = std::array<Component*, maxComponents>;


class Component
{
public:
	Entity* entity; // Pointer back to the owning Entity (allows component to access other components).

	virtual void init() {}     // Runs once when the component is created and attached.
	virtual void update(float dt) {} // Runs every frame (takes delta time 'dt' directly).
	virtual void draw() {}     // Runs every frame for rendering visuals.

	virtual ~Component() {}    // Virtual destructor for proper memory cleanup.
};

class Entity
{
private:

	Manager& manager; // Reference to the central game Manager.
	bool active = true; // State flag: true if the entity is currently active in the game.
	std::vector<std::unique_ptr<Component>> components; // List of all components owned by this entity.

	ComponentArray componentArray; // Quick array for retrieving component pointers.
	ComponentBitSet componentBitSet; // Switches for component lookup (fast 'hasComponent' check).
	GroupBitSet groupBitSet;         // Switches for group membership.

public:

	// Constructor: Initializes the Entity and uses a list to safely start internal data structures.
	Entity(Manager& mManager) : manager(mManager), active(true), componentArray{}, componentBitSet{}, groupBitSet{}
	{
	}

	void update(float dt) // Runs every frame, passing the time elapsed.
	{
		for (auto& c : components)c->update(dt); // Calls update on all components owned by this entity.
	}

	void draw()
	{
		for (auto& c : components)c->draw(); // Calls draw on all components owned by this entity.
	}

	bool isActive() { return active; } // Getter: checks if the entity is currently alive.
	void destroy() { active = false; } // Marks the entity for deletion (removed by Manager::refresh).

	// Checks the group bitset to see if the entity belongs to the requested group.
	bool hasGroup(Group mGroup)
	{
		return groupBitSet[mGroup];
	}

	// DECLARATION ONLY: Implementation is in ECS.cpp
	void addGroup(Group mGroup);

	void delGroup(Group mGroup)
	{
		groupBitSet[mGroup] = false; // Turns off the switch for this group.
	}

	// Fast check: uses the bitset to see if the Entity has a component of type T.
	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeId<T>()];
	}

	// The primary function to create and attach a component.
	template <typename T, typename... TArgs > T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...)); // Creates the new component (T) with arguments.
		c->entity = this; // Sets the component's back-pointer to its owner.
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr)); // Adds component to the Entity's list.

		this->componentArray[getComponentTypeId<T>()] = c; // Updates the fast array lookup.
		this->componentBitSet[getComponentTypeId<T>()] = true; // Turns on the component switch.

		c->init(); // Runs the component's initialization logic.
		return *c;
	}

	// Retrieves a component of type T using the fast component array.
	template<typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeId<T>()]);
		return *static_cast<T*>(ptr); // Safely casts the generic pointer back to its original type (T).
	}
};

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities; // The master list of all Entities.
	std::array<std::vector<Entity*>, maxGroups> groupedEntities; // Specialized lists for fast group access.

public:
	void update(float dt) // Master update call for the entire world.
	{
		for (auto& e : entities) e->update(dt); // Tells every entity to update.
	}
	void draw()
	{
		for (auto& e : entities) e->draw(); // Tells every entity to draw.
	}

	// Deletes inactive entities and cleans up group lists.
	void refresh()
	{
		// 1. Clean up group lists (removes entities that are inactive or no longer belong to the group).
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase
			(std::remove_if(std::begin(v), std::end(v), [i](Entity* mEntity)
				{
					return !mEntity->isActive() || !mEntity->hasGroup(i);
				}),
				std::end(v));
		}

		// 2. Clean up the master list (removes entities marked for destruction).
		entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(entities));
	}

	// Adds an Entity's pointer to the Manager's specific group list.
	void AddToGrp(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	// Used by Systems (like CollisionSystem) to get the list of entities belonging to a group.
	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}

	// The Entity Factory: Creates a new entity and returns a reference to it.
	Entity& addEntity()
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}

};