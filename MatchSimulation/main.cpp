#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include "Match.h"
#include <thread>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include "sqlite3.h"
#include "json.hpp"
std::atomic<bool> matchCompleted(false);

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    #define SOCKET_TYPE SOCKET
    #define CLOSE_SOCKET closesocket
    #define INIT_SOCKET() WSAStartup(MAKEWORD(2, 2), &wsaData)
    #define CLEANUP_SOCKET() WSACleanup()
    #define SOCKET_ERROR_CODE WSAGetLastError()
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define SOCKET_TYPE int
    #define CLOSE_SOCKET close
    #define INIT_SOCKET() (void)0
    #define CLEANUP_SOCKET() (void)0
    #define SOCKET_ERROR_CODE errno
#endif

#pragma comment(lib, "Ws2_32.lib")
// first version of match simulation based on own algorithm and model

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

PositionDetails returnPosition(int pos) {
    switch (pos) {
        case -1:
            return NONE;
        case 0:
            return GK;
        case 1:
            return LB;
        case 2:
            return CB;
        case 3:
            return RB;
        case 4:
            return DM;
        case 5:
            return CM;
        case 6:
            return CAM;
        case 7:
            return LF;
        case 8:
            return ST;
        case 9:
            return RF;
        default:
            return NONE;
    }
}

void loadPlayers(const std::string& teamName, Team* team) {
    sqlite3* db;
    sqlite3_stmt* stmt;

    std::string query = "SELECT p.short_name, p.first_position, p.second_position, p.is_first_squad, p.age, p.pace, p.attacking_finishing, "
                        "p.power_long_shots, p.skill_long_passing, p.attacking_short_passing, "
                        "p.mentality_vision, p.dribbling, p.defending, p.goalkeeping_diving, "
                        "p.goalkeeping_handling, p.goalkeeping_kicking, p.goalkeeping_positioning, "
                        "p.goalkeeping_reflexes "
                        "FROM players p "
                        "INNER JOIN team_membership tm ON p.short_name = tm.player_name "
                        "INNER JOIN teams_premier_league t ON tm.team_id = t.team_id "
                        "WHERE t.team_name = '" + teamName + "'";


    if (sqlite3_open("../../database.db", &db) != SQLITE_OK) {
        std::cerr << "Failed to open database\n";
        return;
    }

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement\n";
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, teamName.c_str(), -1, SQLITE_STATIC);

    std::vector<Player*> firstPlayers;
    std::vector<Player*> benchPlayers;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* nameText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string name = (nameText ? nameText : "");
        const char* isFirstSquadText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string isFirstSquadStr = (isFirstSquadText ? isFirstSquadText : "");

        int firstPosition = sqlite3_column_int(stmt, 1);
        bool isFirstSquad;
        if (isFirstSquadStr == "True")
            isFirstSquad = true;
        else
            isFirstSquad = false;

        PositionDetails posD = returnPosition(firstPosition);
        PositionType posT = choosePosType(posD);
        Position pos = {posD, posT};
        int age = sqlite3_column_int(stmt, 4);
        int pace = sqlite3_column_int(stmt, 5);
        int finishing = sqlite3_column_int(stmt, 6);
        int longs = sqlite3_column_int(stmt, 7);
        int longp = sqlite3_column_int(stmt, 8);
        int shortp = sqlite3_column_int(stmt, 9);
        int vis = sqlite3_column_int(stmt, 10);
        int drib = sqlite3_column_int(stmt, 11);
        int def = sqlite3_column_int(stmt, 12);

        int gk_d = sqlite3_column_int(stmt, 13);
        int gk_h = sqlite3_column_int(stmt, 14);
        int gk_k = sqlite3_column_int(stmt, 15);
        int gk_p = sqlite3_column_int(stmt, 16);
        int gk_r = sqlite3_column_int(stmt, 17);

        Player* player;
        player = new Player(name, team, pos, age, pace, finishing, longs, shortp,
                            longp, vis, drib, def,
                            gk_d, gk_h, gk_k, gk_p, gk_r);

        if (isFirstSquad) {
            firstPlayers.push_back(player);
        }
        else
            benchPlayers.push_back(player);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    Player** firstArray = new Player*[11]();
    for (size_t i = 0; i < 11; ++i) {
        firstArray[i] = firstPlayers[i];
    }

    std::sort(firstArray, firstArray + 11, [](const Player* a, const Player* b) {
        return a->getPosition().positionDetails < b->getPosition().positionDetails;
    });
    team->setPlayers(firstArray);
    Player** benchArray = new Player*[9]();
    for (size_t i = 0; i < 9; ++i) {
        benchArray[i] = benchPlayers[i];
    }
    std::sort(benchArray, benchArray + 9, [](const Player* a, const Player* b) {
        return a->getPosition().positionDetails < b->getPosition().positionDetails;
    });
    team->setBenchPlayers(benchArray);

    delete[] firstArray;
    delete[] benchArray;
}

