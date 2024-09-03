#ifndef OPENFM_TACTIC_H
#define OPENFM_TACTIC_H
#include <iostream>

class Tactic {
private:
    std::string name;
    int mentality, tempo, passingStyle, workRate, width, pressingLine, strictness;
    int formation[3];

public:
    Tactic(int mentality, int tempo, int passingStyle,
           int workRate, int width, int pressingLine, int strictness, int defenders, int midfielders, int attackers);
    void setMentality(int newMentality);
    int getMentality() const;
    void setTempo(int newTempo);
    int getTempo() const;
    void setPassingStyle(int newPassingStyle);
    int getPassingStyle() const;
    void setWorkRate(int newWorkRate);
    int getWorkRate() const;
    void setWidth(int newWidth);
    int getWidth() const;
    void setPressingLine(int newPressingLine);
    int getPressingLine() const;
    void setStrictness(int newStrictness);
    int getStrictness() const;

    void changeFormation(int defenders, int midfielders, int attackers);
    int* getFormation();
};


#endif //OPENFM_TACTIC_H
