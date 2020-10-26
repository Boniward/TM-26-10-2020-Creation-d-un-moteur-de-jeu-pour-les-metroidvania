#include "EliminationQuest.hpp"

EliminationQuest::EliminationQuest(std::string name, QuestState state, std::string targetId, unsigned int numberToSlay, unsigned int numberSlayed) :
Quest(name, state),
m_targetId(targetId),
m_numberToSlay(numberToSlay),
m_numberSlayed(numberSlayed)
{
}

EliminationQuest::~EliminationQuest()
{

}

bool EliminationQuest::unlock()
{
}

void EliminationQuest::checkProgression(std::string target)
{
    if(target == m_targetId && m_state == 2)
    {
        m_numberSlayed++;
    }
}

bool EliminationQuest::complete()
{
    if(m_numberSlayed >= m_numberToSlay)
    {
        return true;
    }

    return false;
}
