#include "PokemonChallengeApp.hpp"
#include "States/StateGame.hpp"

PokemonChallengeApplication::PokemonChallengeApplication(int argc, char* argv[]) :
    lys::Application("Pokemon Challenge", lys::CommandLineArguments(argc, argv))
{}

std::size_t PokemonChallengeApplication::mt_Initialize(void)
{
    m_State_Manager.mt_Add_State(0, new GameState);

    return 0;
}
