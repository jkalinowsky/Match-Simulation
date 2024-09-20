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
#define TIME 0
#define GOALS 1
#define SHOTS 2
#define SHOTS_OT 3 // on target
#define CORNERS 4
#define R_CARDS 5
#define DRIBBLES 6
#define PASSES 7
#define TOUCHES 8
#define INTERCEPTIONS 9

#define PLAYERGOALS 0
#define PLAYERASSISTS 1
#define PLAYERSHOTS 2
#define PLAYERSHOTSONTARGET 3
#define PLAYERTOUCHES 4
#define PLAYERPASSESATTEMPT 5
#define PLAYERPASSESCOMPLETED 6
#define PLAYERSHORTPASSES 7
#define PLAYERLONGPASSES 8
#define PLAYERTHROUGHPASSES 9
#define PLAYERDRIBBLESATTEMPT 10
#define PLAYERDRIBBLESCOMPLETED 11
#define PLAYERINTERCEPTIONS 12
#define PLAYERBLOCKS 13

class Match {
private:
    Team* homeTeam, *awayTeam;
    int* stats[10];
    int* homeStats[11];
    int* awayStats[11];
    std::atomic<bool> running;
    std::chrono::milliseconds delay;
public:
    Match(Team* team1, Team* team2);
    Match();

    void setHomeTeam(Team* team);
    void setAwayTeam(Team* team);

    void pauseMatch();
    void resumeMatch();
    void skipMatch();
    void firstSpeed();
    void secondSpeed();
    void thirdSpeed();
    void handleClientCommand(const std::string& command);
    int* getPlayerStats(const std::string& name);
    int** getStats();
    void printStats();
    void matchStatsToFile();
    void playMatch();
    void attackAction(Team* team, Player** ballOwner, int* ball);
    void defenceAction(Team* team, Player** ballOwner);
    void actionWithBall(Player** ballOwner, int* ball, int idx);
    void resetPositions();
};


#endif //OPENFM_MATCH_H
