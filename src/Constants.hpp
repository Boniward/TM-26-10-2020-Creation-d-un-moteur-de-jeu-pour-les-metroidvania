#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

const unsigned int FPS(60);
const unsigned int TIME_BETWEEN_FRAMES(1000/FPS);
const int TILE_SIZE(32);
const int SCALE(5);

enum EntityType
{
    BACKGROUD_TYPE,
    GATE_TYPE,
    ENEMY_TYPE,
    TRAP_TYPE,
    PLAYER_TYPE,
    UI_TYPE
};

enum SpriteType
{
    STATIC_SPRITE,
    LOOP_SPRITE,
    LINEAR_SPRITE
};

#endif
