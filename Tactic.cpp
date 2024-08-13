#include "Tactic.h"

Tactic::Tactic(const std::string &name, int mentality, int tempo, int passingStyle, int workRate, int width,
               int pressingLine, int strictness, int defenders, int midfielders, int attackers) {
    this->name = name;
    this->mentality = mentality;
    this->tempo = tempo;
    this->passingStyle = passingStyle;
    this->workRate = workRate;
    this->width = width;
    this->pressingLine = pressingLine;
    this->strictness = strictness;
    formation[0] = defenders;
    formation[1] = midfielders;
    formation[2] = attackers;
}

void Tactic::setMentality(int newMentality) {
    if (newMentality >= 0 && newMentality < 5)
        this->mentality = newMentality;
}

int Tactic::getMentality() const {
    return this->mentality;
}

void Tactic::setTempo(int newTempo) {
    if (newTempo >= 0 && newTempo < 3)
        this->tempo = newTempo;
}

int Tactic::getTempo() const {
    return this->tempo;
}

void Tactic::setPassingStyle(int newPassingStyle) {
    if (newPassingStyle >= 0 && newPassingStyle < 4)
        this->passingStyle = newPassingStyle;
}

int Tactic::getPassingStyle() const {
    return this->passingStyle;
}

void Tactic::setWorkRate(int newWorkRate) {
    if (newWorkRate >= 0 && newWorkRate < 3)
        this->workRate = newWorkRate;
}

int Tactic::getWorkRate() const {
    return this->workRate;
}

void Tactic::setWidth(int newWidth) {
    if (newWidth >= 0 && newWidth < 3)
        this->width = newWidth;
}

int Tactic::getWidth() const {
    return this->width;
}

void Tactic::setPressingLine(int newPressingLine) {
    if (newPressingLine >= 0 && newPressingLine < 3)
        this->pressingLine = newPressingLine;
}

int Tactic::getPressingLine() const {
    return this->pressingLine;
}

void Tactic::setStrictness(int newStrictness) {
    if (newStrictness >= 0 && newStrictness < 3)
        this->strictness = newStrictness;
}

int Tactic::getStrictness() const {
    return this->strictness;
}

void Tactic::changeFormation(int defenders, int midfielders, int attackers)  {
    if (defenders + midfielders + attackers == 10) {
        formation[0] = defenders;
        formation[1] = midfielders;
        formation[2] = attackers;
    }
}

int* Tactic::getFormation() {
    return formation;
}