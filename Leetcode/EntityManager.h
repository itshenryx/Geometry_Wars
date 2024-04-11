#pragma once

#include <memory>
#include <vector>

#include "Entity.h"
using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;

class EntityManager {
  EntityVec m_entities;
  EntityVec m_toAdd;
  EntityMap m_entitiesMap;
  size_t    m_totalEntities = 0;

public:
  EntityManager();
  
  void update();
  void removeDeadEntities(EntityVec& vec);
  
  std::shared_ptr<Entity> addEntity(const std::string& tag);
  
  const EntityVec& getEntities();
  const EntityVec& getEntities(const std::string & tag);
};