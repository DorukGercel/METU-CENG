#include"Game.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/
using namespace std;

Game::Game(uint maxTurnNumber, uint boardSize, Coordinate chest):
    turnNumber(1), maxTurnNumber(maxTurnNumber), board(Board(boardSize, &players, chest)), no_knights(0), no_barbarians(0){
        vector<Player*> v;
        players = v;
}

Game::~Game(){}

void Game::addPlayer(int id, int x, int y, Team team, std::string cls){
    Player* p;
    if(cls == "ARCHER"){
        p = new Archer(id,x,y,team);
        if(team == KNIGHTS){
            no_knights+=1;
        }
        else{
            no_barbarians+=1;
        }
    }
    if(cls == "FIGHTER"){
        p = new Fighter(id,x,y,team);
        if(team == KNIGHTS){
            no_knights+=1;
        }
        else{
            no_barbarians+=1;
        }
    }
    if(cls == "PRIEST"){
        p = new Priest(id,x,y,team);
        if(team == KNIGHTS){
            no_knights+=1;
        }
        else{
            no_barbarians+=1;
        }
    }
    if(cls == "SCOUT"){
        p = new Scout(id,x,y,team);
        if(team == KNIGHTS){
            no_knights+=1;
        }
        else{
            no_barbarians+=1;
        }
    }
    if(cls == "TANK"){
        p = new Tank(id,x,y,team);
        if(team == KNIGHTS){
            no_knights+=1;
        }
        else{
            no_barbarians+=1;
        }
    }
    players.push_back(p);
}

bool Game::isGameEnded(){
    if(no_barbarians == 0){
        cout << "Game ended at turn "<<turnNumber-1<<". All barbarians dead. Knight victory."<<endl;
        return true;
    }
    if(no_knights == 0){
        cout << "Game ended at turn "<<turnNumber-1<<". All knights dead. Barbarian victory."<<endl;
        return true;
    }
    if(captureChest()){
        cout << "Game ended at turn "<< turnNumber-1<<". Chest captured. Barbarian victory."<<endl;
        return true;
    }
    if(turnNumber > maxTurnNumber){
        cout << "Game ended at turn "<<turnNumber-1<<". Maximum turn number reached. Knight victory.";
        return true;
    }
    else {return false;}        
}

bool Game::captureChest() const{
    if(board.barbOnChest()){
        return true;
    }
}

void Game::playTurn(){
    if(isGameEnded() == false){
    sortVector();
    cout << "Turn "<<turnNumber<<" has started."<<endl;
    for(int i = 0; i < players.size(); i++){
        if(players[i]->isDead()){
            playTurnForPlayer(players[i]);
            i-=1;
        }
        else{
            playTurnForPlayer(players[i]);
        }
    }
    turnNumber+=1;
    }
}

void Game::sortVector(){
    Player* tmp;
    for(int i = 0; i < players.size() - 1 ; i++){
        for(int j = 0; j < players.size() - i -1; j++){
            if(players[j]->getID() > players[j+1]->getID()){
                tmp = players[j+1];
                players[j+1] = players[j];
                players[j] = tmp;
            }
        }
    }
}

Goal Game::playTurnForPlayer(Player* player){
    if(player->isDead()){
        cout << "Player "<< player->getBoardID() <<" has died." << endl;

            if(player->getTeam() == KNIGHTS){no_knights-=1;}
            if(player ->getTeam() == BARBARIANS){no_barbarians-=1;}
            
            int x = 0;
        for(int i = 0; i < players.size(); i++){
            if(players[i]->getID() == player->getID()){
                x=i;
                break;
            }
        }
        for(x; x < players.size()-1; x++){
            players[x] = players[x+1];
        }
        players.resize(players.size()-1);
        
    }
    else{
        vector<Goal> v = player->getGoalPriorityList();
        return goalMaker(player, v[0], 0);
    }
}

