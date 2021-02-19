#include"Scout.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

using namespace std;

Scout::Scout(uint id,int x, int y, Team team):
    Player(id, x, y, team){
        HP = 125;
}

int Scout::getAttackDamage() const{
    return 25;
}

int Scout::getHealPower() const{
    return 0;
}

int Scout::getMaxHP() const{
    return 125;
}

vector<Goal> Scout::getGoalPriorityList(){
    vector<Goal> goals;
    goals.resize(3);
    goals[0] = CHEST;
    goals[1] = TO_ALLY;
    goals[2] = ATTACK;

    return goals;
}

const string Scout::getClassAbbreviation() const{
    if(this->getTeam() == BARBARIANS){
        return "SC";
    }
    else{
        return "sc";
    }
}

vector<Coordinate> Scout::getMoveableCoordinates(){
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

vector<Coordinate> Scout::getAttackableCoordinates(){
    Coordinate cord1(1,0), cord2(-1,0), cord3(0,1), cord4(0,-1);
    vector<Coordinate> attackable;
    attackable.push_back(this->coordinate + cord1);
    attackable.push_back(this->coordinate + cord2);
    attackable.push_back(this->coordinate + cord3);
    attackable.push_back(this->coordinate + cord4);
    attackable.push_back(this->coordinate + cord1 + cord3);
    attackable.push_back(this->coordinate + cord2 + cord3);
    attackable.push_back(this->coordinate + cord1 + cord4);
    attackable.push_back(this->coordinate + cord2 + cord4);

    return attackable;
}

vector<Coordinate> Scout::getHealableCoordinates(){
    vector<Coordinate> heallable;
    
    return heallable;
}