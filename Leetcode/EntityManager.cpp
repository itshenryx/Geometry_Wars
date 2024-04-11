#pragma once
#include "EntityManager.h"

EntityManager::EntityManager() = default;

void EntityManager::update() {
  //Add entities from prev frame;
  for (auto& e : m_toAdd) {
    m_entities.push_back(e);
    m_entitiesMap[e->getTag()].push_back(e);
  }

  m_toAdd.clear();

  //Remove previous frame's dead entities; 
  removeDeadEntities(m_entities);

  for (auto& [tag,entityVec] : m_entitiesMap) {
    removeDeadEntities(entityVec);
  }
}

bool shouldRemove(std::shared_ptr<Entity> e) {
  return !e->isActive();
}

void EntityManager::removeDeadEntities(EntityVec& vec) {
  vec.erase(std::remove_if(vec.begin(), vec.end(), shouldRemove),vec.end());
}

const EntityVec& EntityManager::getEntities() {
  return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag) {
  return m_entitiesMap[tag];
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag) {
  auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

  m_toAdd.push_back(e);
  return e;
}
