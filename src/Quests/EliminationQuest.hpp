#ifndef ELIMINATIONQUEST_HPP
#define ELIMINATIONQUEST_HPP

#include "../Quest.hpp"

class EliminationQuest : public Quest
{
    public:
        EliminationQuest(std::string name, QuestState state, std::string targetId, unsigned int numberToSlay, unsigned int numberSlayed);
        ~EliminationQuest();

        bool unlock() override;
        void checkProgression(std::string target) override;
        bool complete() override;

        void updateStatus(std::string target);

    private:
        std::string m_targetId;
        unsigned int m_numberToSlay;
        unsigned int m_numberSlayed;
};

#endif
