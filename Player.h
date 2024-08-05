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
#define VISION 5
#define DRIBBLING 6
#define DEFENDING 7
#define GK_DIVING 8
#define GK_HANDLING 9
#define GK_KICKING 10
#define GK_POSITIONING 11
#define GK_REFLEXES 12

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

    bool operator==(const fieldSection& other) const {
        return x == other.x && y == other.y;
    }
};

class Team;

class Player {
private:

    std::string name;
    Team* team;
    Position position{};
    int age;
    int attributes[13] = {0,0,0,0,0,0,0, 0};
    int overall = 0;
    fieldSection fieldSection = {-1, -1};
public:
    Player(std::string name, Team* team, Position position, int age, int pace, int finishing, int longShooting,
           int shortPassing, int longPassing, int vision, int dribbling, int defending,
           int gk_diving, int gk_handling, int gk_kicking, int gk_positioning, int gk_reflexes);

    void print();

    std::string getName();

    Team* getTeam();

    void calculateOverall();
    struct fieldSection calculateFieldSection() const;
    int returnAttribute(int attribute);
    void setFieldSection(int newX, int newY);
    void moveOnField(int xInc, int yInc);
    struct fieldSection getFieldSection() const;
    struct fieldSection mirrorFS(struct fieldSection fs);
    Position getPosition() const;
    

    // functions which are responsible for Player actions
    bool canRun();
    bool shouldIShoot();
    bool shouldIPass();
    bool didIIntercept();
    int choosePassType();
    Player* choosePlayerForPass(int passType);
    bool isPassCompleted(int passType, Player* o);
    bool didIDribble(Player* o);
    // need to add function for dribbling
    bool didIBlock();
    int didIShoot();
    bool didISaved();
};


#endif //OPENFM_PLAYER_H
