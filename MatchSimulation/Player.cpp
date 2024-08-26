//
// Created by kubak on 30.06.2024.
//

#include "Player.h"

#include <utility>

Player::Player(std::string name, Team* team, Position position, int age, int pace, int finishing, int longShooting,
               int shortPassing, int longPassing, int vision, int dribbling, int defending,
               int gk_diving, int gk_handling, int gk_kicking, int gk_positioning, int gk_reflexes) {
    this->name = std::move(name);
    this->team = team;
    this->position = position;
    this->age = age;
    attributes[PACE] = pace;
    attributes[FINISHING] = finishing;
    attributes[LONGSHOOTING] = longShooting;
    attributes[LONGPASSING] = longPassing;
    attributes[SHORTPASSING] = shortPassing;
    attributes[VISION] = vision;
    attributes[DRIBBLING] = dribbling;
    // CAN BE ADDED IN FUTURE attributes[STRENGTH] = strength;
    // CAN BE ADDED IN FUTURE attributes[AGGRESSION] = aggression;
    attributes[DEFENDING] = defending;
    attributes[GK_DIVING] = gk_diving;
    attributes[GK_HANDLING] = gk_handling;
    attributes[GK_KICKING] = gk_kicking;
    attributes[GK_POSITIONING] = gk_positioning;
    attributes[GK_REFLEXES] = gk_reflexes;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 5; j++) {
            heatMap[i][j] = 0;
        }
    }

    fs = calculateFieldSection();
    movingCooldown = 0;
    calculateOverall();
}

int Player::returnAttribute(int attribute) {
    return this->attributes[attribute];
}

std::string Player::getName() {
    return name;
}

Team* Player::getTeam() {
    return team;
}

Position Player::getPosition() const {
    return this->position;
}

bool Player::getMovingCooldown() {
    return movingCooldown;
}

struct fieldSection Player::getFieldSection() const {
    return this->fs;
}

fieldSection Player::calculateFieldSection() const {
    switch(position.positionDetails) {
        case 0:
            return {2, 0};
        case 1:
            return {0, 1};
        case 2:
            return {2, 1};
        case 3:
            return {4, 1};
        case 4:
        case 5:
            return {2, 2};
        case 6:
            return {2, 3};
        case 7:
            return {0, 3};
        case 8:
            return {2,4};
        case 9:
            return {4, 3};
    }
}

struct fieldSection Player::mirrorFS(struct fieldSection fs) {
    return {5 - fs.x, 9 - fs.y};
}

struct fieldSection Player::randomPosition() const {
    int x[3] = {-1, 0, 1};
    int y[3] = {-1, 0, 1};
    return {this->fs.x + x[rand() % 3], this->fs.y + y[rand() % 3]};
}