Tactic* loadTactic(const std::string& teamName) {
    sqlite3* db;
    sqlite3_stmt* stmt;

    std::string query = "SELECT tactic_mentality, tactic_tempo, tactic_passing_style, "
                        "tactic_work_rate, tactic_width, tactic_pressing_line, tactic_strictness, "
                        "formation_defenders, formation_midfielders, formation_attackers "
                        "FROM teams_premier_league WHERE team_name = '" + teamName + "'";



    if (sqlite3_open("../../database.db", &db) != SQLITE_OK) {
        std::cerr << "Failed to open database\n";
        return nullptr;
    }

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement\n";
        sqlite3_close(db);
        return nullptr;
    }

    sqlite3_bind_text(stmt, 1, teamName.c_str(), -1, SQLITE_STATIC);


    int mentality, tempo, passingStyle, workRate, width, pressingLine, strictness, f1, f2, f3;
    mentality = sqlite3_column_int(stmt, 0);
    tempo = sqlite3_column_int(stmt, 1);
    workRate = sqlite3_column_int(stmt, 2);
    width = sqlite3_column_int(stmt, 3);
    pressingLine = sqlite3_column_int(stmt, 4);
    strictness = sqlite3_column_int(stmt, 5);
    f1 = sqlite3_column_int(stmt, 6);
    f2 = sqlite3_column_int(stmt, 7);
    f3 = sqlite3_column_int(stmt, 8);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return new Tactic(mentality, tempo, passingStyle, workRate, width, pressingLine, strictness, f1, f2, f3);
}

void sendStatsAsJson(int** stats, int numRows, int numCols, SOCKET_TYPE clientSocket) {
    nlohmann::json jsonStats;
    jsonStats["type"] = "match-stats";
    nlohmann::json statsArray = nlohmann::json::array();

    for (int i = 0; i < numRows; ++i) {
        nlohmann::json row;
        for (int j = 0; j < numCols; ++j) {
            row.push_back(stats[i][j]);
        }
        statsArray.push_back(row);
    }
    jsonStats["data"] = statsArray;
    std::string jsonString = jsonStats.dump();
    send(clientSocket, jsonString.c_str(), jsonString.size(), 0);
}

void sendStatsPeriodically(SOCKET_TYPE clientSocket, Match* match) {
    int time = 0;
    while(true) {
        int** stats = match->getStats();
        if (stats[TIME][0] - time > 20 ) {
            int numRows = 9;
            int numCols = 2;
            sendStatsAsJson(stats, numRows, numCols, clientSocket);
            time = stats[TIME][0];
        }
    }
}

