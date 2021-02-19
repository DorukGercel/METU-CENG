#include"Player.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

using namespace std;

Player::Player(uint id,int x, int y, Team team): 
    id(id), coordinate(x, y), team(team){}


uint Player::getID() const{
    return id;
} 

const Coordinate& Player::getCoord() const{
    return coordinate; 
}

int Player::getHP() const{
    return HP;
}

Team Player::getTeam() const{
    return team;
}

std::string Player::getBoardID(){
    string s;
    s = to_string(id);
    if(this->id < 10){
        return "0" + s; 
    }
    else{
        return s;
    }
}

bool Player::attack(Player* enemy){
    enemy->HP = (enemy->HP) - (this->getAttackDamage());
    
    cout << "Player " << this->getBoardID() << " attacked " << "Player " << enemy->getBoardID() << " (" << this->getAttackDamage() << ")" << endl;
    if(enemy->HP <= 0){
        return true;
    }
    else{
        return false;
    }
}

void Player::heal(Player* ally){
    ally->HP = (ally->HP) + (this->getHealPower());
    if(ally->HP > ally->getMaxHP()){
        ally->HP = ally->getMaxHP();
    }
    cout << "Player " << this->getBoardID() << " healed " << "Player " << ally->getBoardID() << endl;
}

void Player::movePlayerToCoordinate(Coordinate c){
    
    cout << "Player " << this->getBoardID() << " moved from " << this->coordinate << " to " << c << endl;
    this->coordinate.x = c.x;
    this->coordinate.y = c.y;
            
}

bool Player::isDead() const{
    if(HP <= 0){
        return true;
    }
    else{
        return false;
    }
}

Player::Player(Player& rhs):
    id(rhs.id), coordinate(rhs.coordinate.x,rhs.coordinate.y),HP(rhs.HP), team(rhs.team){}