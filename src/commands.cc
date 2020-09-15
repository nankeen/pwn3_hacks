#include <iostream>
#include <dlfcn.h>
#include <gameLogic.hh>
#include <string.h>

static bool frozen;

void Player::Chat(const char *msg)
{
    // Get the original chat function
    void (*chat)(Player *, const char *) = (void (*)(Player *, const char *))dlsym(RTLD_NEXT, "_ZN6Player4ChatEPKc");

    if (strncmp("!tp ", msg, 4) == 0) {

        // Absolute teleport
        Vector3 new_pos = this->GetPosition();
        sscanf(msg+4, "%f %f %f", &(new_pos.x), &(new_pos.y), &(new_pos.z));
        this->SetPosition(new_pos);

    } else if (strncmp("!tpr ", msg, 5) == 0) {

        // Relative teleport
        Vector3 new_pos = this->GetPosition();
        sscanf(msg+5, "%f %f %f", &(new_pos.x), &(new_pos.y), &(new_pos.z));
        this->SetPosition(new_pos + this->GetPosition());

    } else if (strncmp("!ls", msg, 3) == 0) {

        // Get the game world object
        ClientWorld *world = *((ClientWorld**)(dlsym(RTLD_NEXT, "GameWorld")));
        if (world == NULL) {
            std::cerr << "Couldn't locate 'GameWorld' symbol" << std::endl;
            return;
        }

        // List all actors
        for (auto iactor: world->m_actors) {
            Actor *actor = (Actor *)iactor.m_object;
            std::string name = actor->GetDisplayName();
            Vector3 pos = actor->GetPosition();

            std::cout << name << ": "
                << pos.x << ", "
                << pos.y << ", "
                << pos.z << ", " << std::endl;
        }
    }

}
