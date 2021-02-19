#include"Priest.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

using namespace std;

Priest::Priest(uint id,int x, int y, Team team):
    Player(id, x , y, team){
        HP = 150;
}

int Priest::getAttackDamage() const{
    return 0;
}

int Priest::getHealPower() const{
    return 50;
}

int Priest::getMaxHP() const{
    return 150;
}

vector<Goal> Priest::getGoalPriorityList(){
    vector<Goal> goals;
    goals.push_back(HEAL);
    goals.push_back(TO_ALLY);
    goals.push_back(CHEST);
    
    return goals;
}

const string Priest::getClassAbbreviation() const{
    if(this->getTeam() == BARBARIANS){
        return "PR";
    }
    else{
        return "pr";
    }
}

vector<Coordinate> Priest::getMoveableCoordinates(){
    Coordinate cord1(1,0), cord2(-1,0), cord3(0,1), cord4(0,-1);
    vector<Coordinate> move;
    move.push_back(this->coordinate + cord1);
    move.push_back(this->coordinate + cord2);
    move.push_back(this->coordinate + cord3);
    move.push_back(this->coordinate + cord4);
    move.push_back(this->coordinate + cord1 + cord3);
    move.push_back(this->coordinate + cord2 + cord3);
    move.push_back(this->coordinate + cord1 + cord4);
    move.push_back(this->coordinate + cord2 + cord4);

    return move;
}

vector<Coordinate> Priest::getAttackableCoordinates(){
    vector<Coordinate> attackable;
    return attackable;
}

vector<Coordinate> Priest::getHealableCoordinates(){
    Coordinate cord1(1,0), cord2(-1,0), cord3(0,1), cord4(0,-1);
    vector<Coordinate> heallable;
    heallable.push_back(this->coordinate + cord1);
    heallable.push_back(this->coordinate + cord2);
    heallable.push_back(this->coordinate + cord3);
    heallable.push_back(this->coordinate + cord4);
    heallable.push_back(this->coordinate + cord1 + cord3);
    heallable.push_back(this->coordinate + cord2 + cord3);
    heallable.push_back(this->coordinate + cord1 + cord4);
    heallable.push_back(this->coordinate + cord2 + cord4);

    return heallable;
}