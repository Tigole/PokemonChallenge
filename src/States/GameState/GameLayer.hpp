#ifndef _GAME_LAYER_HPP
#define _GAME_LAYER_HPP 1

#include <Lys/Lys.hpp>

#include "Gameplay/PokemonMove.hpp"


struct KinematicComponent
{
    lys::Vector2f m_Velocity = {0.0f, 0.0f};

    float m_Max_Speed = 5.0f;
    float m_Acceleration = 10.0f;
};

struct PositionComponent
{
    lys::Vector2f m_Pos = {0.0f, 0.0f};
};

struct SpriteComponent
{
    lys::Vector2f m_Size = {50.0f, 50.0f};
    lys::Color m_Color = {};
};

struct CollisionComponent
{
    lys::Vector2f m_Size = {40.0f, 40.0f};
};

struct AIComponent
{
    bool m_Is_Colliding = false;
    float m_Collision_Duration = 0.0f;
};

struct StatsComponent
{
    int m_Level = 5;
    int m_Life_Current = 18;
    int m_Life_Max = 18;
    int m_ATT = 10;
    int m_DEF = 9;
    int m_SPE = 10;
    int m_SPD = 11;
};


struct Player
{
    KinematicComponent m_Kinematic_Component = {};
    PositionComponent m_Position_Component = {};
    SpriteComponent m_Sprite_Component = {};
    CollisionComponent m_Collision_Component = {};


    StatsComponent m_Stats_Component = {};
};

struct Foe
{
    KinematicComponent m_Kinematic_Component = {};
    PositionComponent m_Position_Component = {};
    SpriteComponent m_Sprite_Component = {};
    CollisionComponent m_Collision_Component = {};
    AIComponent m_AI_Component = {};
};

/*struct Projectile
{
    KinematicComponent m_Kinematic_Component = {};
    PositionComponent m_Position_Component = {};
    SpriteComponent m_Sprite_Component = {};
    CollisionComponent m_Collision_Component = {};
};*/


class GameLayer : public lys::Layer
{
public:
    GameLayer();

    lys::LayerForward mt_On_Update(float elapsed_time) override;
    void mt_On_Render(void) override;

private:

    void mt_Draw_Scene(void);
    void mt_Draw_HUD(void);

    bool m_Input_Left = false;
    bool m_Input_Right = false;
    bool m_Input_Up = false;
    bool m_Input_Down = false;

    Player m_Player = {};
    std::vector<Foe> m_Foes = {};

    void mt_Build_Quad_Tree(void);
    void mt_Update_Player(float elapsed_time);
    void mt_Update_Foes(float elapsed_time);
    void mt_Check_Collision(float elapsed_time);
    void mt_Apply_Damages(void);

    int mt_Compute_Damages(int attacker_level, int attacker_power, int defenser_power, const PokemonMove& move) const;

    void mt_Initialize_Player(void);
    void mt_Initialize_Foe(Foe& foe, const lys::Vector2f& position, float max_speed, float acceleration) const;

    lys::LayerForward mt_On_Event_Key_Pressed(const lys::KeyEvent& event) override;
    lys::LayerForward mt_On_Event_Key_Released(const lys::KeyEvent& event) override;
};



#endif // _GAME_LAYER_HPP
