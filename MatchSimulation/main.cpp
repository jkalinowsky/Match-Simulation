#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include "Match.h"
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include "sqlite3.h"

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
    sqlite3* db;
    sqlite3_stmt* stmt;
    std::string query = "SELECT short_name, club_position, age, pace, attacking_finishing, "
                        "power_long_shots, skill_long_passing, attacking_short_passing, "
                        "mentality_vision, dribbling, defending, goalkeeping_diving, "
                        "goalkeeping_handling, goalkeeping_kicking, goalkeeping_positioning, "
                        "goalkeeping_reflexes FROM players WHERE club_name = '" + teamName + "'";

    if (sqlite3_open("C:\\Users\\kubak\\Projekty\\OpenFM\\database.db", &db) != SQLITE_OK) {
        std::cerr << "Failed to open database\n";
        return nullptr;
    }

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement\n";
        sqlite3_close(db);
        return nullptr;
    }

    // Bind the team name parameter
    sqlite3_bind_text(stmt, 1, teamName.c_str(), -1, SQLITE_STATIC);

    std::vector<Player*> players;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (players.size() >= 11) break;

        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string posStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        if (posStr != "SUB" && posStr != "RES") {
            PositionDetails posD = stringToPosition(posStr);
            PositionType posT = choosePosType(posD);
            Position pos = {posD, posT};
            int age = sqlite3_column_int(stmt, 2) / 4;
            int pace = sqlite3_column_int(stmt, 3) / 4;
            int finishing = sqlite3_column_int(stmt, 4) / 4;
            int longs = sqlite3_column_int(stmt, 5) / 4;
            int longp = sqlite3_column_int(stmt, 6) / 4;
            int shortp = sqlite3_column_int(stmt, 7) / 4;
            int vis = sqlite3_column_int(stmt, 8) / 4;
            int drib = sqlite3_column_int(stmt, 9) / 4;
            int def = sqlite3_column_int(stmt, 10) / 4;

            int gk_d = sqlite3_column_int(stmt, 11) / 4;
            int gk_h = sqlite3_column_int(stmt, 12) / 4;
            int gk_k = sqlite3_column_int(stmt, 13) / 4;
            int gk_p = sqlite3_column_int(stmt, 14) / 4;
            int gk_r = sqlite3_column_int(stmt, 15) / 4;

            Player* player;
            if (posD == GK) {
                player = new Player(name, team, pos, age, pace, finishing, longs, shortp, longp, vis, drib, def,
                                    gk_d, gk_h, gk_k, gk_p, gk_r);
            } else {
                player = new Player(name, team, pos, age, pace, finishing, longs, shortp, longp, vis, drib, def,
                                    0, 0, 0, 0, 0);
            }
            players.push_back(player);
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    Player** playerArray = new Player*[players.size()];
    for (size_t i = 0; i < players.size(); ++i) {
        playerArray[i] = players[i];
    }

    if (players.size() < 11) {
        for (size_t i = players.size(); i < 11; ++i) {
            playerArray[i] = nullptr;
        }
    }

    return playerArray;
}
void serializeStats(int** stats, std::vector<char>& buffer) {
    buffer.resize(9 * 2 * sizeof(int));  // 9 rows * 2 columns * size of int
    char* ptr = buffer.data();

    // Copy each int into the buffer
    for (int i = 0; i < 9; ++i) {
        memcpy(ptr, stats[i], 2 * sizeof(int));
        ptr += 2 * sizeof(int);
    }
}

void handleClient(SOCKET_TYPE clientSocket, Match* match) {
    char buffer[512];
    int recvResult;

    std::vector<char> binaryData;

    while ((recvResult = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        std::string command(buffer, recvResult);

        if (command == "GET_STATS") {
            int** stats = match->getStats();
            serializeStats(stats, binaryData);

            send(clientSocket, binaryData.data(), binaryData.size(), 0);
        } else {
            match->handleClientCommand(command);
            send(clientSocket, "ACK", 3, 0); // Acknowledge receipt
        }
    }

    #if defined(_WIN32) || defined(_WIN64)
        CLOSE_SOCKET(clientSocket);
    #else
        CLOSE_SOCKET(clientSocket);
    #endif
}

int main() {
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

    Match* match = new Match(manC, arsenal);

    std::thread clientThread(handleClient, ClientSocket, match);
    clientThread.detach();

    match->playMatch();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    CLOSE_SOCKET(ListenSocket);
    CLEANUP_SOCKET();

    std::cout << "THE END";
    return 0;
}
