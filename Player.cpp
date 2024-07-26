//
// Created by kubak on 30.06.2024.
//

#include "Player.h"

#include <utility>

void Player::print() {
    std::cout << "Player: " << name << ", Position: " << position.positionDetails << ", Age: " << age << ", Overall: " << overall << std::endl;
}

void Player::calculateOverall() {
    for (int attribute : attributes)
        this->overall += attribute;
}

int Player::returnAttribute(int attribute) {
    return this->attributes[attribute];
}

void Player::moveOnField(int xInc, int yInc) {
    this->fieldSection.x += xInc;
    this->fieldSection.y += yInc;
}

void Player::setFieldSection(int newX, int newY) {
    this->fieldSection.x = newX;
    this->fieldSection.y = newY;
}

struct fieldSection Player::getFieldSection() const {
    return this->fieldSection;
}

Position Player::getPosition() const {
    return this->position;
}

// functions which are responsible for Player actions

// TO DO
bool Player::canRun() {
    return true;
}

bool Player::shouldIShoot() {
    if (this->fieldSection.y >= 8) {
        if (rand() % 100 <= this->attributes[FINISHING] * 6)
            return true;
    }
    else if (this->fieldSection.y >= 6) {
        if (rand() % 100 <= this->attributes[LONGSHOOTING] * 4)
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
    int passChance = 60 + passModifier - this->attributes[DRIBBLING];
    if (rand() % 100 <= passChance)
        return true;
    return false;
}

bool Player::didIIntercept() {
    if (rand() % 100 <= this->attributes[DEFENDING] * 2)
        return true;
    return false;
}

int Player::choosePassType() {
    int shortPass, throughPass, longPass;
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

Player* Player::choosePlayerForPass(int passType) {
    Team* t = this->team;
    int chances[3];
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
    Player** avPlayers;
    avPlayers = t->getPlayers(passType, this);
    int random = rand() % 100;
    int* numbers = new int[11];
    int index = 0;
    for (int i = 0; i < 11; i++) {
        numbers[i] = -1;
    }
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

    if (index == 0)
        return nullptr;

    return avPlayers[numbers[rand() % index]];
}

bool Player::isPassCompleted(int passType, Player** o) {
    // p is player to pass, o is opponent or opponents which are in the same field section, they have chance to intercept
    // calculate pass chance and then calculate interception chance
    int passChance;
    switch(passType) {
        case SHORTPASS:
            passChance = 1.0 * this->attributes[SHORTPASSING] * 10;
            break;
        case THROUGHPASS:
            passChance = 0.75 * (this->attributes[LONGPASSING] * 1/3 + this->attributes[VISION] * 2/3) * 10;
            break;
        case LONGPASS:
            passChance = 0.8 * this->attributes[LONGPASSING] * 10;
            break;
    }

    if ((rand()%100) <= passChance) {
        for (int i = 0; i < sizeof(o) / sizeof(o[0]); i++) {
            if (o[i]->didIIntercept())
                return false;
        }
        return true;
    }
    return false;
}

bool Player::shouldIDribble(Player* o) {
    int dribbleChance = (this->attributes[DRIBBLING] * 3/4 + this->attributes[PACE] * 1/4) * 10 -
                        (o->attributes[DEFENDING] * 2/3 + o->attributes[PACE] * 1/3) * 4;
    if (rand()%100 <= dribbleChance)
        return true;
    return false;
}


Player::Player(std::string name, Team* team, Position position, int age, int pace, int finishing, int longShooting,
       int shortPassing, int longPassing, int vision, int dribbling, int defending) {
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

    calculateOverall();
}