vector<Player*> Game::attackableEnemies(Player* p){
    vector<Coordinate> tmp = p->getAttackableCoordinates();
    vector<Player*> v;
    Player* x;
    for(int i = 0; i < tmp.size();i++){
        x = board[tmp[i]];
        if(x){
            if(x->getTeam()!= p->getTeam()){
                v.push_back(x);
            }
        }
    }
    return v;
}

Player* Game::attackEnemy(std::vector<Player*> v){
    if(v.size() == 0){
        return NULL;
    }
    else{
        Player* p = NULL;
        for(int i = 0; i<v.size(); i++){
            if(p == NULL){
                p = v[i];
            }
            if(v[i]->getID() < p->getID()){
                p = v[i];
            }
        }
        return p;
    }
}

vector<Player*> Game::heallableAllies(Player* p){
    vector<Coordinate> tmp = p->getHealableCoordinates();
    vector<Player*> v;
    Player* x;
    for(int i = 0; i < tmp.size();i++){
        x = board[tmp[i]];
        if(x){
            if(x->getTeam()== p->getTeam()){
                v.push_back(x);
            }
        }
    }

    Player* player;
    for(int i = 0; i < v.size(); i++){
        for(int j = 0; j < v.size() - i -1; j++){
            if(v[j]->getID() > v[j+1]->getID()){
                player = v[j+1];
                v[j+1] = v[j];
                v[j] = player;
            }
        }
    }

    return v;
}

vector<Coordinate> Game::goToChestCoords(Player* p){
    vector<Coordinate> v;
    vector<Coordinate> tmp = p->getMoveableCoordinates();
    for(int i = 0; i < tmp.size(); i++){
        if(board.chestCord()-tmp[i] < board.chestCord() - p->getCoord()){
            v.push_back(tmp[i]);
        }
    }
    v.push_back(p->getCoord());
    return v;
}

Coordinate Game::goChestCoord(std::vector<Coordinate> v){
    Coordinate cord(v[v.size()-1].x,v[v.size()-1].y);
    for(int i = 0; i<v.size(); i++){
        if((board.chestCord() - v[i]) < (board.chestCord() - cord) && !(board.isPlayerOnCoordinate(v[i]))){
                cord = v[i];
        }
    }
    return cord;
}

Player* Game::getCloseEnemy(Player* p){
    Player* x = NULL;
    for(int i = 0; i < players.size(); i++){
        if(players[i]->getTeam() != p->getTeam()){
            if(x == NULL){
                   x = players[i]; 
            }
            if(players[i]->getCoord() - p->getCoord() == x->getCoord() - p->getCoord()){
                if(players[i]->getID() < x->getID()){
                    x = players[i];
                }
            }
            if(players[i]->getCoord() - p->getCoord() < x->getCoord() - p->getCoord()){
                    x = players[i];
            }
        }
    }
    return x;
}
//enemy null geldi onla ilgilen .)))))
vector<Coordinate> Game::goToEnemyCoords(Player* p, Player* enemy){
    vector<Coordinate> v;
    if(enemy != NULL){
        vector<Coordinate> tmp = p->getMoveableCoordinates();
        for(int i = 0; i < tmp.size(); i++){
            if(enemy->getCoord()-tmp[i] < enemy->getCoord() - p->getCoord()){
                v.push_back(tmp[i]);
            }
        }
    }
    v.push_back(p->getCoord());
    return v;
}

Coordinate Game::goEnemyCoord(std::vector<Coordinate> v, Player* enemy){
    Coordinate cord(v[v.size()-1].x,v[v.size()-1].y);
    if(enemy != NULL){
        for(int i = 0; i<v.size(); i++){
            if((enemy->getCoord() - v[i]) < (enemy->getCoord() - cord) && !(board.isPlayerOnCoordinate(v[i]))){
                    cord = v[i];
            }
        }
    }
    return cord;
}

Player* Game::getCloseAlly(Player* p){
    Player* x = NULL;
    for(int i = 0; i < players.size(); i++){
        if(players[i]->getTeam() == p->getTeam() && players[i]->getID() != p->getID()){
            if(x == NULL){
                   x = players[i]; 
            }
            if(players[i]->getCoord() - p->getCoord() == x->getCoord() - p->getCoord()){
                if(players[i]->getID() < x->getID()){
                    x = players[i];
                }
            }
            if(players[i]->getCoord() - p->getCoord() < x->getCoord() - p->getCoord()){
                    x = players[i];
            }
        }
    }
    return x;
}

