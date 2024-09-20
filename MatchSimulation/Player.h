#ifndef OPENFM_PLAYER_H
#define OPENFM_PLAYER_H
#include <iostream>
#include <random>
#include "Team.h"
#include <ctime>

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
    NONE = -1,
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
    fieldSection fs = {-1, -1};
    int movingCooldown;
    int heatMap[10][5];
public:
    Player(std::string name, Team* team, Position position, int age, int pace, int finishing, int longShooting,
           int shortPassing, int longPassing, int vision, int dribbling, int defending,
           int gk_diving, int gk_handling, int gk_kicking, int gk_positioning, int gk_reflexes);

    int returnAttribute(int attribute);
    std::string getName();
    Team* getTeam();
    Position getPosition() const;
    bool getMovingCooldown();

    struct fieldSection getFieldSection() const;
    struct fieldSection calculateFieldSection() const;
    static struct fieldSection mirrorFS(struct fieldSection fs);
    struct fieldSection randomPosition() const;

    bool isRightPositionAttack(int fsX, int fsY);
    bool isRightPositionDefence(int fsX, int fsY);

    void calculateOverall();
    void setFieldSection(int newX, int newY);
    void moveWithoutBallAttack(int ballY);
    void moveWithoutBallDefence(int ballY);
    void decreaseMovingCooldown();

    void updateHeatmap();
    void printHeatmap();
    

    // functions which are responsible for Player actions
    bool canRun();
    bool shouldIShoot();
    bool shouldIPass();
    bool didIIntercept();
    int choosePassType();
    int choosePlayerForPass(int passType);
    bool isPassCompleted(int passType);
    bool didIDribble(Player* o);
    void dribble();
    bool didISaved(int chance);
};


#endif //OPENFM_PLAYER_H
