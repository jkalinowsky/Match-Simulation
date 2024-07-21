#ifndef OPENFM_TEAM_H
#define OPENFM_TEAM_H
#include "Tactic.h"
#include "Player.h"

class Team {
private:
    Tactic tactic;
    Player** players;

public:
    Team(Tactic& tactic, Player** newPlayers)
            : tactic(tactic) {
        for (int i = 0; i < 11; i++) {
            players[i] = newPlayers[i];
        }
    }

    void setTactic(Tactic& tactic);
    Tactic getTactic() const;
    bool playersNearby(Player* p1, Player* p2);
    Player** getPlayers(int passType, Player* tp);


};


#endif //OPENFM_TEAM_H
