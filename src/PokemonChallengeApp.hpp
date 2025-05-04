#ifndef _POKEMON_CHALLENGE_APP_HPP
#define _POKEMON_CHALLENGE_APP_HPP 1

#include "Lys/Lys.hpp"

class PokemonChallengeApplication : public lys::Application
{
public:
    PokemonChallengeApplication(int argc, char* argv[]);

private:

    std::size_t mt_Initialize(void) override;
};

#endif // _POKEMON_CHALLENGE_APP_HPP
