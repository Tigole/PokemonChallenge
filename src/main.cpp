#include <iostream>
#include "Lys/Lys.hpp"

using namespace std;


class PokemonChallengeApplication : public lys::Application
{
public:
    PokemonChallengeApplication(int argc, char* argv[]);

private:

    std::size_t mt_Initialize(void) override;
};

PokemonChallengeApplication::PokemonChallengeApplication(int argc, char* argv[]) :
    lys::Application("Pokemon Challenge", lys::CommandLineArguments(argc, argv))
{}

std::size_t PokemonChallengeApplication::mt_Initialize(void) 
{
    return -1;
}

int main(int argc, char** argv)
{
    PokemonChallengeApplication l_Application(argc, argv);

    return l_Application.mt_Run();
}
