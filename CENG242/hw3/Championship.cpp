#include "Championship.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

using namespace std;
Championship::Championship() {
    //cout <<"1"<<endl;
    numeberOfRaces = 0;
    //cout <<"2"<<endl;
}

Championship::Championship(const Championship &rhs) {
    races.resize(rhs.races.size());
    //cout <<"3"<<endl;
    numeberOfRaces = rhs.races.size();
    //cout <<"4"<<endl;
    original_race = rhs.original_race;
    races = rhs.races;
}

Championship::~Championship() {}

void Championship::addNewRace(Race &race) {
    Race new_race = Race(race);

    Car* p = new_race.getHeadCar();
    Car* q = race.getHeadCar();

    while(q){
        p->setTotal(q->getTotal());
        p->setFastest(q->getFastest());
        p->setLatest(q->getLatest());

        Laptime* x = q->getHeadLap();
        p->setLap(x);
        Laptime* y = p->getHeadLap();
        x = x->getNext();
        while(x){
            y->setNextLap(x);
            y = y->getNext();
            x = x->getNext();
        }
        p = p->getNextCar();
        q = q->getNextCar();
    }

    numeberOfRaces+=1;

    races.resize(numeberOfRaces);
    //new_race.bubbleSortCars();
    original_race = new_race;
    races[0] = new_race;


}

void Championship::addNewRace(std::string race_name) {
    Race new_race = Race(race_name);

    new_race.setNumberOfCars(original_race.getNumberOfCarsinRace());

    Car* p = this->original_race.getHeadCar();

    new_race.setHeadCar(new Car(p->getDriverName()));
    Car* q = new_race.getHeadCar();


    p = p->getNextCar();

    while(p){
        q->setNextCar(new Car(p->getDriverName()));
        p = p->getNextCar();
        q = q->getNextCar();
    }

    numeberOfRaces+=1;
    races.push_back(new_race);
}

void Championship::removeRace(std::string race_name) {
    for(int i = 0; i < numeberOfRaces; i++){
        if(races[i].getRaceName() == race_name){
            races.erase(races.begin()+i);
            break;
        }
    }
}

void Championship::addLap(std::string race_name) {
    for(int i = 0; i < numeberOfRaces; i++){
        if(races[i].getRaceName() == race_name){
            ++races[i];
            break;
        }
    }
}

Race Championship::operator[](std::string race_name) {
    for(int i = 0; i < numeberOfRaces; i++){
        if(races[i].getRaceName() == race_name){
            return races[i];
        }
    }
}

Race Championship::pointAdder() const{

    Race* new_race = new Race(races[0]);
    Car* p = this->races[0].getHeadCar();

    new_race->setHeadCar(new Car(p->getDriverName()));
    Car* q = new_race->getHeadCar();

    p = p->getNextCar();

    while(p){
        q->setNextCar(new Car(p->getDriverName()));
        p = p->getNextCar();
        q = q->getNextCar();
    }

    Car* x = new_race->getHeadCar();

    int total;
    while(x){
        total = 0;
        string name = x->getDriverName();
        //cout << x->getDriverName() << endl << "xxxxxxxxxxx"<<endl;
        for(int i = 0; i < races.size(); i++){
            Car* z = races[i].getHeadCar();
            while(z){
                if(z->getDriverName() == name){
                    total = total + z->getPoint();
                    //break;
                }
                z = z->getNextCar();
            }
            //cout << z->getPoint() << endl << "xxxxxxxxxxx"<<endl;
            //cout <<"xxxxxxxxxxxxxxx"<<endl;
            //cout<< total <<endl << endl;
            //cout <<"xxxxxxxxxxxxxxx"<<endl;
        }
        x->setPoint(total);
        x = x->getNextCar();
    }
    //cout << *new_race << endl << "---------"<<endl;
    return *new_race;
}

std::ostream &operator<<(std::ostream &os, const Championship &championship) {
    //cout << "aaaaa" << endl;
    Race final_results = championship.pointAdder();
    //cout << championship.pointAdder() << endl;
    final_results.bubbleSortPoints();
    //cout << final_results << endl;
    Car* p = final_results.getHeadCar();

    int a = 1;
    int b = 1;
    int z = a;
    int x = final_results.digit_maker();

    while(p){
        for (int i = 0; x - b - i > 0; i++) {
            os << "0";
        }

        os << a << "-";

        string name = p->getDriverName();
        string surname = name.substr(name.rfind(" ")+1);

        os << surname[0] << (char)(surname[1]-32) << (char)(surname[2]-32);

        os <<"--"<<p->getPoint()<< endl;

        a+=1;
        p = p->getNextCar();
        for(int d = b; d>0; d--){
            z = (a)/10;
        }
        if(z >= 1){b+=1;}
    }
    return os;
}

/*
string name = car.getDriverName();
string surname = name.substr(name.rfind(" ")+1);

os << surname[0] << (char)(surname[1]-32) << (char)(surname[2]-32);
*/

/*
 *
 int a = 1;
    int b = 1;
    int z = a;
    int x = race.digit_maker();
 *
 *
 *
for (int i = 0; x - b - i > 0; i++) {
                os << "0";
            }

            os << a << "--";
            os << *(p) << endl;

            p = p->getNextCar();
            for(int d = b; d>0; d--){
                z = (a)/10;
            }
            if(z >= 1){b+=1;}
        }
 */