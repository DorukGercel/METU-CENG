#include "Laptime.h"
#include "Car.h"
#include "Race.h"
#include "Championship.h"
#include <iostream>

using namespace std;

int main(){

    /**********************************************/
    /************** LAPTIME TEST ******************/
    /**********************************************/
    Laptime small(87625);
    Laptime large(188524);

    if(small<large)
        cout<<small<<endl<<endl;
    else
        cout<<"ERROR"<<endl<<endl;

    if(large>small)
        cout<<large<<endl<<endl;
    else
        cout<<"ERROR"<<endl<<endl;

    small = large+small;

    cout<<"small "<<small<<endl;
    cout<<"large "<<large<<endl;

    /**********************************************/
    /************** CAR AND RACE TEST *************/
    /**********************************************/

    cout<<"CREATING CARS"<<endl<<endl;
    Car alonso("Fernando Alonso");
    Car hamilton("Lewis Hamilton");
    Car vettel("Sebastian Vettel");
    Car leclerc("Charles Leclerc");
    Car bottas("Valtteri Bottas");
    Car verstappen("Max Verstappen");
    Car gasly("Pierre Gasly");
    Car hulkenberg("Nico Hulkenberg");
    Car ricciardo("Daniel Ricciardo");
    Car raikkonen("Kimi Raikkonen");
    Car giovinazzi("Antonio Giovinazzi");


    cout<<"CREATING a RACE NAMED BAKU"<<endl<<endl;
    Race baku("baku");
    cout<<"RACE CREATED"<<endl<<endl;
    baku.addCartoRace(alonso);
    baku.addCartoRace(hamilton);
    baku.addCartoRace(vettel);
    baku.addCartoRace(leclerc);
    baku.addCartoRace(bottas);
    baku.addCartoRace(verstappen);
    baku.addCartoRace(gasly);
    baku.addCartoRace(hulkenberg);
    baku.addCartoRace(ricciardo);
    baku.addCartoRace(raikkonen);
    baku.addCartoRace(giovinazzi);

    cout<<"NUMBER OF CARS"<<endl<<endl;
    cout<<baku.getNumberOfCarsinRace()<<endl;
/*
    gasly.Lap(small);
    gasly.Lap(small);
    gasly.Lap(large);
    gasly.Lap(large);
    cout << gasly << endl;
*/
  cout<<"PREFIX TEST"<<endl<<endl;
    ++baku;

    cout<<"OSTREAM TEST"<<endl<<endl;;
    cout<<baku<<endl<<endl;


    for(int i = 0;i<55;i++) {
        //cout << "a";
        ++baku;
    }

    cout<<"AFTER MANY LAPS"<<endl<<endl;
    cout<<baku<<endl<<endl;

    for(int i = 0;i<35;i++) {
        //cout << "a";
        --baku;
    }

    cout<<"AFTER MANY LAPS"<<endl<<endl;
    cout<<baku<<endl<<endl;

    baku.goBacktoLap(15);

    cout<<"Aaaaa MANY LAPS"<<endl<<endl;
    cout<<baku<<endl<<endl;

    cout << baku.fastestCarName()<<endl;

    Car* p = baku.getHeadCar();

    cout << p->lapNoCounter() << endl;

    //cout << *p << endl;

    cout << baku[0] << endl;
    //cout << baku[0] << endl;

    //cout << baku ["Fernando Alonso"] << endl;


    //cout << 100 << endl;
/*
    cout << "ccc" << endl;
    cout << "xxx" << endl;

*/
  //  cout<<"AFTER MANY LAPS"<<endl<<endl;
   // cout<<baku<<endl<<endl;

   //cout <<"1**"<<endl;
   cout<<"*aaa***************"<< endl;
   Championship championship;
    cout<<"*aaa***************"<< endl;
    //cout <<"a**"<<endl;
   championship.addNewRace(baku);
    cout<<"*aaa***************"<< endl;
    //cout <<"b**"<<endl;
   championship.addNewRace("china");
    cout<<"*aaa***************"<< endl;
    //cout <<"c**"<<endl;
   championship.addNewRace("momo");
    cout<<"*aaa***************"<< endl;
    championship.removeRace("momo");
    cout << "000" << endl;
    return 0;
}