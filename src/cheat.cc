#include <dlfcn.h>
#include <iostream>
#include <gameLogic.hh>
#include <X11/Xlib.h>
#include <memory>

#include "cheat.hh"
#include "flying_cheat.hh"
#include "speed_cheat.hh"

// Use for keyboard cool down
static int kb_cooldown = 0;

// Used for X Server functions
static Display *display;

// Used to track the state of cheats
std::unique_ptr<std::map<std::string, std::unique_ptr<Cheat>>> cheats;

// Check key events
bool check_key(char *keys_return, KeySym keysym)
{
    KeyCode kc = XKeysymToKeycode(display, keysym);
    return !!(keys_return[kc >> 3] & (1<<(kc&7)));
}

// Handle the world tick
// Can be use to listen to input events
void World::Tick(float f)
{
    // Call the original world tick function first
    void (*world_tick)(World*, float) = (void (*)(World*, float))dlsym(RTLD_NEXT, "_ZN5World4TickEf");
    if (world_tick!= NULL) {
        world_tick(this, f);
    }

    // Get the game world object
    ClientWorld *world = *((ClientWorld**)(dlsym(RTLD_NEXT, "GameWorld")));
    if (world == NULL) {
        std::cerr << "Couldn't locate 'GameWorld' symbol" << std::endl;
        return;
    }

    // Run all on world tick callbacks
    for (auto &[name, cheat]: *cheats) {
        if (kb_cooldown > 0) {
            kb_cooldown--;
        } else if (cheat->check_toggle(display)) {
            kb_cooldown = 500;
            cheat->toggle();
            std::cout << "Toggling cheat: " << name << "..." << std::endl;
        }
        cheat->on_world_tick(world);
    }
}

// Increase the player speed
void increase_speed(ClientWorld *world)
{
    Player *player = (Player *)(world->m_activePlayer.m_object); 
    player->m_walkingSpeed = 0xf00d;
}

// Open the display structure
__attribute__((constructor)) static void init_cheats()
{
    cheats = std::make_unique<std::map<std::string, std::unique_ptr<Cheat>>>();
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        perror("XOpenDisplay error");
        exit(1);
    }

    // Register cheats
    cheats->emplace("fly_hacks", std::make_unique<FlyingCheat>());
    cheats->emplace("speed_hacks", std::make_unique<SpeedCheat>());
}

// Clean up the display structure
__attribute__((destructor)) static void cleanup_cheats()
{
    XCloseDisplay(display);
    cheats->clear();
}

void Cheat::on_world_tick(ClientWorld *world) {}
bool Cheat::check_toggle(Display *) { return false; }

// Implement basic cheat controls
void Cheat::enable()
{
    this->active = true;
}

void Cheat::disable()
{
    this->active = false;
}

void Cheat::toggle()
{
    if (this->active) {
        this->disable();
    } else {
        this->enable();
    }
}
