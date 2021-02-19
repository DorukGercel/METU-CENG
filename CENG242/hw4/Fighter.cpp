#include"Fighter.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

using namespace std;
Fighter::Fighter(uint id,int x, int y, Team team):
    Player(id, x, y, team){
    HP = 400;
}

int Fighter::getAttackDamage() const{
    return 100;
}

int Fighter::getHealPower() const{
    return 0;
}

int Fighter::getMaxHP() const{
    return 400;
}

vector<Goal> Fighter::getGoalPriorityList(){
    vector<Goal> goals;
    goals.push_back(ATTACK);
    goals.push_back(TO_ENEMY);
    goals.push_back(CHEST);
    
    return goals;
}

const string Fighter::getClassAbbreviation() const{
    if(this->getTeam() == BARBARIANS){
        return "FI";
    }
    else{
        return "fi";
    }
}

vector<Coordinate> Fighter::getMoveableCoordinates(){
    Coordinate cord1(1,0), cord2(-1,0), cord3(0,1), cord4(0,-1);
    vector<Coordinate> move;
    move.push_back(this->coordinate + cord1);
    move.push_back(this->coordinate + cord2);
    move.push_back(this->coordinate + cord3);
    move.push_back(this->coordinate + cord4);

    return move;
}

vector<Coordinate> Fighter::getAttackableCoordinates(){
    Coordinate cord1(1,0), cord2(-1,0), cord3(0,1), cord4(0,-1);
    vector<Coordinate> attackable;
    attackable.push_back(this->coordinate + cord1);
    attackable.push_back(this->coordinate + cord2);
    attackable.push_back(this->coordinate + cord3);
    attackable.push_back(this->coordinate + cord4);

    return attackable;
}

vector<Coordinate> Fighter::getHealableCoordinates(){
    vector<Coordinate> heallable;
    return heallable;
}