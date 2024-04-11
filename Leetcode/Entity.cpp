#pragma once
#include "Entity.h"


Entity::Entity(size_t id, const std::string& tag):m_id(id), m_tag(tag) {}

void Entity::destroy() {
  m_active = false;
}

bool Entity::isActive() const {
  return m_active;
}

const std::string& Entity::getTag() const{
  return m_tag;
}

size_t Entity::getId() const{
  return m_id;
}
