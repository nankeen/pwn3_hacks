#include <iostream>
#include <dlfcn.h>
#include <gameLogic.hh>

#include "flying_cheat.hh"

const static std::string module_name = "fly_hacks";

void FlyingCheat::on_game_tick(ClientWorld *world) {
    Player *player = (Player *)(world->m_activePlayer.m_object); 
    player->m_jumpSpeed = 1337;
    player->m_jumpHoldTime = 0xdef00d;
}

// Returns true if the flying cheat is enabled
// Call the base function other wise
bool Player::CanJump()
{
    auto cheat = cheats.find(module_name);
    if (cheat == cheats.end()) {
        std::cerr << "Attempt to bind fly hacks without registering module." << std::endl;
        exit(1);
    }
    std::cout << "Got fly hacks module" << std::endl;

    // Always allow when cheat is enabled
    if (cheat->second->active) {
        return true;
    }

    // Call the original can jump function
    void *can_jump = dlsym(RTLD_NEXT, "Player::CanJump");
    if (can_jump == NULL) {
        std::cerr << "Could not locate original jump function" << std::endl;
    }
    return false;
}