void handleClient(SOCKET_TYPE clientSocket, Match* match) {
    char buffer[512];
    int recvResult;
    bool setupDone = false;

    std::vector<char> binaryData;

    while ((recvResult = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        std::string command(buffer, recvResult);
        if (!setupDone) {
            try {
                auto j = nlohmann::json::parse(command);

                if (j.contains("team1") && j.contains("team2")) {
                    std::string team1_n = j["team1"];
                    std::string team2_n = j["team2"];
                    Team* team1 = new Team(team1_n);
                    loadPlayers(team1_n, team1);
                    team1->setTactic(loadTactic(team1_n));
                    Team* team2 = new Team(team2_n);
                    loadPlayers(team2_n, team2);
                    team2->setTactic(loadTactic(team1_n));
                    match->setHomeTeam(team1);
                    match->setAwayTeam(team2);

                    std::thread matchThread([match]() {
                        match->playMatch();
                        matchCompleted.store(true);
                    });
                    matchThread.detach();

                    setupDone = true;
                } else {
                    std::cerr << "Invalid JSON format. Missing team1 or team2." << std::endl;
                }
            } catch (const nlohmann::json::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }
        if (command.rfind("STATS", 0) == 0) {
            std::string playerName = command.substr(6);
            int* stats = match->getPlayerStats(playerName);

            nlohmann::json jsonStats;
            jsonStats["type"] = "player-stats";
            nlohmann::json statsArray = nlohmann::json::array();

            for (int i = 0; i < 13; i++) {
                statsArray.push_back(stats[i]);
            }

            jsonStats["data"] = statsArray;

            std::string jsonString = jsonStats.dump();
            send(clientSocket, jsonString.c_str(), jsonString.size(), 0);
            delete[] stats;
        }
        else {
            match->handleClientCommand(command);
        }
        send(clientSocket, "ACK", 3, 0);
    }

    #if defined(_WIN32) || defined(_WIN64)
        CLOSE_SOCKET(clientSocket);
    #else
        CLOSE_SOCKET(clientSocket);
    #endif
}

int main() {
    srand(time(0));
    #if defined(_WIN32) || defined(_WIN64)
        WSADATA wsaData;
        SOCKET ListenSocket = INVALID_SOCKET;
        SOCKET ClientSocket = INVALID_SOCKET;
        struct addrinfo *result = NULL, *ptr = NULL, hints;
    #else
        int ListenSocket = -1;
        int ClientSocket = -1;
        struct sockaddr_in hints, clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
    #endif

    int iResult;
    INIT_SOCKET();

    #if defined(_WIN32) || defined(_WIN64)
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        iResult = getaddrinfo(NULL, "27015", &hints, &result);
        if (iResult != 0) {
            std::cerr << "Getaddrinfo failed: " << iResult << std::endl;
            CLEANUP_SOCKET();
            return 1;
        }

        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            std::cerr << "Socket failed: " << SOCKET_ERROR_CODE << std::endl;
            freeaddrinfo(result);
            CLEANUP_SOCKET();
            return 1;
        }

        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            std::cerr << "Bind failed: " << SOCKET_ERROR_CODE << std::endl;
            freeaddrinfo(result);
            CLOSE_SOCKET(ListenSocket);
            CLEANUP_SOCKET();
            return 1;
        }

        freeaddrinfo(result);
    #else
        ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (ListenSocket < 0) {
            std::cerr << "Socket creation failed: " << SOCKET_ERROR_CODE << std::endl;
            CLEANUP_SOCKET();
            return 1;
        }

        memset(&hints, 0, sizeof(hints));
        hints.sin_family = AF_INET;
        hints.sin_addr.s_addr = INADDR_ANY;
        hints.sin_port = htons(27015);

        if (bind(ListenSocket, (struct sockaddr*)&hints, sizeof(hints)) < 0) {
            std::cerr << "Bind failed: " << SOCKET_ERROR_CODE << std::endl;
            CLOSE_SOCKET(ListenSocket);
            CLEANUP_SOCKET();
            return 1;
        }
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        iResult = listen(ListenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            std::cerr << "listen failed: " << SOCKET_ERROR_CODE << std::endl;
            CLOSE_SOCKET(ListenSocket);
            CLEANUP_SOCKET();
            return 1;
        }
    #else
        if (listen(ListenSocket, SOMAXCONN) < 0) {
            std::cerr << "listen failed: " << SOCKET_ERROR_CODE << std::endl;
            CLOSE_SOCKET(ListenSocket);
            CLEANUP_SOCKET();
            return 1;
        }
    #endif

    std::cout << "Waiting for a connection..." << std::endl;

    #if defined(_WIN32) || defined(_WIN64)
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << SOCKET_ERROR_CODE << std::endl;
            CLOSE_SOCKET(ListenSocket);
            CLEANUP_SOCKET();
            return 1;
        }
    #else
        ClientSocket = accept(ListenSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (ClientSocket < 0) {
            std::cerr << "Accept failed: " << SOCKET_ERROR_CODE << std::endl;
            CLOSE_SOCKET(ListenSocket);
            CLEANUP_SOCKET();
            return 1;
        }
    #endif

    std::cout << "Client connected." << std::endl;

    Match* match = new Match();

    std::thread clientThread(handleClient, ClientSocket, match);
    std::thread statsThread(sendStatsPeriodically, ClientSocket, match);

    while (true) {
        if (matchCompleted.load()) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    clientThread.join();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    CLOSE_SOCKET(ListenSocket);
    CLEANUP_SOCKET();

    std::cout << "THE END";
    return 0;
}
