//
// Created by kubak on 30.06.2024.
//

#include "Player.h"

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

bool Player::isPassCompleted(Player* p) {
    int passChance =
}
