#include "QuestManager.hpp"

QuestManager::QuestManager()
{
}

QuestManager::~QuestManager()
{
}

void QuestManager::update()
{
    for(auto& quest : m_quests)
    {
        for(std::string target : m_victims)
        {
            quest->checkProgression(target);
        }
        quest->update();
    }
    m_victims.clear();
}

void QuestManager::clear()
{
    for(auto& quest : m_quests)
    {
        delete quest;
    }
    m_quests.clear();
}

Quest* QuestManager::getQuest(std::string name)
{
    for(auto& quest : m_quests)
    {
        if(quest->getName() == name)
        {
            return quest;
        }
    }
}

void QuestManager::listQuests()
{
    unsigned int count(0);
    for(auto& quest : m_quests)
    {
        std::cout << "[" << count << "]Quest : " << quest->getName() << std::endl;
        quest->listRewards();
        std::cout << std::endl;
        count++;
    }
}

std::vector<std::string> QuestManager::getVictims()
{
    return  m_victims;
}

void QuestManager::addVictim(std::string victim)
{
    m_victims.emplace_back(victim);
}
