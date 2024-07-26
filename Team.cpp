#include "Team.h"

void Team::setTactic(Tactic* tactic) {
    this->tactic = tactic;
}

void Team::setPlayers(Player **players) {
    for (int i = 0; i < 11; i++) {
        this->players[i] = players[i];
    }
}

Tactic Team::getTactic() const {
    return *tactic;
}

bool Team::playersNearby(Player* p1, Player* p2) {
    return (std::abs(p1->getFieldSection().x - p2->getFieldSection().x) <= 1 &&
            std::abs(p1->getFieldSection().y - p2->getFieldSection().y) <= 1);
}

Player **Team::getPlayers(int passType, Player* tp) {
    Player** players = new Player*[11];
    for (int i = 0; i < 11; ++i) {
        players[i] = nullptr;
    }
    int index = 0;
    switch(passType) {
        case 0:
            for (int i = 0; i < 11; i++) {
                if (playersNearby(tp, players[i]))
                    players[index++] = players[i];
            }
            break;
        case 1:
            for (int i = 0; i < 11; i++) {
                if (!playersNearby(tp, players[i])) {
                    if (players[i]->canRun())
                        players[index++] = players[i];
                }
            }
            break;
        case 2:
            for (int i = 0; i < 11; i++) {
                if (!playersNearby(tp, players[i]))
                    players[index++] = players[i];
            }
            break;
    }
}