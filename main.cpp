#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include "Match.h"
#include <unistd.h>
// first version of match simulation based on own algorithm and model

PositionDetails stringToPosition(const std::string& positionStr) {
    if (positionStr == "ST") return ST;
    else if (positionStr == "LW" || positionStr == "LF" || positionStr == "LM") return LF;
    else if (positionStr == "RW" || positionStr == "RF" || positionStr == "RM") return RF;
    else if (positionStr == "CAM" || positionStr == "CF") return CAM;
    else if (positionStr == "CM" || positionStr == "LCM" || positionStr == "RCM") return CM;
    else if (positionStr == "CDM" || positionStr == "RDM" || positionStr == "LDM") return DM;
    else if (positionStr == "RWB" || positionStr == "RB") return RB;
    else if (positionStr == "LWB" || positionStr == "LB") return LB;
    else if (positionStr == "CB" || positionStr == "LCB" || positionStr == "RCB") return CB;
    else if (positionStr == "GK") return GK;
}

PositionType choosePosType(PositionDetails posD) {
    if (posD == GK) return GOALKEEPER;
    else if (posD == LB || posD == CB || posD == RB) return DEF;
    else if (posD == DM || posD == CM || posD == CAM) return MID;
    else return ATT;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int safe_stoi(const std::string& str) {
    if (str.empty()) {
        return rand() % 10 + 1;
    }
    try {
        return std::stoi(str);
    } catch (const std::invalid_argument&) {
        return rand() % 10 + 1;
    }
}

Player** loadPlayers(const std::string& teamName, Team* team) {
    std::ifstream file("/mnt/c/Users/kubak/Projekty/OpenFM/OpenFM/database.csv");
    if (!file) {
        std::cerr << "Failed to open database.csv\n";
    }
    std::string line;
    std::vector<Player*> players;
    int playerCount = 0;



    while(std::getline(file, line)) {
        if (playerCount >= 11) break;
        std::vector<std::string> fields = split(line, ',');
        if (fields[3] == teamName && fields[1] != "RES" && fields[1] != "SUB") {
            std::string name = fields[0];
            PositionDetails posD = stringToPosition(fields[1]);
            PositionType posT = choosePosType(posD);
            Position pos = {posD, posT};
            int age = (int) safe_stoi(fields[2]) / 4;
            int pace = (int) safe_stoi(fields[5]) / 4;
            int finishing = (int) safe_stoi(fields[6]) / 4;
            int longs = (int) safe_stoi(fields[7]) / 4;
            int longp = (int) safe_stoi(fields[8]) / 4;
            int shortp = (int) safe_stoi(fields[9]) / 4;
            int vis = (int) safe_stoi(fields[10]) / 4;
            int drib = (int) safe_stoi(fields[11]) / 4;
            int def = (int) safe_stoi(fields[12]) / 4;
            Player* player;
            if (posD == GK) {
                int gk_d = safe_stoi(fields[13]) / 4;
                int gk_h = safe_stoi(fields[14]) / 4;
                int gk_k = safe_stoi(fields[15]) / 4;
                int gk_p = safe_stoi(fields[16]) / 4;
                int gk_r = safe_stoi(fields[17]) / 4;
                player = new Player(name, team, pos, age, pace, finishing, longs,
                                            shortp, longp, vis, drib, def,
                                            gk_d, gk_h, gk_k, gk_p, gk_r);
            }
            else {
                player = new Player(name, team, pos, age, pace, finishing, longs,
                                            shortp, longp, vis, drib, def,
                                            0, 0, 0, 0, 0);
            }
            players.push_back(player);
            playerCount++;
        }
    }

    Player** playerArray = new Player*[11];
    for (int i = 0; i < 11; i++) {
        playerArray[i] = players[i];
    }

    return playerArray;

}


int main() {
    /*char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        perror("getcwd() error");
    }*/
    srand(time(0));
    Tactic* posBased = new Tactic("Tactic", 1, 2, 0, 2, 2, 2, 2, 4, 4, 2);
    Tactic* longBased = new Tactic("Tactic", 4, 2, 1, 2, 2, 2, 2, 4, 4, 2);

    // loading manchester city
    Team* manC = new Team("Manchester City");
    Player** players = loadPlayers("Manchester City", manC);
    std::sort(players, players + 11, [](const Player* a, const Player* b) {
        return a->getPosition().positionDetails < b->getPosition().positionDetails;
    });
    manC->setPlayers(players);

    // loading arsenal
    Team* arsenal = new Team("Arsenal");
    players = loadPlayers("Arsenal", arsenal);
    std::sort(players, players + 11, [](const Player* a, const Player* b) {
        return a->getPosition().positionDetails < b->getPosition().positionDetails;
    });
    arsenal->setPlayers(players);

    manC->setTactic(posBased);
    arsenal->setTactic(posBased);

    Match* m = new Match(manC, arsenal);
    m->playMatch();
    std::cout << "THE END";
    return 0;
}
