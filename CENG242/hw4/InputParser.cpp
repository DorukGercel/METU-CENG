#include"InputParser.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

using namespace std;

Game* InputParser::parseGame(){
    uint boardSize, chest_x, chest_y, maxTurnNo, noPlayers;
    cin >> boardSize;
    cin >> chest_x;
    cin >> chest_y;
    cin >> maxTurnNo;
    cin >> noPlayers;

    Coordinate chest(chest_x, chest_y);
    Game* game = new Game(maxTurnNo, boardSize, chest);

    for(int i = 0; i < noPlayers; i++){
        uint id, x, y;
        string type,str;
        Team team;

        cin >> id;
        cin >> type;
        cin >> str;
        cin >> x;
        cin >> y;


        if(str == "BARBARIAN"){
            team = BARBARIANS;
        }
        if (str == "KNIGHT")
        {
            team = KNIGHTS;
        }

        game->addPlayer(id, x, y, team, type);
        
    }

    return game;
}
