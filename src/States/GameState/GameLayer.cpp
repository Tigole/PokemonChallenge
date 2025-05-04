#include "GameLayer.hpp"
#include "Gameplay/PokemonDefinition.hpp"


#include <Lys/Core/SFML_Conversion.hpp>

/// https://tcrf.net/Development:Pok%C3%A9mon_Red_and_Blue/Unused_Maps

constexpr const char* TOKEN = "KOKO";

lys::Vector2f mt_Move_Toward(const lys::Vector2f& current, const lys::Vector2f& target, float amount)
{
    lys::Vector2f l_Ret = current + (target - current) * amount;

    l_Ret.x = (target.x - current.x > 0.0f) * std::min(l_Ret.x, target.x)
            + (target.x - current.x < 0.0f) * std::max(l_Ret.x, target.x);
    l_Ret.y = (target.y - current.y > 0.0f) * std::min(l_Ret.y, target.y)
            + (target.y - current.y < 0.0f) * std::max(l_Ret.y, target.y);

    return l_Ret;
}

GameLayer::GameLayer() :
    lys::Layer("GameLayer")
{
    mt_Initialize_Player();

    constexpr const float l_Distance_To_Player = 500.0f;
    m_Foes.resize(6);
    for (std::size_t ii = 0; ii < m_Foes.size(); ii++)
    {
        const float l_Angle = lys::fn_Lerp(0.0f, 2 * M_PI, static_cast<float>(ii) / m_Foes.size());

        mt_Initialize_Foe(m_Foes[ii],
                          m_Player.m_Position_Component.m_Pos + lys::Vector2f(cos(l_Angle) * l_Distance_To_Player,
                                                                              sin(l_Angle) * l_Distance_To_Player),
                          m_Player.m_Kinematic_Component.m_Max_Speed * 0.25f,
                          m_Player.m_Kinematic_Component.m_Acceleration * 0.5f);
    }
}

lys::LayerForward GameLayer::mt_On_Update(float elapsed_time)
{
    mt_Build_Quad_Tree();
    mt_Update_Player(elapsed_time);
    mt_Update_Foes(elapsed_time);
    mt_Check_Collision(elapsed_time);
    mt_Apply_Damages();

    return lys::LayerForward::Continue;
}

void GameLayer::mt_On_Render(void)
{
    mt_Draw_Scene();
    mt_Draw_HUD();
}

void GameLayer::mt_Draw_Scene(void)
{
    lys::RectangleSettings l_Rectangle;

    for (std::size_t ii = 0; ii < m_Foes.size(); ii++)
    {
        l_Rectangle.m_Fill_Color = m_Foes[ii].m_Sprite_Component.m_Color;
        l_Rectangle.m_Position = m_Foes[ii].m_Position_Component.m_Pos - m_Foes[ii].m_Sprite_Component.m_Size * 0.5f;
        l_Rectangle.m_Size = m_Foes[ii].m_Sprite_Component.m_Size;

        lys::Renderer::smt_Get().mt_Draw_Rectangle(l_Rectangle);
    }

    l_Rectangle.m_Fill_Color = m_Player.m_Sprite_Component.m_Color;
    l_Rectangle.m_Position = m_Player.m_Position_Component.m_Pos - m_Player.m_Sprite_Component.m_Size * 0.5f;
    l_Rectangle.m_Size = m_Player.m_Sprite_Component.m_Size;

    lys::Renderer::smt_Get().mt_Draw_Rectangle(l_Rectangle);

    if (true)
    {
        l_Rectangle.m_Outline_Color = {0.1f, 0.1f, 0.1f, 0.9f};
        l_Rectangle.m_Outline_Thickness = -3.0f;
        l_Rectangle.m_Fill_Color = lys::Color::Transparent;

        l_Rectangle.m_Position = m_Player.m_Position_Component.m_Pos - m_Player.m_Collision_Component.m_Size * 0.5f;
        l_Rectangle.m_Size = m_Player.m_Collision_Component.m_Size;
        lys::Renderer::smt_Get().mt_Draw_Rectangle(l_Rectangle);

        for (std::size_t ii = 0; ii < m_Foes.size(); ii++)
        {
            l_Rectangle.m_Position = m_Foes[ii].m_Position_Component.m_Pos - m_Foes[ii].m_Collision_Component.m_Size * 0.5f;
            l_Rectangle.m_Size = m_Foes[ii].m_Collision_Component.m_Size;

            lys::Renderer::smt_Get().mt_Draw_Rectangle(l_Rectangle);
        }
    }
}

