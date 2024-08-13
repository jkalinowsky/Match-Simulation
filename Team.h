#ifndef OPENFM_TEAM_H
#define OPENFM_TEAM_H

#include <optional>
#include "Tactic.h"
#include "Player.h"

class Player;

class Team {
private:
    Tactic* tactic;
    Player** players;
    std::string name;
public:
    Team(std::string newName);

    std::string getName();
    Player** getPlayers();
    Player& getPlayerIdx(int index);
    Tactic getTactic() const;
    std::optional<std::reference_wrapper<Player>> getPlayerAtFS(struct fieldSection fs);
    int getPlayersAtFSNumber(struct fieldSection fs);
    Player** getPlayersForPass(int passType, Player* tp);

    void setTactic(Tactic* tactic);
    void setPlayers(Player** players);

    Player& findNearestPlayer(struct fieldSection fs);
    bool playersNearby(Player* p1, Player* p2);


};


#endif //OPENFM_TEAM_H
