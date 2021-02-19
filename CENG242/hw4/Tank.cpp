#include"Tank.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

using namespace std;
Tank::Tank(uint id,int x, int y, Team team):
    Player(id, x, y, team){
        HP =1000;
}

int Tank::getAttackDamage() const{
    return 25;
}

int Tank::getHealPower() const{
    return 0;
}

int Tank::getMaxHP() const{
    return 1000;
}

vector<Goal> Tank::getGoalPriorityList(){
    vector<Goal> goals;
    goals.resize(3);
    goals[0] = TO_ENEMY;
    goals[1] = ATTACK;
    goals[2] = CHEST;
    
    return goals;
}

const string Tank::getClassAbbreviation() const{
    if(this->getTeam() == BARBARIANS){
        return "TA";
    }
    else{
        return "ta";
    }
}

vector<Coordinate> Tank::getMoveableCoordinates(){
    Coordinate cord1(1,0), cord2(-1,0), cord3(0,1), cord4(0,-1);
    vector<Coordinate> move;
    move.push_back(this->coordinate + cord1);
    move.push_back(this->coordinate + cord2);
    move.push_back(this->coordinate + cord3);
    move.push_back(this->coordinate + cord4);
    
    return move;
}

vector<Coordinate> Tank::getAttackableCoordinates(){
    Coordinate cord1(1,0), cord2(-1,0), cord3(0,1), cord4(0,-1);
    vector<Coordinate> attackable;
    attackable.push_back(this->coordinate + cord1);
    attackable.push_back(this->coordinate + cord2);
    attackable.push_back(this->coordinate + cord3);
    attackable.push_back(this->coordinate + cord4);
    
    return attackable;
}

vector<Coordinate> Tank::getHealableCoordinates(){
    vector<Coordinate> heallable;
    
    return heallable;
}