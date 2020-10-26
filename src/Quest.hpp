#ifndef QUEST_HPP
#define QUEST_HPP

#include <map>
#include <typeinfo>
#include <string>
#include <iterator>

#include "Reward.hpp"

enum QuestState
{
    PENDING,
    UNLOCKED,
    IN_PROGRESS,
    DONE,
    CANCELLED
};

class Quest
{
    public:
        Quest();
        Quest(std::string name, QuestState state);
        ~Quest();

        void update();
        void listRewards();

        virtual bool unlock() = 0;
        virtual void checkProgression(std::string target) = 0;
        virtual bool complete() = 0;

        std::string getName();

        template <typename T, typename... TArgs>
        void addReward(TArgs&&... arguments)
        {
            T* newReward(new T(std::forward<TArgs>(arguments)...));
            m_rewards.emplace(&typeid(T), newReward);
        }

        template <typename T>
        T* getReward()
        {
            return static_cast<T*>(m_rewards[&typeid(T)]);
        }

    protected:
        std::string m_name;
        QuestState m_state;
        std::map<const std::type_info*, Reward*> m_rewards;
        std::map<const std::type_info*, Reward*>::iterator m_iterator;

};

#endif
