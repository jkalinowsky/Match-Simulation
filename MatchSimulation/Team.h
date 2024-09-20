#ifndef OPENFM_TEAM_H
#define OPENFM_TEAM_H

#include "Tactic.h"
#include "Player.h"

class Player;

class Team {
private:
    Tactic* tactic;
    Player** players;
    Player** benchPlayers;
    std::string name;
public:
    Team(std::string newName);
    ~Team();

    std::string getName();
    Player** getPlayers();
    Player& getPlayerIdx(int index);
    Tactic getTactic() const;
    int getPlayerIndexAtFS(struct fieldSection fs);
    int getPlayersAtFSNumber(struct fieldSection fs);
    Player** getPlayersForPass(int passType, Player* tp);
    int findPlayerIndex(const std::string &name);

    void setTactic(Tactic* tactic);
    void setPlayers(Player** players);
    void setBenchPlayers(Player** players);
    void setNumBenchPlayers(int num);

    Player& findNearestPlayer(struct fieldSection fs);
    bool playersNearby(Player* p1, Player* p2);


};


#endif //OPENFM_TEAM_H