void GameLayer::mt_Draw_HUD(void)
{
    constexpr const float l_Progress_Bar_Width = 200.0f;
    lys::RectangleSettings l_Rectangle;

    l_Rectangle.m_Size.y = 25.0f;
    l_Rectangle.m_Position = {10.0f, 10.0f};

    auto l_fn_Draw_Progress_Bar = [&](int current, int max, const lys::Color& inner_color, const lys::Color& outer_color)
    {
        l_Rectangle.m_Fill_Color = inner_color;
        l_Rectangle.m_Outline_Thickness = 0.0f;

        l_Rectangle.m_Size.x = l_Progress_Bar_Width * static_cast<float>(current) / max;

        lys::Renderer::smt_Get().mt_Draw_Rectangle(l_Rectangle);

        l_Rectangle.m_Fill_Color = lys::Color::Transparent;
        l_Rectangle.m_Outline_Color = outer_color;
        l_Rectangle.m_Outline_Thickness = 2.0f;

        l_Rectangle.m_Size.x = l_Progress_Bar_Width;

        lys::Renderer::smt_Get().mt_Draw_Rectangle(l_Rectangle);
    };

    l_fn_Draw_Progress_Bar(m_Player.m_Stats_Component.m_Life_Current, m_Player.m_Stats_Component.m_Life_Max, {0.9f, 0.1f, 0.1f}, {0.6f, 0.6f, 0.2f});
}

void GameLayer::mt_Build_Quad_Tree(void)
{
    //
}

void GameLayer::mt_Update_Player(float elapsed_time)
{
    const lys::Vector2f l_Input_Vector(m_Input_Right - m_Input_Left, m_Input_Down - m_Input_Up);

    m_Player.m_Kinematic_Component.m_Velocity = mt_Move_Toward(m_Player.m_Kinematic_Component.m_Velocity,
                                                               l_Input_Vector.mt_Get_Normalized() * m_Player.m_Kinematic_Component.m_Max_Speed,
                                                               elapsed_time * m_Player.m_Kinematic_Component.m_Acceleration);

    m_Player.m_Position_Component.m_Pos += m_Player.m_Kinematic_Component.m_Velocity;
}

void GameLayer::mt_Update_Foes(float elapsed_time)
{
    for (std::size_t ii = 0; ii < m_Foes.size(); ii++)
    {
        const lys::Vector2f l_Direction =   (m_Foes[ii].m_AI_Component.m_Is_Colliding == true) * lys::Vector2f(0.0f)
                                        +   (m_Foes[ii].m_AI_Component.m_Is_Colliding == false) * (m_Player.m_Position_Component.m_Pos - m_Foes[ii].m_Position_Component.m_Pos);

        m_Foes[ii].m_Kinematic_Component.m_Velocity = mt_Move_Toward(m_Foes[ii].m_Kinematic_Component.m_Velocity,
                                                                   l_Direction.mt_Get_Normalized() * m_Foes[ii].m_Kinematic_Component.m_Max_Speed,
                                                                   elapsed_time * m_Foes[ii].m_Kinematic_Component.m_Acceleration);

        m_Foes[ii].m_Position_Component.m_Pos += m_Foes[ii].m_Kinematic_Component.m_Velocity;
    }
}

void GameLayer::mt_Check_Collision(float elapsed_time)
{
    const sf::FloatRect l_Player(lys::sf_To<float>(m_Player.m_Position_Component.m_Pos - m_Player.m_Collision_Component.m_Size * 0.5f), lys::sf_To<float>(m_Player.m_Collision_Component.m_Size));
    sf::FloatRect l_Intersection_Area;

    for (std::size_t ii = 0; ii < m_Foes.size(); ii++)
    {
        const sf::FloatRect l_Foe(lys::sf_To<float>(m_Foes[ii].m_Position_Component.m_Pos - m_Foes[ii].m_Collision_Component.m_Size * 0.5f), lys::sf_To<float>(m_Foes[ii].m_Collision_Component.m_Size));
        m_Foes[ii].m_AI_Component.m_Is_Colliding = false;
        if (l_Foe.intersects(l_Player, l_Intersection_Area) == true)
        {
            AIComponent& l_AI = m_Foes[ii].m_AI_Component;
            l_AI.m_Is_Colliding = ((l_Intersection_Area.width * l_Intersection_Area.height) / (l_Foe.width * l_Foe.height)) > 0.25f;

            l_AI.m_Collision_Duration = (m_Foes[ii].m_AI_Component.m_Is_Colliding == true) * (l_AI.m_Collision_Duration + elapsed_time);
        }
    }
}