bool Player::isRightPositionAttack(int fsX, int fsY) {
    // very long code, figure out how to simplify it
    if (fsX < 0 || fsX > 4 || fsY < 0 || fsY > 9)
        return false;
    int mentality = this->getTeam()->getTactic().getMentality();
    int posDet = this->getPosition().positionDetails;
    // 0 very att, 1 att, 2 balanced, 3 def, 4 very def
    switch (posDet) {
        case 0: // GOALKEEPER
            switch(mentality) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                    if (fsX == 2 && fsY == 0)
                        return true;
                    break;
            }
            return false;
        case 1: // LEFT BACK
            switch(mentality) {
                case 0:
                case 1:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 3 && fsY <= 6))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 2 && fsY <= 5))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 1 && fsY <= 4))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 0 && fsY <= 4))
                        return true;
                    break;
            }
            return false;
        case 2: // CENTER BACK
            switch(mentality) {
                case 0:
                case 1:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 2 && fsY <= 5))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 1 && fsY <= 5))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 1 && fsY <= 3))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 0 && fsY <= 2))
                        return true;
                    break;
            }
            return false;
        case 3: // RIGHT BACK
            switch(mentality) {
                case 0:
                case 1:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 3 && fsY <= 6))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 2 && fsY <= 5))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 1 && fsY <= 4))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 0 && fsY <= 4))
                        return true;
                    break;
            }
            return false;
        case 4: // DEFENSIVE MIDFIELDER
            switch(mentality) {
                case 0:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 3 && fsY <= 7))
                        return true;
                    break;
                case 1:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 3 && fsY <= 6))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 2 && fsY <= 6))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 2 && fsY <= 5))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 1 && fsY <= 4))
                        return true;
                    break;
            }
            return false;
        case 5: // CENTRAL MIDFIELDER
            switch(mentality) {
                case 0:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 4 && fsY <= 8))
                        return true;
                    break;
                case 1:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 4 && fsY <= 7))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 3 && fsY <= 7))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 3 && fsY <= 6))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 2 && fsY <= 5))
                        return true;
                    break;
            }
            return false;
        case 6: // OFFENSIVE MIDFIELDER
            switch(mentality) {
                case 0:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 5 && fsY <= 9))
                        return true;
                    break;
                case 1:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 4 && fsY <= 9))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 4 && fsY <= 8))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 3 && fsY <= 8))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 3 && fsY <= 7))
                        return true;
                    break;
            }
            return false;
        case 7: // LEFT FORWARD
            switch(mentality) {
                case 0:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 7 && fsY <= 9))
                        return true;
                    break;
                case 1:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 6 && fsY <= 9))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 5 && fsY <= 9))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 5 && fsY <= 8))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 4 && fsY <= 8))
                        return true;
                    break;
            }
            return false;
        case 8: // STRIKER
            switch(mentality) {
                case 0:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 7 && fsY <= 9))
                        return true;
                    break;
                case 1:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 6 && fsY <= 9))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 6 && fsY <= 9))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 5 && fsY <= 8))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 4 && fsY <= 8))
                        return true;
                    break;
            }
            return false;
        case 9: // RIGHT FORWARD
            switch(mentality) {
                case 0:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 7 && fsY <= 9))
                        return true;
                    break;
                case 1:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 6 && fsY <= 9))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 5 && fsY <= 9))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 5 && fsY <= 8))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 4 && fsY <= 8))
                        return true;
                    break;
            }
            return false;
    }
}

