#include"Archer.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

using namespace std;
Archer::Archer(uint id,int x, int y, Team team):
    Player(id, x, y, team){
    HP = 200;
}


int Archer::getAttackDamage() const{
    return 50;
}

int Archer::getHealPower() const{
    return 0;
}

int Archer::getMaxHP() const{
    return 200;
}

vector<Goal> Archer::getGoalPriorityList(){
    vector<Goal> goals;
    goals.push_back(ATTACK);
    
    return goals;
}

const string Archer::getClassAbbreviation() const{
    if(this->getTeam() == BARBARIANS){
        return "AR";
    }
    else{
        return "ar";
    }
}

vector<Coordinate> Archer::getMoveableCoordinates(){
    vector<Coordinate> move;
    return move;
}

vector<Coordinate> Archer::getAttackableCoordinates(){
    Coordinate cord1(1,0), cord2(-1,0), cord3(0,1), cord4(0,-1);
    vector<Coordinate> attackable;
    attackable.push_back(this->coordinate + cord1);
    attackable.push_back(this->coordinate + cord2);
    attackable.push_back(this->coordinate + cord3);
    attackable.push_back(this->coordinate + cord4);
    attackable.push_back(this->coordinate + cord1 + cord1);
    attackable.push_back(this->coordinate + cord2 + cord2);
    attackable.push_back(this->coordinate + cord3 + cord3);
    attackable.push_back(this->coordinate + cord4 + cord4);

    return attackable;
}

vector<Coordinate> Archer::getHealableCoordinates(){
    vector<Coordinate> heallable;
    return heallable;
}