//
// Created by kubak on 21.07.2024.
//

#include "Match.h"

Match::Match(Team* team1, Team* team2)  {
    homeTeam = team1;
    awayTeam = team2;


    for (int i = 0; i < 9; i++) {
        stats[i] = new int[2];
        stats[i][HOME] = 0;
        stats[i][AWAY] = 0;
    }
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

    std::cout << "Hometeam heatmap" << std::endl;
    for (int i = 0; i < 11; i++) {
        awayTeam->getPlayers()[i]->printHeatmap();
    }
}

void Match::matchStatsToFile() {
    int homePos = static_cast<int>((static_cast<double>(stats[TOUCHES][HOME]) / (stats[TOUCHES][AWAY] + stats[TOUCHES][HOME])) * 100);

    std::ofstream outFile;
    outFile.open("/mnt/c/Users/kubak/Projekty/OpenFM/OpenFM/stats.txt", std::ios_base::app);

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

void Match::playMatch() {
    int time = 0;
    Player** ballOwner = new Player*;
    (*ballOwner) = &homeTeam->getPlayerIdx(10);
    int ball = HOME;

    std::cout << homeTeam->getName() << " - " << awayTeam->getName() << std::endl;

    while (time < 5400) {
        if ((*ballOwner)->getTeam()->getName() == homeTeam->getName()) {
            defenceAction(awayTeam, ballOwner);
            attackAction(homeTeam, ballOwner, &ball, time);
        }
        else {
            defenceAction(homeTeam, ballOwner);
            attackAction(awayTeam, ballOwner, &ball,  time);
        }
        /*std::cout << std::endl << "BALL " << ball << std::endl;
        for (int i = 0; i < 11; i++) {
            std::cout << "PLAYER: " << homeTeam->getPlayers()[i]->getName() << " POSITION: " << homeTeam->getPlayers()[i]->getFieldSection().x << " | " << homeTeam->getPlayers()[i]->getFieldSection().y << std::endl;
        }*/
        //std::cout << "TIME: " << time << std::endl;
        time+=4;
    }
    this->printStats();
    this->matchStatsToFile();
}

void Match::attackAction(Team* team, Player** ballOwner, int* ball, int time) {
    bool wasAction = false;
    for (int i = 0; i < 11; i++) {
        Player* player = team->getPlayers()[i];
        if (!wasAction && (*ballOwner)->getName() == player->getName()) {
            wasAction = true;
            actionWithBall(ballOwner, ball, time);
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

void Match::actionWithBall(Player** ballOwner, int* ball, int time) {
    if ((*ballOwner)->shouldIShoot()) {
        //std::cout << "SHOT BY " << (*ballOwner)->getName() << " in " << time/60+1 << std::endl;
        stats[SHOTS][*ball] += 1;
        auto opt = homeTeam->getPlayerAtFS((*ballOwner)->getFieldSection());
        if (*ball == AWAY) {
            opt = awayTeam->getPlayerAtFS((*ballOwner)->getFieldSection());
        }
        if (opt) {
            Player& defender = opt.value();
            if (defender.didIIntercept()) {
                if (*ball == HOME) {
                    (*ballOwner) = &defender;
                    *ball = AWAY;
                    return;
                }
                else {
                    (*ballOwner) = &defender;
                    *ball = HOME;
                    return;
                }
            }
        }
        else {
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
                if (*ball == AWAY)
                    gk = homeTeam->getPlayerIdx(0);

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
                                homeTeam->findNearestPlayer(gk.getFieldSection());
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
                    std::cout << "GOOOOAAAAL BY " << (*ballOwner)->getName() << " in " << time/60+1 << " minute and xG was: " << chance << std::endl;
                    stats[GOALS][*ball] += 1;
                    if (*ball == HOME) {
                        (*ballOwner) = &awayTeam->getPlayerIdx(10);
                        *ball = AWAY;
                        resetPositions();
                        return;
                    }
                    else {
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
    }
    else {
        if ((*ballOwner)->shouldIPass()) {
            // passing actions
            int passType = (*ballOwner)->choosePassType();
            Player& passPlayer = (*ballOwner)->choosePlayerForPass(passType);
            if ((*ballOwner)->isPassCompleted(passType)) {
                auto opt = awayTeam->getPlayerAtFS((*ballOwner)->getFieldSection());
                if (*ball == AWAY) {
                    opt = homeTeam->getPlayerAtFS((*ballOwner)->getFieldSection());
                }
                if (opt) {
                    Player& defender = opt.value();
                    if (defender.didIIntercept()) {
                        if (*ball == HOME) {
                            (*ballOwner) = &defender;
                            *ball = AWAY;
                            stats[INTERCEPTIONS][*ball] += 1;
                            return;
                        }
                        else {
                            (*ballOwner) = &defender;
                            *ball = HOME;
                            stats[INTERCEPTIONS][*ball] += 1;
                            return;
                        }
                    }
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
            auto opt = awayTeam->getPlayerAtFS((*ballOwner)->getFieldSection());
            if (*ball == AWAY) {
                opt = homeTeam->getPlayerAtFS((*ballOwner)->getFieldSection());
            }
            if (opt) {
                Player& defender = opt.value();
                if(!(*ballOwner)->didIDribble(&defender)) {
                    if (*ball == HOME) {
                        *ball = AWAY;
                        (*ballOwner) = &defender;
                        return;
                    }
                    else {
                        *ball = HOME;
                        (*ballOwner) = &defender;
                        return;
                    }
                }
            }
            else {
                if ((*ballOwner)->getMovingCooldown() == 0) {
                    (*ballOwner)->dribble();
                    stats[DRIBBLES][*ball] += 1;
                }
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