bool Player::isRightPositionDefence(int fsX, int fsY) {
    // very long code, figure out how to simplify it
    if (fsX < 0 || fsX > 4 || fsY < 0 || fsY > 9)
        return false;
    int mentality = this->getTeam()->getTactic().getMentality();
    int posDet = position.positionDetails;
    // 0 very att, 1 att, 2 balanced, 3 def, 4 very def
    switch (posDet) {
        case 0: // GOALKEEPER
            switch(mentality) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                    if (fsX == 2 && fsY == 0)
                        return true;
                    break;
            }
            return false;
        case 1: // LEFT BACK
            switch(mentality) {
                case 0:
                case 1:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 0 && fsY <= 4))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 0 && fsY <= 3))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 0 && fsY <= 2))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 0 && fsY <= 1))
                        return true;
                    break;
            }
            return false;
        case 2: // CENTER BACK
            switch(mentality) {
                case 0:
                case 1:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 0 && fsY <= 4))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 0 && fsY <= 3))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 0 && fsY <= 2))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 0 && fsY <= 1))
                        return true;
                    break;
            }
            return false;
        case 3: // RIGHT BACK
            switch(mentality) {
                case 0:
                case 1:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 0 && fsY <= 4))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 0 && fsY <= 3))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 0 && fsY <= 2))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 0 && fsY <= 1))
                        return true;
                    break;
            }
            return false;
        case 4: // DEFENSIVE MIDFIELDER
            switch(mentality) {
                case 0:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 1 && fsY <= 5))
                        return true;
                    break;
                case 1:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 1 && fsY <= 4))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 0 && fsY <= 4))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 0 && fsY <= 3))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 0 && fsY <= 2))
                        return true;
                    break;
            }
            return false;
        case 5: // CENTRAL MIDFIELDER
            switch(mentality) {
                case 0:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 2 && fsY <= 6))
                        return true;
                    break;
                case 1:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 1 && fsY <= 6))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 1 && fsY <= 5))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 0 && fsY <= 4))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 0 && fsY <= 3))
                        return true;
                    break;
            }
            return false;
        case 6: // OFFENSIVE MIDFIELDER
            switch(mentality) {
                case 0:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 3 && fsY <= 7))
                        return true;
                    break;
                case 1:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 2 && fsY <= 7))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 2 && fsY <= 6))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 2 && fsY <= 5))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 1 && fsY <= 5))
                        return true;
                    break;
            }
            return false;
        case 7: // LEFT FORWARD
            switch(mentality) {
                case 0:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 4 && fsY <= 7))
                        return true;
                    break;
                case 1:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 4 && fsY <= 6))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 3 && fsY <= 5))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 2 && fsY <= 5))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 0 && fsX <= 1) && (fsY >= 1 && fsY <= 5))
                        return true;
                    break;
            }
            return false;
        case 8: // STRIKER
            switch(mentality) {
                case 0:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 4 && fsY <= 8))
                        return true;
                    break;
                case 1:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 4 && fsY <= 7))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 3 && fsY <= 6))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 2 && fsY <= 6))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 1 && fsX <= 3) && (fsY >= 2 && fsY <= 5))
                        return true;
                    break;
            }
            return false;
        case 9: // RIGHT FORWARD
            switch(mentality) {
                case 0:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 4 && fsY <= 7))
                        return true;
                    break;
                case 1:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 4 && fsY <= 6))
                        return true;
                    break;
                case 2:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 3 && fsY <= 5))
                        return true;
                    break;
                case 3:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 2 && fsY <= 5))
                        return true;
                    break;
                case 4:
                    if ((fsX >= 3 && fsX <= 4) && (fsY >= 1 && fsY <= 5))
                        return true;
                    break;
            }
            return false;
    }
}

void Player::calculateOverall() {
    for (int attribute : attributes)
        this->overall += attribute;
}

void Player::setFieldSection(int newX, int newY) {
    this->fs.x = newX;
    this->fs.y = newY;
}

void Player::moveWithoutBallAttack(int ballY) {
    if (this->getPosition().positionDetails != GK) {
        if (this->isRightPositionAttack(fs.x, fs.y)) {
            struct fieldSection newPos = randomPosition();
            while (!this->isRightPositionAttack(newPos.x, newPos.y)) {
                newPos = randomPosition();
            }
            this->setFieldSection(newPos.x, newPos.y);
            return;
        }
        else {
            struct fieldSection newPos = randomPosition();
            int distance = abs(ballY - this->fs.y);
            int newDistance = abs(ballY - newPos.y);
            while (!(newDistance <= distance && newPos.x >= 0 && newPos.x <= 4 && newPos.y >= 0 && newPos.y <= 9)) {
                newPos = randomPosition();
                newDistance = abs(ballY - newPos.y);
            }
            this->setFieldSection(newPos.x, newPos.y);
            return;
        }
    }
}

void Player::moveWithoutBallDefence(int ballY) {
    if (this->getPosition().positionDetails != GK) {
        if (this->isRightPositionDefence(fs.x, fs.y)) {
            struct fieldSection newPos = randomPosition();
            while (!this->isRightPositionDefence(newPos.x, newPos.y)) {
                newPos = randomPosition();
            }
            this->setFieldSection(newPos.x, newPos.y);
            return;
        }
        else {
            struct fieldSection newPos = randomPosition();
            int distance = abs(ballY - this->fs.y);
            int newDistance = abs(ballY - newPos.y);
            while (!(newDistance >= distance && newPos.x >= 0 && newPos.x <= 4 && newPos.y >= 0 && newPos.y <= 9)) {
                newPos = randomPosition();
                newDistance = abs(ballY - newPos.y);
            }
            this->setFieldSection(newPos.x, newPos.y);
            return;
        }
    }
}