vector<Coordinate> Game::goToAllyCoords(Player* p, Player* ally){
    vector<Coordinate> v;
    if(ally != NULL){
        vector<Coordinate> tmp = p->getMoveableCoordinates();
        for(int i = 0; i < tmp.size(); i++){
            if(ally->getCoord()-tmp[i] < ally->getCoord() - p->getCoord()){
                v.push_back(tmp[i]);
            }
        }
    }
    v.push_back(p->getCoord());
    return v;
}

Coordinate Game::goAllyCoord(std::vector<Coordinate> v, Player* ally){
    Coordinate cord(v[v.size()-1].x,v[v.size()-1].y);
    if(ally != NULL){
        for(int i = 0; i<v.size(); i++){
            if((ally->getCoord() - v[i]) < (ally->getCoord() - cord) && !(board.isPlayerOnCoordinate(v[i]))){
                    cord = v[i];
            }
        }
    }
    return cord;
}

Goal Game::goalMaker(Player* p, Goal goal, int no_Priority){
    if(goal == NO_GOAL){return NO_GOAL;}
    if(goal == ATTACK){
        Player* enemy = attackEnemy(attackableEnemies(p));
        if(enemy != NULL){
            p->attack(enemy);
            return goal;
        }
        else{
            vector<Goal> v = p->getGoalPriorityList();
            if(no_Priority + 1 != v.size()){
                return goalMaker(p, v[no_Priority+1], no_Priority+1);
            }
            else{
                return goalMaker(p, NO_GOAL, no_Priority+1);
            }
        }
    }
    if(goal == HEAL){
        vector<Player*> allies = heallableAllies(p);
        if(allies.size() != 0){
            for(int i = 0; i < allies.size(); i++){
                p->heal(allies[i]);
            }
            return goal;
        }
        else{
            vector<Goal> v = p->getGoalPriorityList();
            if(no_Priority + 1 != v.size()){
                return goalMaker(p, v[no_Priority+1], no_Priority+1);
            }
            else{
                return goalMaker(p, NO_GOAL, no_Priority+1);
            }
        }
    }
    if(goal == CHEST){
        Coordinate cord = goChestCoord(goToChestCoords(p));
        if(cord != p->getCoord()){
            p->movePlayerToCoordinate(cord);
            return goal;
        }
        else{
            vector<Goal> v = p->getGoalPriorityList();
            if(no_Priority + 1 != v.size()){
                return goalMaker(p, v[no_Priority+1], no_Priority+1);
            }
            else{
                return goalMaker(p, NO_GOAL, no_Priority+1);
            }
        }

    }
    if(goal == TO_ENEMY){
        Coordinate cord = goEnemyCoord(goToEnemyCoords(p, getCloseEnemy(p)),getCloseEnemy(p));
        if(cord != p->getCoord()){
            p->movePlayerToCoordinate(cord);
            return goal;
        }
        else{
            vector<Goal> v = p->getGoalPriorityList();
            if(no_Priority + 1 != v.size()){
                return goalMaker(p, v[no_Priority+1], no_Priority+1);
            }
            else{
                return goalMaker(p, NO_GOAL, no_Priority+1);
            }
        }
    }
    if(goal == TO_ALLY){
        Coordinate cord = goAllyCoord(goToAllyCoords(p,getCloseAlly(p)),getCloseAlly(p));
        if(cord != p->getCoord()){
            p->movePlayerToCoordinate(cord);
            return goal;
        }
        else{
            vector<Goal> v = p->getGoalPriorityList();
            if(no_Priority + 1 != v.size()){
                return goalMaker(p, v[no_Priority+1], no_Priority+1);
            }
            else{
                return goalMaker(p, NO_GOAL, no_Priority+1);
            }
        }
    }
}

