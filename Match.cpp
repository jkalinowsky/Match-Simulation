//
// Created by kubak on 21.07.2024.
//

#include "Match.h"

Match::Match(Team* team1, Team* team2)  {
    homeTeam = team1;
    awayTeam = team2;


    for (int i = 0; i < 7; i++) {
        stats[i] = new int[2];
        stats[i][HOME] = 0;
        stats[i][AWAY] = 0;
    }
}

void Match::playMatch() {
    // FIRST VERSION - FULL MATCH, WITHOUT EXHAUSTION, PAUSES AND TACTICAL CHANGES
    int time = 0; // we add 3 seconds per action, match last 5400 seconds so 1800 actions per game
    Player* ballOwner = homeTeam->getPlayers()[10];
    int ball = HOME;
    bool wasAction = false;

    std::cout << "Match starts" << std::endl;

    while (time < 5400) {
        wasAction = false;
        if (ball == HOME) {
            playersAction(homeTeam->getPlayers(), ballOwner, &ball, &wasAction);
            playersAction(awayTeam->getPlayers(), nullptr, &ball, &wasAction);
        }
        else {
            playersAction(awayTeam->getPlayers(), ballOwner, &ball, &wasAction);
            playersAction(homeTeam->getPlayers(), nullptr, &ball, &wasAction);
        }
        //std::cout << "TIME: " << time << std::endl;
        time+=3;
    }

    std::cout << "Match ends. Passes: " << stats[PASSES][HOME] << " | " << stats[PASSES][AWAY] << std::endl;
}

void Match::playersAction(Player** players, Player* ballOwner, int* ball, bool* wasAction) {
    // players in offensive
    if (ballOwner != nullptr) {
        for (int i = 0; i < 11; i++) {
            if (ballOwner->getName() == players[i]->getName() && !*wasAction) {
                actionWithBall(ballOwner, ball);
                *wasAction = true;
            }
            // movement without ball
        }
    }
    else {
        for (int i = 0; i < 11; i++) {
            // defending
        }
    }
}

void Match::actionWithBall(Player* ballOwner, int* ball) {
    if (ballOwner->shouldIShoot()) {
        // shooting actions
    }
    else {
        if (ballOwner->shouldIPass()) {
            // passing actions
            int passType = ballOwner->choosePassType();
            Player* passPlayer = ballOwner->choosePlayerForPass(passType);
            if (passPlayer == nullptr) {
                // idk we lost a ball or sth !TO COMPLETE!
                //std::cout << ballOwner->getName() << " lost a ball. ";
                if (*ball == HOME) {
                    *ballOwner = *awayTeam->getPlayers()[0];
                    *ball = AWAY;
                }
                else {
                    *ballOwner = *homeTeam->getPlayers()[0];
                    *ball = HOME;
                }
                //std::cout << ballOwner->getName() << " resumes play." << std::endl;
                return;
            }
            Player* defender = awayTeam->getPlayerAtFS(ballOwner->getFieldSection());
            if (ballOwner->isPassCompleted(passType, defender)) {
                //std::cout << "Ball passed from " << ballOwner->getName() << " to " << passPlayer->getName() << std::endl;
                *ballOwner = *passPlayer;
                this->stats[PASSES][*ball] += 1;
                return;
            }
            else {
                std::cout << ballOwner->getName() << " lost a ball. ";
                *ballOwner = *defender;
                std::cout << ballOwner->getName() << " make great interception!" << std::endl;
                if (*ball == HOME)
                    *ball = AWAY;
                else
                    *ball = HOME;
                return;
            }
        }
        else {
            Player* defender = awayTeam->getPlayerAtFS(ballOwner->getFieldSection());
            if (defender == nullptr) {
                // here running instead of dribbling
            }
            else {
                if (ballOwner->didIDribble(defender)) {
                    // dribbling completed
                }
                else {
                    // ball lost
                    *ballOwner = *defender;
                    if (*ball == HOME)
                        *ball = AWAY;
                    else
                        *ball = HOME;
                    return;
                }
            }
        }
    }
}
