#include <iostream>
#include <dlfcn.h>
#include <gameLogic.hh>
#include <type_traits>
#include <X11/keysym.h>

#include "cheat.hh"
#include "flying_cheat.hh"

const static std::string module_name = "fly_hacks";

/*
void FlyingCheat::on_world_tick(ClientWorld *world) {
    if (!this->active) {
        return;
    }

    Player *player = (Player *)(world->m_activePlayer.m_object); 

    this->m_jumpSpeed = player->m_jumpSpeed;
    this->m_jumpHoldTime = player->m_jumpHoldTime;

    player->m_jumpSpeed = 1337;
    player->m_jumpHoldTime = 0xdef00d;
}
*/

void FlyingCheat::enable()
{
    this->active = true;
    // Get the game world object
    ClientWorld *world = *((ClientWorld**)(dlsym(RTLD_NEXT, "GameWorld")));
    if (world == NULL) {
        std::cerr << "Couldn't locate 'GameWorld' symbol" << std::endl;
        return;
    }

    Player *player = (Player *)(world->m_activePlayer.m_object); 

    this->m_jumpSpeed = player->m_jumpSpeed;
    this->m_jumpHoldTime = player->m_jumpHoldTime;

    player->m_jumpSpeed = 13337;
    player->m_jumpHoldTime = 0xdef00d;
    
    std::cout << module_name << " enabled" << std::endl;
}

void FlyingCheat::disable()
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
    player->m_jumpSpeed = this->m_jumpSpeed;
    player->m_jumpHoldTime = this->m_jumpHoldTime;

    std::cout << module_name << " disabled" << std::endl;
}

// Keyboard combination to activate cheat
bool FlyingCheat::check_toggle(Display *display)
{
    char keys_return[32];

    XQueryKeymap(display, keys_return);
    // Control_L + F
    return check_key(keys_return, XK_F) && check_key(keys_return, XK_Control_L);
}

bool Player::CanJump()
{
    // Check if cheat is active
    if (!cheats) {
        std::cerr << "Cheats map not initialized" << std::endl;
        exit(1);
    }
    auto cheat = cheats->find(module_name);
    if (cheat == cheats->end()) {
        std::cerr << "Cheat not registered" << std::endl;
        exit(1);
    }

    // Always allow jump if active
    if (cheat->second->active) {
        return true;
    }

    // Call the original function
    bool (*can_jump)(Player *) = (bool (*)(Player *))dlsym(RTLD_NEXT, "_ZN6Player7CanJumpEv");
    if (can_jump != NULL) {
        return can_jump(this);
    }

    return true;
}
