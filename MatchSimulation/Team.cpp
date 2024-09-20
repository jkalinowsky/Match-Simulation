#include "Team.h"

Team::Team(std::string newName) {
    players = new Player*[11]();
    benchPlayers = new Player*[9]();
    name = newName;
}

Team::~Team() {
    delete tactic;
    if (players) {
        for (int i = 0; players[i] != nullptr; ++i) {
            delete players[i];
        }
        delete[] players;
    }

    if (benchPlayers) {
        for (int i = 0; benchPlayers[i] != nullptr; ++i) {
            delete benchPlayers[i];
        }
        delete[] benchPlayers;
    }
}

std::string Team::getName() {
    return name;
}

Player **Team::getPlayers() {
    return players;
}

Player &Team::getPlayerIdx(int index) {
    return *players[index];
}

Tactic Team::getTactic() const {
    return *tactic;
}

int Team::getPlayerIndexAtFS(fieldSection fs) {
    for (int i = 0; i < 11; i++) {
        if (players[i]->mirrorFS(players[i]->getFieldSection()) == fs)
            return i;
    }
    return -1;
}

int Team::getPlayersAtFSNumber(struct fieldSection fs) {
    std::vector<Player *> playersVector;
    for (int i = 0; i < 11; i++) {
        if (players[i]->mirrorFS(players[i]->getFieldSection()) == fs) {
            playersVector.push_back(players[i]);
        }
    }
    return playersVector.size();
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
                    if (teamPlayers[i]->canRun())
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

int Team::findPlayerIndex(const std::string &name) {
    for (int i=0; i<11; i++) {
        if (players[i]->getName() == name)
            return i;
    }

    return -1;
}

void Team::setTactic(Tactic* tactic) {
    this->tactic = tactic;
}

void Team::setPlayers(Player **players) {
    for (int i = 0; i < 11; i++) {
        this->players[i] = players[i];
    }
}

void Team::setBenchPlayers(Player **players) {
    for (int i = 0; i < 9; i++) {
        this->benchPlayers[i] = players[i];
    }
}

Player& Team::findNearestPlayer(struct fieldSection fs) {
    int distance;
    int min = 99;
    int index = -1;
    fs = players[0]->mirrorFS(fs);
    for (int i = 0; i < 11; i++) {
        distance = std::abs(players[i]->getFieldSection().x - fs.x) + std::abs(players[i]->getFieldSection().y - fs.y);
        if (distance < min) {
            min = distance;
            index = i;
        }
    }

    return *players[index];
}

bool Team::playersNearby(Player* p1, Player* p2) {
    return (std::abs(p1->getFieldSection().x - p2->getFieldSection().x) <= 2 &&
            std::abs(p1->getFieldSection().y - p2->getFieldSection().y) <= 2);
}