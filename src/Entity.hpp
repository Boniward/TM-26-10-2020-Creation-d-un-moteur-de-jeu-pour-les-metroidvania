#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <map>
#include <typeinfo>
#include <iterator>
#include <memory>
#include "Component.hpp"
#include "Constants.hpp"

class Component;

class Entity
{
    public:
        Entity();
        Entity(std::string name);
        ~Entity();

        void update();
        void render();

        void listComponents();
        std::string getName();

        template <typename T, typename... TArgs>
        void addComponent(TArgs&&... arguments)
        {
            if(!hasComponent<T>())
            {
                T* newComponent(new T(this, std::forward<TArgs>(arguments)...));
                m_components.emplace(&typeid(T), newComponent);
            }
        }

        template <typename T>
        T* getComponent()
        {
            return static_cast<T*>(m_components[&typeid(T)]);
        }

        template <typename T>
        bool hasComponent()
        {
            for(m_iterator = m_components.begin(); m_iterator != m_components.end(); m_iterator++)
            {
                if(m_iterator->first == &typeid(T))
                {
                    return true;
                }
            }

            return false;
        }


    private:
        std::string m_name;
        std::map<const std::type_info*, Component*> m_components;
        std::map<const std::type_info*, Component*>::iterator m_iterator;
};

#endif
