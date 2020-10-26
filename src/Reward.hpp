#ifndef REWARD_HPP
#define REWARD_HPP

#include <string>
#include <iostream>

class Reward
{
    public:
        Reward();
        ~Reward();

        virtual void obtain() = 0;
        virtual void listInfos() = 0;
};

#endif
