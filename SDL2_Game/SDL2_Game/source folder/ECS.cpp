#include "ECS/ECS.h"
#include "ECS/GroupLabels.h" 

// Implementation of addGroup, defined here once (and only once).
void Entity::addGroup(Group mGroup)
{
    groupBitSet[mGroup] = true; // Sets the Entity's internal switch to ON for this group.
    manager.AddToGrp(this, mGroup); // Tells the Manager to add the Entity to the group's specialized list.
}