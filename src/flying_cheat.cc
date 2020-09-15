#include <iostream>
#include <dlfcn.h>
#include <gameLogic.hh>
#include <type_traits>

#include "flying_cheat.hh"

const static std::string module_name = "fly_hacks";

void FlyingCheat::on_world_tick(ClientWorld *world) {
    if (!this->active) {
        return;
    }

    Player *player = (Player *)(world->m_activePlayer.m_object); 
    player->m_jumpSpeed = 1337;
    player->m_jumpHoldTime = 0xdef00d;
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
