#ifndef OPENFM_PLAYER_H
#define OPENFM_PLAYER_H
#include <iostream>
#include <random>
#include "Team.h"

#define PACE 0
#define FINISHING 1
#define LONGSHOOTING 2
#define SHORTPASSING 3
#define LONGPASSING 4
#define DRIBBLING 5
#define DEFENDING 6

enum PositionDetails {
    GK = 0,
    LB = 1,
    CB = 2,
    RB = 3,
    DM = 4,
    CM = 5,
    CAM = 6,
    LF = 7,
    ST = 8,
    RF = 9,
};

enum PositionType {
    GOALKEEPER = 0,
    DEF = 1,
    MID = 2,
    ATT = 3,
};

enum PassType {
    SHORTPASS = 0,
    THROUGHPASS = 1,
    LONGPASS = 2,
};

struct Position {
    PositionDetails positionDetails;
    PositionType positionType;
};

struct fieldSection {
    int x;
    int y;
};

class Player {
private:
    std::string name;
    Team* team;
    Position position{};
    int age;
    int attributes[7] = {0,0,0,0,0,0,0};
    int overall = 0;
    fieldSection fieldSection = {-1, -1};


    Player(const std::string &name, Team &team, Position position, int age, int pace, int finishing, int longShooting,
           int shortPassing, int longPassing, int dribbling, int defending) {
        this->name = name;
        this->team = &team;
        this->position = position;
        this->age = age;
        attributes[PACE] = pace;
        attributes[FINISHING] = finishing;
        attributes[LONGSHOOTING] = longShooting;
        attributes[LONGPASSING] = longPassing;
        attributes[SHORTPASSING] = shortPassing;
        // CAN BE ADDED IN FUTURE attributes[VISION] = vision;
        attributes[DRIBBLING] = dribbling;
        // CAN BE ADDED IN FUTURE attributes[STRENGTH] = strength;
        // CAN BE ADDED IN FUTURE attributes[AGGRESSION] = aggression;
        attributes[DEFENDING] = defending;

        calculateOverall();
    }

public:
    void calculateOverall();
    int returnAttribute(int attribute);
    void setFieldSection(int xInc, int yInc);
    void moveOnField(int newX, int newY);
    struct fieldSection getFieldSection() const;

    // functions which are responsible for Player actions
    bool canRun();
    bool shouldIShoot();
    bool shouldIPass();
    bool didIIntercept();
    int choosePassType();
    Player* choosePlayerForPass(int passType);
    bool isPassCompleted(Player* p);
    bool shouldIDribble();
    bool didIDribble();
    bool didIBlock();
    int didIShoot();
    bool didISaved();
};


#endif //OPENFM_PLAYER_H
