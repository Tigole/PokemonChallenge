#ifndef _POKEMON_TYPES_HPP
#define _POKEMON_TYPES_HPP 1

#include <array>

enum class PokemonType
{
    None = 0,
    Normal = 1,
    Fire = 2,
    Water = 3,
    Electric = 4,
    Grass = 5,
    Ice = 6,
    Fighting = 7,
    Poison = 8,
    Ground = 9,
    Flying = 10,
    Psychic = 11,
    Bug = 12,
    Rock = 13,
    Ghost = 14,
    Dragon = 15,
    Steel = 16,
    Dark = 17,
    Fairy = 18,

    COUNT
};

class PokemonTypeTable
{
public:
    static PokemonTypeTable& smt_Get();

    float mt_Get_Factor(PokemonType move_type, PokemonType pokemon_type);

private:
    PokemonTypeTable();

    void mt_Set_Factor(PokemonType move_type, PokemonType pokemon_type, float factor);
    std::array<std::array<float,static_cast<std::size_t>(PokemonType::COUNT)>, static_cast<std::size_t>(PokemonType::COUNT)> m_Look_Up_Table;
};


#endif // _POKEMON_TYPES_HPP