void GameLayer::mt_Apply_Damages(void)
{
    for (std::size_t ii = 0; ii < m_Foes.size(); ii++)
    {
        AIComponent& l_AI = m_Foes[ii].m_AI_Component;
        m_Player.m_Stats_Component.m_Life_Current -= (l_AI.m_Is_Colliding) * (l_AI.m_Collision_Duration > 0.5f) * mt_Compute_Damages(5, 50, 10, PokemonMove{10, PokemonType::Bug});
        l_AI.m_Collision_Duration = l_AI.m_Collision_Duration * (l_AI.m_Collision_Duration <= 0.5f);
    }
}

int GameLayer::mt_Compute_Damages(int attacker_level, int attacker_power, int defenser_power, const PokemonMove& move) const
{
    const float l_Level = (2 * attacker_level) / 5 + 2;
    const float l_Main = (l_Level * move.m_Power * attacker_power / defenser_power) / 50.0f + 2.0f;
    constexpr const float l_Targets_Factor = 1.0f;
    constexpr const float l_Weather_Factor = 1.0;
    constexpr const float l_Critical_Factor = 1.0f;
    constexpr const float l_Random_Factor = 1.0f;
    const float l_STAB_Factor = fn_Compute_STAB_Factor(move.m_Type, PokemonDefinition::sm_Definitions[attacker.m_Id].m_Types);
    const float l_Type_Factor = PokemonTypeTable::smt_Get().mt_Get_Factor(move.m_Type, PokemonDefinition::sm_Definitions[defender.m_Id].m_Types[0]) * PokemonTypeTable::smt_Get().mt_Get_Factor(move.m_Type, PokemonDefinition::sm_Definitions[defender.m_Id].m_Types[1]);
    constexpr const float l_Burn_Factor = 1.0f;
    const float l_Other_Factor = fn_Compute_Burn_Factor(attacker.m_Flags.m_Burned);
    const int l_Ret = l_Main * l_Targets_Factor * l_Weather_Factor * l_Critical_Factor * l_Random_Factor * l_STAB_Factor * l_Type_Factor * l_Burn_Factor * l_Other_Factor;

    return l_Ret;
}

void GameLayer::mt_Initialize_Player(void)
{
    m_Player.m_Sprite_Component.m_Color = {0.4f, 0.8f, 0.6f};
    m_Player.m_Position_Component.m_Pos = lys::Window::smt_Get().mt_Get_Size() * 0.5f;
    m_Player.m_Kinematic_Component.m_Max_Speed = 1.0f;
}

void GameLayer::mt_Initialize_Foe(Foe& foe, const lys::Vector2f& position, float max_speed, float acceleration) const
{
    foe.m_Position_Component.m_Pos = position;
    foe.m_Kinematic_Component.m_Max_Speed = max_speed;
    foe.m_Kinematic_Component.m_Acceleration = acceleration;
    foe.m_Sprite_Component.m_Color = {0.8f, 0.4f, 0.4f};
}

lys::LayerForward GameLayer::mt_On_Event_Key_Pressed(const lys::KeyEvent& event)
{
    m_Input_Left |= event.m_Key.code == sf::Keyboard::Q;
    m_Input_Right |= event.m_Key.code == sf::Keyboard::D;
    m_Input_Up |= event.m_Key.code == sf::Keyboard::Z;
    m_Input_Down |= event.m_Key.code == sf::Keyboard::S;

    return lys::LayerForward::Stop;
}

lys::LayerForward GameLayer::mt_On_Event_Key_Released(const lys::KeyEvent& event)
{
    m_Input_Left ^= event.m_Key.code == sf::Keyboard::Q;
    m_Input_Right ^= event.m_Key.code == sf::Keyboard::D;
    m_Input_Up ^= event.m_Key.code == sf::Keyboard::Z;
    m_Input_Down ^= event.m_Key.code == sf::Keyboard::S;

    return lys::LayerForward::Stop;
}