void Player::decreaseMovingCooldown() {
    movingCooldown -= 1;
}

void Player::updateHeatmap() {
    if (fs.y >= 0 && fs.y < 10 && fs.x >= 0 && fs.x < 5) {
        heatMap[fs.y][fs.x] += 1;
    } else {
        std::cerr << "Error: fs.y or fs.x is out of bounds!" << std::endl;
    }
}

void Player::printHeatmap() {
    std::cout << "Player " << this->getName() << " ";
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 5; j++) {
            std::cout << heatMap[i][j] << " ";
        }
    }
    std::cout << std::endl;
}

bool Player::canRun() {
    if (movingCooldown == 0)
        return true;
    return false;
}

bool Player::shouldIShoot() {
    if (this->fs.y >= 7) {
        int chance = fs.y * 5 + this->attributes[FINISHING];
        if (rand()%2000 <= chance)
            return true;
    }
    else {
        int chance = fs.y * 5 + this->attributes[LONGSHOOTING];
        if (rand()%2000 <= chance)
            return true;
    }
    return false;
}

bool Player::shouldIPass() {
    // this should be based on team's tactic and player attributes
    // position based tactic will higher chance to pass, also depending on player's attributes and strictness of tactic
    Tactic tactic = this->team->getTactic();
    // basic percentage 60% to pass and 40% for strictness = balanced, if tactics strictness is strict, player
    // attributes makes less impact on chance
    int passModifier;
    switch (tactic.getPassingStyle()) {
        case 0:
            passModifier = int(this->attributes[SHORTPASSING] * 1.5);
            break;
        case 1:
            passModifier = int((this->attributes[SHORTPASSING] + this->attributes[LONGPASSING]) / 2 * 1.5);
            break;
        default:
            passModifier = int(this->attributes[LONGPASSING] * 1.5);

    }
    int passChance = 60 + passModifier - this->attributes[DRIBBLING] / 2;
    if (rand() % 100 <= passChance)
        return true;
    return false;
}

bool Player::didIIntercept() {
    if (rand() % 100 <= this->attributes[DEFENDING] * 1.5)
        return true;
    return false;
}

int Player::choosePassType() {
    int shortPass, throughPass;
    switch(this->team->getTactic().getPassingStyle()) {
        // short 70/20/10
        case 0:
            shortPass = 70;
            throughPass = 20;
            break;
        // balanced 60/20/20
        case 1:
            shortPass = 60;
            throughPass = 20;
            break;
        // long 25/20/55
        case 2:
            shortPass = 25;
            throughPass = 20;
            break;
        // direct 40/25/35
        case 3:
            shortPass = 40;
            throughPass = 25;
            break;
    }

    int random = rand() % 100;
    if (random <= shortPass)
        return SHORTPASS;
    else if (random <= shortPass + throughPass)
        return THROUGHPASS;
    else
        return LONGPASS;
}

