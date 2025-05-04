#ifndef _STATE_GAME_HPP
#define _STATE_GAME_HPP 1

#include "GameState/GameLayer.hpp"

class GameState : public lys::State
{
public:
    GameState();

private:
    GameLayer m_Game_Layer = {};
};

GameState::GameState() :
    lys::State()
{
    mt_Push_Layer(&m_Game_Layer);
}

#endif // _STATE_GAME_HPP
