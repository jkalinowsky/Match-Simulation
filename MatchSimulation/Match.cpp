//
// Created by kubak on 21.07.2024.
//

#include "Match.h"

Match::Match(Team* team1, Team* team2) : delay(300) {
    homeTeam = team1;
    awayTeam = team2;


    for (int i = 0; i < 9; i++) {
        stats[i] = new int[2];
        stats[i][HOME] = 0;
        stats[i][AWAY] = 0;
    }
    running = true;
}

Match::Match() : delay(300) {
    stats[0] = new int[1];
    stats[0][0] = 0;
    for (int i = 1; i < 10; i++) {
        stats[i] = new int[2];
        stats[i][HOME] = 0;
        stats[i][AWAY] = 0;
    }

    for (int i = 0; i < 11; i++) {
        homeStats[i] = new int[13];
        awayStats[i] = new int[13];
        for (int j = 0; j < 13; j++) {
            homeStats[i][j] = 0;
            awayStats[i][j] = 0;
        }
    }
    running = true;
}

void Match::setHomeTeam(Team *team) {
    homeTeam = team;
}

void Match::setAwayTeam(Team *team) {
    awayTeam = team;
}

void Match::printStats() {
    int homePos = static_cast<int>((static_cast<double>(stats[TOUCHES][HOME]) / (stats[TOUCHES][AWAY] + stats[TOUCHES][HOME])) * 100);
    std::cout << "Match ends. RESULT: " << stats[GOALS][HOME] << " - " << stats[GOALS][AWAY];
    std::cout << " Shots: " << stats[SHOTS][HOME] << " | " << stats[SHOTS][AWAY];
    std::cout << " Shots on target: " << stats[SHOTS_OT][HOME] << " | " << stats[SHOTS_OT][AWAY];
    std::cout << " Possesion: " << homePos << "% - " << 100-homePos << "%" << std::endl;
    std::cout << "Passes: " << stats[PASSES][HOME] << " | " << stats[PASSES][AWAY];
    std::cout << " Touches: " << stats[TOUCHES][HOME] << " | " << stats[TOUCHES][AWAY];
    std::cout << " Interceptions: : " << stats[INTERCEPTIONS][HOME] << " | " << stats[INTERCEPTIONS][AWAY];
    std::cout << " Dribbles: : " << stats[DRIBBLES][HOME] << " | " << stats[DRIBBLES][AWAY] << std::endl;

    /*std::cout << "Away team heatmap" << std::endl;
    for (int i = 0; i < 11; i++) {
        awayTeam->getPlayers()[i]->printHeatmap();
    }*/
    std::cout << "Home team" << std::endl;
    for (int i=0; i < 11; i++) {
        std::cout << homeTeam->getPlayerIdx(i).getName() << " ";
        for (int j=0; j < 11; j++) {
            std::cout << homeStats[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Away team" << std::endl;
    for (int i=0; i < 11; i++) {
        std::cout << awayTeam->getPlayerIdx(i).getName() << " ";
        for (int j=0; j < 11; j++) {
            std::cout << awayStats[i][j] << " ";
        }
        std::cout << std::endl;
    }

    running = false;
}

void Match::matchStatsToFile() {
    int homePos = static_cast<int>((static_cast<double>(stats[TOUCHES][HOME]) / (stats[TOUCHES][AWAY] + stats[TOUCHES][HOME])) * 100);

    std::ofstream outFile;
    outFile.open("C:\\Users\\kubak\\Projekty\\OpenFM\\MatchSimulation\\stats.txt", std::ios_base::app);

    if (outFile.is_open()) {
        outFile << "Match ends. RESULT: " << stats[GOALS][HOME] << " - " << stats[GOALS][AWAY];
        outFile << " Shots: " << stats[SHOTS][HOME] << " | " << stats[SHOTS][AWAY];
        outFile << " Shots on target: " << stats[SHOTS_OT][HOME] << " | " << stats[SHOTS_OT][AWAY];
        outFile << " Possession: " << homePos << "% - " << 100 - homePos << "%";
        outFile << " Passes: " << stats[PASSES][HOME] << " | " << stats[PASSES][AWAY];
        outFile << " Touches: " << stats[TOUCHES][HOME] << " | " << stats[TOUCHES][AWAY];
        outFile << " Interceptions: " << stats[INTERCEPTIONS][HOME] << " | " << stats[INTERCEPTIONS][AWAY];
        outFile << " Dribbles: " << stats[DRIBBLES][HOME] << " | " << stats[DRIBBLES][AWAY] << std::endl;

        outFile.close();
    } else {
        std::cerr << "Error opening file!" << std::endl;
    }
}

int** Match::getStats() {
    return stats;
}

void Match::playMatch() {
    Player** ballOwner = new Player*;
    (*ballOwner) = &homeTeam->getPlayerIdx(10);
    int ball = HOME;

    std::cout << homeTeam->getName() << " - " << awayTeam->getName() << std::endl;
    while (stats[TIME][0] < 5400) {
        if (running) {
            if ((*ballOwner)->getTeam()->getName() == homeTeam->getName()) {
                defenceAction(awayTeam, ballOwner);
                attackAction(homeTeam, ballOwner, &ball);
            } else {
                defenceAction(homeTeam, ballOwner);
                attackAction(awayTeam, ballOwner, &ball);
            }
            stats[TIME][0] += 3 + rand()%5;
            std::this_thread::sleep_for(delay);
            std::cout << "Next action" << std::endl;
        }
    }
    this->printStats();
    //this->matchStatsToFile();
}

void Match::pauseMatch() {
    running = false;
}

void Match::resumeMatch() {
    running = true;
}

void Match::skipMatch() {
    std::chrono::milliseconds newDelay(0);
    delay = newDelay;
}

void Match::firstSpeed() {
    std::chrono::milliseconds newDelay(300);
    delay = newDelay;
}

void Match::secondSpeed() {
    std::chrono::milliseconds newDelay(150);
    delay = newDelay;
}

void Match::thirdSpeed() {
    std::chrono::milliseconds newDelay(50);
    delay = newDelay;
}

void Match::handleClientCommand(const std::string& command) {
    if (command == "PAUSE") {
        pauseMatch();
    }
    else if (command == "START") {
        resumeMatch();
    }
    else if (command == "SKIP") {
        skipMatch();
    }
    else if (command == "1X") {
        firstSpeed();
    }
    else if (command == "3X") {
        secondSpeed();
    }
    else if (command == "5X") {
        thirdSpeed();
    }
}

void Match::attackAction(Team* team, Player** ballOwner, int* ball) {
    bool wasAction = false;
    for (int i = 0; i < 11; i++) {
        Player* player = team->getPlayers()[i];
        if (!wasAction && (*ballOwner)->getName() == player->getName()) {
            wasAction = true;
            actionWithBall(ballOwner, ball, i);
            stats[TOUCHES][*ball] += 1;
        }
        else {
            // movement without ball
            player->moveWithoutBallAttack((*ballOwner)->getFieldSection().y);
        }

        if (player->getMovingCooldown() > 0)
            player->decreaseMovingCooldown();

        player->updateHeatmap();
    }
}

void Match::defenceAction(Team *team, Player** ballOwner) {
    for (int i = 0; i < 11; i++) {
        Player* player = team->getPlayers()[i];
        if (player->getName() != (*ballOwner)->getName())
            player->moveWithoutBallDefence((*ballOwner)->getFieldSection().y);

        player->updateHeatmap();
    }
}

void Match::actionWithBall(Player** ballOwner, int* ball, int idx) {
    if ((*ballOwner)->shouldIShoot()) {
        //std::cout << "SHOT BY " << (*ballOwner)->getName() << " in " << time/60+1 << std::endl;
        stats[SHOTS][*ball] += 1;
        if (*ball == HOME) {
            this->homeStats[idx][2] += 1;
            int index = awayTeam->getPlayerIndexAtFS((*ballOwner)->getFieldSection());
            if (index != -1) {
                Player& defender = awayTeam->getPlayerIdx(index);
                if (defender.didIIntercept()) {
                    if (*ball == HOME) {
                        (*ballOwner) = &defender;
                        *ball = AWAY;
                        this->awayStats[index][12] += 1;
                    }
                    else {
                        (*ballOwner) = &defender;
                        *ball = HOME;
                        this->homeStats[index][12] += 1;
                    }
                    stats[INTERCEPTIONS][*ball] += 1;
                    return;
                }
            }
        }
        else if (*ball == AWAY) {
            this->awayStats[idx][2] += 1;
            int index = homeTeam->getPlayerIndexAtFS((*ballOwner)->getFieldSection());
            if (index != -1) {
                Player& defender = homeTeam->getPlayerIdx(index);
                if (defender.didIIntercept()) {
                    if (*ball == HOME) {
                        (*ballOwner) = &defender;
                        *ball = AWAY;
                        this->awayStats[index][13] += 1;
                    }
                    else {
                        (*ballOwner) = &defender;
                        *ball = HOME;
                        this->homeStats[index][13] += 1;
                    }
                    stats[INTERCEPTIONS][*ball] += 1;
                    return;
                }
            }
        }
        int playersInBox = awayTeam->getPlayersAtFSNumber((*ballOwner)->getFieldSection());
        int chance = 0;
        //chance = 50 - (playersInBox * 10)
        struct fieldSection fs = (*ballOwner)->getFieldSection();
        if (fs.x == 2 && fs.y == 9)
            chance = ((30 - playersInBox * 10)) + 30 + (*ballOwner)->returnAttribute(FINISHING);
        else if (fs.x >= 1 && fs.x <= 3 && fs.y == 8)
            chance = ((30 - playersInBox * 10)) + 20 + (*ballOwner)->returnAttribute(FINISHING);
        else if (((fs.x == 1 || fs.x == 3) && fs.y == 9) || (fs.x >= 1 && fs.x <= 3 && fs.y == 7))
            chance = ((30 - playersInBox * 10)) + 10 + ((*ballOwner)->returnAttribute(FINISHING) / 2 + (*ballOwner)->returnAttribute(LONGSHOOTING) / 2);
        else if ((fs.x == 0 || fs.x == 4) && (fs.y >= 6 && fs.y <= 9) || (fs.y <= 6))
            chance = ((30 - playersInBox * 10)/2) + 5 + (*ballOwner)->returnAttribute(LONGSHOOTING);
        if (rand() % 100 <= chance) {
            // on target
            stats[SHOTS_OT][*ball] += 1;
            Player& gk = awayTeam->getPlayerIdx(0);
            if (*ball == AWAY) {
                this->awayStats[idx][3] += 1;
                gk = homeTeam->getPlayerIdx(0);
            }
            else
                this->homeStats[idx][3] += 1;


            if (gk.didISaved(chance)) {
                // saved, here is 60% chance to keep ball in gk hands, 25% to corner and 15% to reflection
                int gkChance = rand() % 100;
                if (gkChance <= 60) {
                    // gk saved, his ball
                    if (*ball == HOME) {
                        *ball = AWAY;
                        (*ballOwner) = &gk;
                        return;
                    }
                    else {
                        *ball = HOME;
                        (*ballOwner) = &gk;
                        return;
                    }
                }
                else if (gkChance <= 85) {
                    // corner
                    // TEMPORARY
                    if (*ball == HOME) {
                        *ball = AWAY;
                        (*ballOwner) = &gk;
                        return;
                    }
                    else {
                        *ball = HOME;
                        (*ballOwner) = &gk;
                        return;
                    }
                }
                else {
                    Player& nearest = awayTeam->findNearestPlayer(gk.getFieldSection());
                    if (rand()%100 <= 60) {
                        // ball for enemy defender
                        if (*ball == AWAY) {
                            nearest = homeTeam->findNearestPlayer(gk.getFieldSection());
                            *ball = HOME;
                            (*ballOwner) = &nearest;
                            return;
                        }
                        *ball = AWAY;
                        (*ballOwner) = &nearest;
                        return;
                    }
                    else {
                        if (*ball == HOME) {
                            nearest = homeTeam->findNearestPlayer(gk.getFieldSection());
                            *ball = HOME;
                            (*ballOwner) = &nearest;
                            return;
                        }
                        *ball = AWAY;
                        (*ballOwner) = &nearest;
                        return;
                    }
                }
            }
            else {
                // GOAL
                //std::cout << "GOOOOAAAAL BY " << (*ballOwner)->getName() << " in " << time/60+1 << " minute and xG was: " << chance << std::endl;
                stats[GOALS][*ball] += 1;
                if (*ball == HOME) {
                    this->homeStats[idx][0];
                    (*ballOwner) = &awayTeam->getPlayerIdx(10);
                    *ball = AWAY;
                    resetPositions();
                    return;
                }
                else {
                    this->awayStats[idx][0];
                    (*ballOwner) = &homeTeam->getPlayerIdx(10);
                    *ball = HOME;
                    resetPositions();
                    return;
                }
            }
        }
        else {
            // ball lost, shot off target
            Player& gk = awayTeam->getPlayerIdx(0);
            if (*ball == AWAY) {
                gk = homeTeam->getPlayerIdx(0);
                *ball = HOME;
                (*ballOwner) = &gk;
                return;
            }
            else {
                *ball = AWAY;
                (*ballOwner) = &gk;
                return;
            }
        }
    }
    else {
        if ((*ballOwner)->shouldIPass()) {
            // passing actions

            int passType = (*ballOwner)->choosePassType();
            if (*ball == HOME) {
                this->homeStats[idx][5] += 1;
                if (passType == 0)
                    this->homeStats[idx][7] += 1;
                else if (passType == 1)
                    this->homeStats[idx][9] += 1;
                else
                    this->homeStats[idx][8] += 1;
            }
            else {
                this->awayStats[idx][5] += 1;
                if (passType == 0)
                    this->awayStats[idx][7] += 1;
                else if (passType == 1)
                    this->awayStats[idx][9] += 1;
                else
                    this->awayStats[idx][8] += 1;
            }

            int i = (*ballOwner)->choosePlayerForPass(passType);;
            if (i == -1) {
                if (*ball == HOME) {
                    Player& nearest = homeTeam->findNearestPlayer((*ballOwner)->getFieldSection());
                    (*ballOwner) = &nearest;
                    *ball = AWAY;
                    return;
                }
                else {
                    Player& nearest = awayTeam->findNearestPlayer((*ballOwner)->getFieldSection());
                    (*ballOwner) = &nearest;
                    *ball = HOME;
                    return;
                }
            }
            Player& passPlayer = (*ballOwner)->getTeam()->getPlayerIdx(i);
            if ((*ballOwner)->isPassCompleted(passType)) {
                if (*ball == HOME) {
                    int index = awayTeam->getPlayerIndexAtFS((*ballOwner)->getFieldSection());
                    if (index != -1) {
                        Player& defender = awayTeam->getPlayerIdx(index);
                        if (defender.didIIntercept()) {
                            if (*ball == HOME) {
                                (*ballOwner) = &defender;
                                *ball = AWAY;
                                this->awayStats[index][12] += 1;
                            }
                            else {
                                (*ballOwner) = &defender;
                                *ball = HOME;
                                this->homeStats[index][12] += 1;
                            }
                            stats[INTERCEPTIONS][*ball] += 1;
                            return;
                        }
                    }
                    this->homeStats[idx][6] += 1;
                }
                else if (*ball == AWAY) {
                    int index = homeTeam->getPlayerIndexAtFS((*ballOwner)->getFieldSection());
                    if (index != -1) {
                        Player& defender = homeTeam->getPlayerIdx(index);
                        if (defender.didIIntercept()) {
                            if (*ball == HOME) {
                                (*ballOwner) = &defender;
                                *ball = AWAY;
                                this->awayStats[index][12] += 1;
                            }
                            else {
                                (*ballOwner) = &defender;
                                *ball = HOME;
                                this->homeStats[index][12] += 1;
                            }
                            stats[INTERCEPTIONS][*ball] += 1;
                            return;
                        }
                    }
                    this->awayStats[idx][6] += 1;
                }
                //std::cout << "Ball passed from " << ballOwner->getName() << " to " << passPlayer->getName() << std::endl;
                (*ballOwner) = &passPlayer;
                this->stats[PASSES][*ball] += 1;
                return;
            }
            else {
                //std::cout << ballOwner->getName() << " lost a ball. ";
                if (*ball == HOME) {
                    *ball = AWAY;
                    (*ballOwner) = &awayTeam->findNearestPlayer((*ballOwner)->getFieldSection());
                    stats[INTERCEPTIONS][*ball] += 1;
                    return;
                }
                else {
                    *ball = HOME;
                    (*ballOwner) = &homeTeam->findNearestPlayer((*ballOwner)->getFieldSection());
                    stats[INTERCEPTIONS][*ball] += 1;
                    return;
                }
                //std::cout << ballOwner->getName() << " make great interception!" << std::endl;
            }
        }
        else {
            if (*ball == HOME) {
                this->homeStats[idx][10] += 1;
                int index = awayTeam->getPlayerIndexAtFS((*ballOwner)->getFieldSection());
                if (index != -1) {
                    Player& defender = awayTeam->getPlayerIdx(index);
                    if (defender.didIIntercept()) {
                        if (*ball == HOME) {
                            (*ballOwner) = &defender;
                            *ball = AWAY;
                            this->awayStats[index][12] += 1;
                        }
                        else {
                            (*ballOwner) = &defender;
                            *ball = HOME;
                            this->homeStats[index][12] += 1;
                        }
                        stats[INTERCEPTIONS][*ball] += 1;
                        return;
                    }
                }
                this->homeStats[idx][11] += 1;
            }
            else if (*ball == AWAY) {
                this->awayStats[idx][10] += 1;
                int index = homeTeam->getPlayerIndexAtFS((*ballOwner)->getFieldSection());
                if (index != -1) {
                    Player& defender = homeTeam->getPlayerIdx(index);
                    if (defender.didIIntercept()) {
                        if (*ball == HOME) {
                            (*ballOwner) = &defender;
                            *ball = AWAY;
                            this->awayStats[index][12] += 1;
                        }
                        else {
                            (*ballOwner) = &defender;
                            *ball = HOME;
                            this->homeStats[index][12] += 1;
                        }
                        stats[INTERCEPTIONS][*ball] += 1;
                        return;
                    }
                }
                this->awayStats[idx][11] += 1;
            }
            if ((*ballOwner)->getMovingCooldown() == 0) {
                (*ballOwner)->dribble();
                stats[DRIBBLES][*ball] += 1;
            }
        }
    }
}

void Match::resetPositions() {
    for (int i = 0; i < 11; i++) {
        awayTeam->getPlayers()[i]->calculateFieldSection();
        homeTeam->getPlayers()[i]->calculateFieldSection();
    }
}