Player& Player::choosePlayerForPass(int passType) {
    int index = 0;
    Player** avPlayers;
    int* numbers = new int[11];
    Team* t = this->getTeam();
    int* chances = new int[3];
    switch(t->getTactic().getMentality()) {
        case 0:
            chances[0] = 35;
            chances[1] = 40;
            chances[2] = 20;
            break;
        case 1:
            chances[0] = 30;
            chances[1] = 45;
            chances[2] = 20;
            break;
        case 2:
            chances[0] = 25;
            chances[1] = 45;
            chances[2] = 25;
            break;
        case 3:
            chances[0] = 25;
            chances[1] = 40;
            chances[2] = 30;
            break;
        case 4:
            chances[0] = 20;
            chances[1] = 45;
            chances[2] = 30;
            break;
    }
    for (int i = 0; i < 11; i++) {
        numbers[i] = -1;
    }
    while (index == 0) {
        avPlayers = t->getPlayersForPass(passType, this);
        int random = rand() % 100;
        if (random <= chances[0]) {
            for (int i = 0; i < 11; i++) {
                if (avPlayers[i] != nullptr) {
                    if (avPlayers[i]->position.positionType == ATT) {
                        numbers[index++] = i;
                    }
                }
            }
        }
        else if (random <= chances[0] + chances[1]) {
            for (int i = 0; i < 11; i++) {
                if (avPlayers[i] != nullptr) {
                    if (avPlayers[i]->position.positionType == MID) {
                        numbers[index++] = i;
                    }
                }
            }
        }
        else if (random <= chances[0] + chances[1] + chances[2]) {
            for (int i = 0; i < 11; i++) {
                if (avPlayers[i] != nullptr) {
                    if (avPlayers[i]->position.positionType == DEF) {
                        numbers[index++] = i;
                    }
                }
            }
        }
        else {
            for (int i = 0; i < 11; i++) {
                if (avPlayers[i] != nullptr) {
                    if (avPlayers[i]->position.positionType == GOALKEEPER) {
                        numbers[index++] = i;
                    }
                }
            }
        }
    }
    return *avPlayers[numbers[rand() % index]];
}

bool Player::isPassCompleted(int passType) {
    // p is player to pass, o is opponent or opponents which are in the same field section, they have chance to intercept
    // calculate pass chance and then calculate interception chance
    int passChance;
    switch(passType) {
        case SHORTPASS:
            passChance = 1.1 * this->attributes[SHORTPASSING] * 4;
            break;
        case THROUGHPASS:
            passChance = 0.8 * (this->attributes[LONGPASSING] * 1/3 + this->attributes[VISION] * 2/3) * 4;
            break;
        case LONGPASS:
            passChance = 0.7 * this->attributes[LONGPASSING] * 4;
            break;
    }
    int chance = rand()%100;
    if (chance <= passChance) {
        //std::cout << "Pass chance - " << passChance << " rand: " << chance << std::endl;
        return true;
    }
    return false;
}

bool Player::didIDribble(Player* o) {
    int dribbleChance = (this->attributes[DRIBBLING] * 3/4 + this->attributes[PACE] * 1/4) * 4 -
                        (o->attributes[DEFENDING] * 2/3 + o->attributes[PACE] * 1/3) * 2;
    if (rand()%100 <= dribbleChance)
        return true;
    return false;
}

void Player::dribble() {
    if (this->getPosition().positionDetails != GK) {
        if (rand()%4 <= 2) {
            struct fieldSection newPos = randomPosition();
            int distance = abs(9 - this->fs.y);
            int newDistance = abs(9 - newPos.y);
            while (!(newDistance <= distance && newPos.x >= 0 && newPos.x <= 4 && newPos.y >= 0 && newPos.y <= 9)) {
                newPos = randomPosition();
                newDistance = abs(9 - newPos.y);
            }
            this->setFieldSection(newPos.x, newPos.y);
            return;
        }
        else {
            struct fieldSection newPos = randomPosition();
            int distance = abs(2 - this->fs.x);
            int newDistance = abs(2 - newPos.x);
            while (!(newDistance <= distance && newPos.x >= 0 && newPos.x <= 4 && newPos.y >= 0 && newPos.y <= 9)) {
                newPos = randomPosition();
                newDistance = abs(2 - newPos.x);
            }
            this->setFieldSection(newPos.x, newPos.y);
            return;
        }

    }
}

bool Player::didISaved(int chance) {
    int saveChance = ((this->attributes[GK_DIVING] + this->attributes[GK_POSITIONING] + this->attributes[GK_REFLEXES]) / 2);
    if (rand()%100 <= chance-saveChance )
        return false;
    return true;
}
