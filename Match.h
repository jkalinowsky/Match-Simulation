#ifndef OPENFM_MATCH_H
#define OPENFM_MATCH_H
#include "Team.h"

#define HOME 0
#define AWAY 1
#define GOALS 0
#define SHOTS 1
#define SHOTS_OT 2 // on target
#define CORNERS 3
#define Y_CARDS 4
#define R_CARDS 5
#define PASSES 6

class Match {
private:
    Team* homeTeam, *awayTeam;
    int* stats[7];
public:
    Match(Team* team1, Team* team2);
    void playMatch();
    void playersAction(Player** players, Player* ballOwner, int* ball, bool* wasAction);
    void actionWithBall(Player* ballOwner, int* ball);
};


#endif //OPENFM_MATCH_H
