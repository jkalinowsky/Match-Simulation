#ifndef OPENFM_MATCH_H
#define OPENFM_MATCH_H
#include "Team.h"

class Match {
private:
    Team* homeTeam, *awayTeam;
    int homeScore = 0;
    int awayScore = 0;
    Match(Team* team1, Team* team2) {
        homeTeam = team1;
        awayTeam = team2;
    }
public:
    void playMatch();
};


#endif //OPENFM_MATCH_H
