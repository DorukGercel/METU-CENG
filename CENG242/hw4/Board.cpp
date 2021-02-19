#include"Board.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

using namespace std;

Board::Board(uint _size, std::vector<Player*>* _players, Coordinate chest):
    size(_size), players(_players), chest(chest){}

Board::~Board(){}

bool Board::isCoordinateInBoard(const Coordinate& c){
    if(c.x > size-1 || c.x < 0 || c.y > size-1 || c.y < 0){
        return false;
    }
    else{
        return true;
    }
}

bool Board::isPlayerOnCoordinate(const Coordinate& c){
    vector<Player*> p  = *(players);
    for(int i = 0; i < p.size(); i++){
        if(p[i]->getCoord() == c){
            return true;
        }
    }
    return false;
}

Player* Board::operator[](const Coordinate& c){
    vector<Player*> p = *(players);
    for(int i = 0; i < p.size(); i++){
        if(p[i]->getCoord() == c){
            return p[i];
        }
    }
    return NULL;
}

Player* Board::returnPlayer(const Coordinate& c){
    vector<Player*> p = *(players);
    for(int i = 0; i < p.size(); i++){
        if(p[i]->getCoord() == c){
            return p[i];
        }
    }
    return NULL;
}

Coordinate Board::getChestCoordinates(){
    return chest;
}

Coordinate Board::chestCord() const{
    return chest;
}

void Board::printBoardwithID(){
    Coordinate cord(0,0);
    for(int i = 0; i < size; i++){
        cord.y = i;
        for(int j = 0; j < size; j++){
            cord.x = j;
            Player* p = returnPlayer(cord);
            if(p){
                cout << p-> getBoardID() << " ";
            }
            else if(chest == cord){
                cout << "Ch ";
            }
            else{
                cout << "__ ";
            }
        }
        cout<<endl;
    }
}

void Board::printBoardwithClass(){
    Coordinate cord(0,0);
    for(int i = 0; i < size; i++){
        cord.y = i;
        for(int j = 0; j < size; j++){
            cord.x = j;
            Player* p = returnPlayer(cord);
            if(p){
                cout << p-> getClassAbbreviation() << " ";
            }
            else if(chest == cord){
                cout << "Ch ";
            }
            else{
                cout << "__ ";
            }
        }
        cout<<endl;
    }
}

bool Board::barbOnChest() const{
    vector<Player*> p = *players;
    for(int i = 0; i < p.size(); i++){
        if(p[i]->getCoord() == chest && p[i]->getTeam() == BARBARIANS){
            return true;
        }
    }
    return false;
}
