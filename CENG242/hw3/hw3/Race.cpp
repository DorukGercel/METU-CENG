#include "Race.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

using namespace std;

Race::Race() {
    this->head = NULL;
    this->last = NULL;
}

Race::Race(std::string race_name) {
    //cout <<"a"<<endl;
    this->race_name = race_name;
    Laptime laptime(Utilizer::generateAverageLaptime());
    this->average_laptime = laptime;
    this ->head = NULL;
    this -> last = head;
    this->numberOfCars = 0;
}

Race::Race(const Race &rhs) {
    //cout <<"7ss"<<endl;
    this->race_name = rhs.race_name;
    this->average_laptime = rhs.average_laptime;
    this->numberOfCars = rhs.numberOfCars;
    //cout <<"8ss"<<endl;
    if(rhs.head == NULL){
        head = NULL;
        last = NULL;
    }
    else {
        this->head = new Car(*(rhs.head));
        Car *p = head;
        Car *q = rhs.head;
        q = q->getNextCar();
        while (q) {
            //cout <<"100ss"<<endl;
            p->setNextCar(new Car(*q));
            cleanLapper(p->getHeadLap());
            p = p->getNextCar();
            q = q->getNextCar();
        }
        //cout <<"9ss"<<endl;
    }
}

Race::~Race() {/*
    Car* p;
    last = NULL;
    if(head == NULL){}
    while(head->getNextCar()){
        //cout << "aaa" << endl;
        p = head->getNextCar();
        head->setNextCar(p->getNextCar());
        p->deleteLaps();
        p->setNextCar(NULL);
        delete p;
    }

    head->deleteLaps();
    delete head;*/
}


void Race::cleanLapper(Laptime *laps) {
    if(laps != NULL) {
        Laptime *p = laps;
        Laptime *q = NULL;
        while (p->getNext()) {
            q = p->getNext();
            p->setNextLap(p->getNext()->getNext());
            delete q;
        }
        delete p;
    }
}

std::string Race::getRaceName() const {
    return race_name;
}

void Race::addCartoRace(Car &car) {
    Car* p = last;
    Car *q = new Car(car);
    if(p == NULL){
        //cout <<"sssaaa"<< endl;
        head = new Car(car);
        last = head;
    }
    else{
        last->setNextCar(q);
        last = last->getNextCar();
    }
    this->numberOfCars+=1;
    //cout << *(last) << endl;
}

int Race::getNumberOfCarsinRace() {
    return numberOfCars;
}

void Race::goBacktoLap(int lap) {
    Car* p = head;
    if(p->lapNoCounter() > lap) {
        while (p) {
            int i = 1;
            Laptime *q = p->getHeadLap();
            while (q && (i < lap)) {
                q = q->getNext();
                i+=1;
            }

            cleanLapper(q->getNext());

            q->setNextLap(NULL);

            p->setTotal(Laptime(p->LapAdder()));
            p->setFastest(*(p->findFastest()));
            p->setLatest(*(p->findLatest()));

            p = p->getNextCar();
        }
        bubbleSortCars();
        pointArranger();
    }
}

void Race::bubbleSortCars() {
    if(numberOfCars > 1) {
        Car* p;
        Car* tmp;
        p = new Car();
        p->setNextCar(head);
        Car* q = p;

        for (int i = 0; i < numberOfCars-1; i+=1) {
            q = p;
            //cout << i << endl;
            for (int j = 0; j < numberOfCars-i-1; j+=1) {
                //cout << j << endl;
                if ((q->getNextCar()->getTotal()).getLaptime() > (q->getNextCar()->getNextCar()->getTotal()).getLaptime()) {
                    tmp = q->getNextCar();
                    q->setNextCar(tmp->getNextCar());
                    tmp->setNextCar(tmp->getNextCar()->getNextCar());
                    q->getNextCar()->setNextCar(tmp);
                    q = q->getNextCar();
                } else {
                    //cout << "a" << endl;
                    q = q->getNextCar();
                }
                //cout << "a" << endl;
            }
            //cout << "b" << endl;
        }
        //cout << "c" << endl;
        //cout << *(head) <<endl;
        //cout << *(p) << endl;
        //cout << *(p->getNextCar())<<endl;
        head = p ->getNextCar();
        //cout << *(head) <<endl;
        //cout << *(last) << endl;
        //cout << "d" << endl;
    }
}

