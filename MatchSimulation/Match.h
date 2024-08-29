#ifndef OPENFM_MATCH_H
#define OPENFM_MATCH_H
#include "Team.h"
#include <fstream>
#include <atomic>
#include <chrono>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

#define HOME 0
#define AWAY 1
#define GOALS 0
#define SHOTS 1
#define SHOTS_OT 2 // on target
#define CORNERS 3
#define R_CARDS 4
#define DRIBBLES 5
#define PASSES 6
#define TOUCHES 7
#define INTERCEPTIONS 8

class Match {
private:
    Team* homeTeam, *awayTeam;
    int* stats[9];
    std::atomic<bool> running;
    std::chrono::seconds delay;
public:
    Match(Team* team1, Team* team2);
    Match();

    void setHomeTeam(Team* team);
    void setAwayTeam(Team* team);

    void pauseMatch();
    void resumeMatch();
    void skipMatch();
    void handleClientCommand(const std::string& command);
    int** getStats();
    void printStats();
    void matchStatsToFile();
    void playMatch();
    void attackAction(Team* team, Player** ballOwner, int* ball, int time);
    void defenceAction(Team* team, Player** ballOwner);
    void actionWithBall(Player** ballOwner, int* ball, int time);
    void resetPositions();
};


#endif //OPENFM_MATCH_H
