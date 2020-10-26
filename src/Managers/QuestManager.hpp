#ifndef QUESTMANAGER_HPP
#define QUESTMANAGER_HPP

#include "../Quest.hpp"
#include <vector>

class QuestManager
{
    public:
        QuestManager();
        ~QuestManager();

        void update();
        void clear();

        template <typename T, typename... TArgs>
        void addQuest(TArgs&&... arguments)
        {
            T* newQuest(new T(std::forward<TArgs>(arguments)...));
            m_quests.emplace_back(newQuest);
        }

        Quest* getQuest(std::string name);
        std::vector<std::string> getVictims();
        void addVictim(std::string victim);

        void listQuests();

    private:
        std::vector<Quest*> m_quests;
        std::vector<std::string> m_victims;
};

#endif
