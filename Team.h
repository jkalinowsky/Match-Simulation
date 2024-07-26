#ifndef OPENFM_TEAM_H
#define OPENFM_TEAM_H
#include "Tactic.h"
#include "Player.h"

class Player;

class Team {
private:
    Tactic* tactic;
    Player** players;

public:
    Team() {}

    void setTactic(Tactic* tactic);
    void setPlayers(Player** players);
    Tactic getTactic() const;
    bool playersNearby(Player* p1, Player* p2);
    Player** getPlayers(int passType, Player* tp);


};


#endif //OPENFM_TEAM_H