void Race::bubbleSortPoints() {
    if(numberOfCars > 1) {
        Car* p;
        Car* tmp;
        p = new Car("a");
        p->setNextCar(head);
        Car* q = p;

        for (int i = 0; i < numberOfCars-1; i+=1) {
            q = p;
            //cout << i << endl;
            for (int j = 0; j < numberOfCars-i-1; j+=1) {
                //cout << j << endl;
                if ((q->getNextCar()->getPoint()) < (q->getNextCar()->getNextCar()->getPoint())) {
                    tmp = q->getNextCar();
                    q->setNextCar(tmp->getNextCar());
                    tmp->setNextCar(tmp->getNextCar()->getNextCar());
                    q->getNextCar()->setNextCar(tmp);
                    q = q->getNextCar();
                } else {
                    //cout << "a" << endl;
                    q = q->getNextCar();
                }
                //cout << "a" << endl;
            }
            //cout << "b" << endl;
        }
        //cout << "c" << endl;
        //cout << *(head) <<endl;
        //cout << *(p) << endl;
        //cout << *(p->getNextCar())<<endl;
        head = p ->getNextCar();
        //cout << *(head) <<endl;
        //cout << *(last) << endl;
        //cout << "d" << endl;
    }
}



void Race::pointArranger() {
    Car* p = head;
    int i = 0;

    int holdr[10] = {25,18,15,12,10,8,6,4,2,1};

    while((i<10) && p){
        if(fastestCarName() == p->getDriverName()){
            p->setPoint(holdr[i] + 1);
            i+=1;
        }
        else{
            p->setPoint(holdr[i]);
            i+=1;
        }
        //cout << "xxxxxxx"<<endl;
        //cout << p->getPoint() << endl;
        //cout << "xxxxxxx"<<endl;
        p = p->getNextCar();
    }
    while(p){
        p->setPoint(0);
        p = p->getNextCar();
    }
}

void Race::operator++() {
    Car* p = head;
    //cout << *(head) << endl;
    //cout << "aaa" << endl;
    while(p){
        p->Lap(average_laptime);
        p = p->getNextCar();
        //cout <<"xxx"<<endl;
    }
    //cout << "bbb" << endl;
    bubbleSortCars();
    pointArranger();
}

void Race::operator--() {
    Car* p = head;
    while(p){
        p->oneLapRemover();
        p = p->getNextCar();
    }
    bubbleSortCars();
    pointArranger();
}

Car Race::operator[](const int car_in_position) {
    Car* p = head;
    int i = 0;
    for(i; i < car_in_position; i++){
        p = p->getNextCar();
    }
    //cout << *p << endl ;
    return *p;
}

Car Race::operator[](std::string driver_name) {
    Car* p = head;
    while(p){
        if(p->getDriverName() == driver_name){
            break;
        }
        p = p->getNextCar();
    }
    return *p;
}

Race& Race::operator=(const Race &rhs) {
    this->race_name = rhs.race_name;
    this->average_laptime = rhs.average_laptime;
    this->numberOfCars = rhs.numberOfCars;

    if(rhs.head == NULL){
        head = NULL;
        last = NULL;
    }
    else {
        this->head = new Car(*rhs.head);
        Car* p = head;
        Car* q = rhs.head;
        q = q->getNextCar();
        while(q){
            p->setNextCar(q);
            p->setLap(NULL);
            p = p->getNextCar();
            q = q->getNextCar();
        }
    }
}


std::ostream &operator<<(std::ostream &os, const Race &race) {
    //std::string fast_name = race.fastestCarName();
    Car* p = race.getHeadCar();
    int holdr[10] = {25,18,15,12,10,8,6,4,2,1};
    int a = 1;
    int b = 1;
    int z = a;
    int x = race.digit_maker();
    while(p){
        if(a <= 10) {
            for (int i = 0; x - b - i > 0; i++) {
                os << "0";
            }
            os << a << "--";
            os << *(p);

            if(race.fastestCarName() == p->getDriverName()){
                os << "--" << holdr[a-1];
                os << "--" << 1 << endl;
            } else{

                os << "--" << holdr[a-1] << endl;
            }
            a+=1;
            p = p->getNextCar();
            for(int d = b; d>0; d--){
                z = (a)/10;
            }
            if(z >= 1){b+=1;}
        } else{
            for (int i = 0; x - b - i > 0; i++) {
                os << "0";
            }

            os << a << "--";
            os << *(p) << endl;

            a+=1;
            p = p->getNextCar();
            for(int d = b; d>0; d--){
                z = (a)/10;
            }
            if(z >= 1){b+=1;}
        }
    }
    return os;


}

std::string Race::fastestCarName() const{
    Car* p = head;
    Car* tmp = head;
    while(p){
        if(p->getFastest() < tmp->getFastest()){
            tmp = p;
        }
        p = p->getNextCar();
    }
    return (tmp->getDriverName());
}

int Race::digit_maker() const{
    int i = 0;
    int x = numberOfCars;
    while(x >= 1){
        x = x/10;
        i+=1;
    }
    return i;
}

Car* Race::getHeadCar() const{
    return head;
}

Car* Race::getLastCar() const{
    return last;
}

void Race::setHeadCar(const Car* car) {
    this->head = new Car(*car);
}

void Race::setAvgLaptime(const Laptime laptime) {
    this->average_laptime = laptime;
}

void Race::setNumberOfCars(int no) {
    this->numberOfCars = no;
}