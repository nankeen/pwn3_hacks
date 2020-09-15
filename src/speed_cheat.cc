#include <iostream>
#include <dlfcn.h>
#include <gameLogic.hh>
#include <X11/keysym.h>

#include "speed_cheat.hh"

const static std::string module_name = "speed_hacks";

void SpeedCheat::enable()
{
    this->active = true;
    // Get the game world object
    ClientWorld *world = *((ClientWorld**)(dlsym(RTLD_NEXT, "GameWorld")));
    if (world == NULL) {
        std::cerr << "Couldn't locate 'GameWorld' symbol" << std::endl;
        return;
    }

    Player *player = (Player *)(world->m_activePlayer.m_object); 

    this->m_walkingSpeed = player->m_walkingSpeed;

    player->m_walkingSpeed = 13337;

    std::cout << module_name << " enabled" << std::endl;
}

void SpeedCheat::disable()
{
    this->active = false;
    // Get the game world object
    ClientWorld *world = *((ClientWorld**)(dlsym(RTLD_NEXT, "GameWorld")));
    if (world == NULL) {
        std::cerr << "Couldn't locate 'GameWorld' symbol" << std::endl;
        return;
    }

    // Reset
    Player *player = (Player *)(world->m_activePlayer.m_object); 
    player->m_walkingSpeed = this->m_walkingSpeed;

    std::cout << module_name << " disabled" << std::endl;
}

// Keyboard combination to activate cheat
bool SpeedCheat::check_toggle(Display *display)
{
    char keys_return[32];

    XQueryKeymap(display, keys_return);
    // Control_L + V
    return check_key(keys_return, XK_V) && check_key(keys_return, XK_Control_L);
}
