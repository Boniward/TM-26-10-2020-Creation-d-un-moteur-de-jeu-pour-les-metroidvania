#include "Quest.hpp"

Quest::Quest()
{
}

Quest::Quest(std::string name, QuestState state) : m_name(name), m_state(state)
{
}

Quest::~Quest()
{
}

void Quest::update()
{
    switch(m_state)
    {
        case PENDING:
            if(unlock())
            {
                m_state = UNLOCKED;
            }
            break;
        case UNLOCKED:
            break;
        case IN_PROGRESS:
            if(complete())
            {
                m_state = DONE;
            }
            break;
        case DONE:
            for(auto& reward : m_rewards)
            {
                reward.second->obtain();
            }
            m_state = CANCELLED;
            break;
        case CANCELLED:
            break;
    }
}

void Quest::listRewards()
{
    for(m_iterator = m_rewards.begin(); m_iterator != m_rewards.end(); m_iterator++)
    {
        std::cout << "\tReward<"<< m_iterator->first->name() << "> : ";
        m_iterator->second->listInfos();
    }
}

std::string Quest::getName()
{
    return m_name;
}
