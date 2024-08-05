#include "Team.h"

void Team::setTactic(Tactic* tactic) {
    this->tactic = tactic;
}

void Team::setPlayers(Player **players) {
    for (int i = 0; i < 11; i++) {
        this->players[i] = players[i];
    }
}

Player **Team::getPlayers() {
    return players;
}

Player* Team::getPlayerAtFS(fieldSection fs) {
    // returning first player because we start from defenders
    for (int i = 0; i < 11; i++) {
        if (players[i]->mirrorFS(players[i]->getFieldSection()) == fs)
            return players[i];
    }
    return nullptr;
}

Tactic Team::getTactic() const {
    return *tactic;
}

bool Team::playersNearby(Player* p1, Player* p2) {
    return (std::abs(p1->getFieldSection().x - p2->getFieldSection().x) <= 1 &&
            std::abs(p1->getFieldSection().y - p2->getFieldSection().y) <= 1);
}

Player **Team::getPlayersForPass(int passType, Player* tp) {
    Player** players = new Player*[11];
    Player** teamPlayers = tp->getTeam()->getPlayers();
    for (int i = 0; i < 11; ++i) {
        players[i] = nullptr;
    }
    int index = 0;
    switch(passType) {
        case 0:
            for (int i = 0; i < 11; i++) {
                if (playersNearby(tp, teamPlayers[i]))
                    players[index++] = teamPlayers[i];
            }
            break;
        case 1:
            for (int i = 0; i < 11; i++) {
                if (!playersNearby(tp, teamPlayers[i])) {
                    if (players[i]->canRun())
                        players[index++] = teamPlayers[i];
                }
            }
            break;
        case 2:
            for (int i = 0; i < 11; i++) {
                if (!playersNearby(tp, teamPlayers[i]))
                    players[index++] = teamPlayers[i];
            }
            break;
    }
    return players;
}

Team::Team() {
    players = new Player*[11]();
}