#ifndef _POKEMON_DEFINITION_HPP
#define _POKEMON_DEFINITION_HPP 1

#include "PokemonTypes.hpp"
#include <cstddef>
#include <unordered_map>
#include <string>

enum class PokemonStatType
{
    HitPoint,
    PhysicalAttack,
    PhysicalDefense,
    SpeciaAttack,
    SpecialDefense,
    COUNT
};

struct PokemonStatStruct
{
    uint16_t m_Hit_Point = 0;
    uint16_t m_Physical_Attack = 0;
    uint16_t m_Physical_Defense = 0;
    uint16_t m_Special_Attack = 0;
    uint16_t m_Special_Defense = 0;
    uint16_t m_Speed = 0;
};

union PokemonStat
{
    PokemonStatStruct m_Struct;
    uint16_t m_Heap[static_cast<std::size_t>(PokemonStatType::COUNT)];
};

struct PokemonFlags
{
    bool m_Burned = false;
};

struct Pokemon
{
    int m_Level = 0;
    uint16_t m_Id = 0;
    PokemonStat m_Current_Stat = {};
    PokemonFlags m_Flags = {};
};

struct PokemonDefinition
{
    static void smt_Populate(void);
    static std::unordered_map<uint16_t, PokemonDefinition> sm_Definitions;
    std::string m_Name = {};
    PokemonStat m_Base_State = {};
    std::array<PokemonType, 2> m_Types = {PokemonType::None, PokemonType::None};
};

#endif // _POKEMON_DEFINITION_HPP
