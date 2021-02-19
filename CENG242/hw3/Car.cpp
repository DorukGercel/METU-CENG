#include "Car.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

using namespace std;

void Car::deleteLaps() {
    //cout << "1aaaaaaaaa" << endl;
    Laptime *p;
    //cout << "2aaaaaaaaa" << endl;
    if (head == NULL){}
    else {
        while (head->getNext()) {
            p = head->getNext();
            head->setNextLap(p->getNext());
            //cout << "xaaaaaaaaa" << endl;
            p->setNextLap(NULL);
            delete p;
        }
        //cout << "3aaaaaaaaa" << endl;
        //cout << "4aaaaaaaaa" << endl;
    }
}

int Car::LapAdder() {
    Laptime *p = head;
    int sum = 0;
    while(p!=NULL){
        sum += p->getLaptime();
        p = p->getNext();
    }
    return sum;
}

Car::Car(std::string driver_name) {
    this->driver_name = driver_name;
    this->performance = Utilizer::generatePerformance();
    this->head = NULL;
    this->next = NULL;
    this->total_laptime = Laptime(LapAdder());
    this->fastest_laptime = Laptime(0);
    this->latest_laptime = Laptime(0);
    this->point = 0;
}

Car::Car(const Car &rhs) {
    //cout <<"az"<<endl;
    this->driver_name = rhs.driver_name;
    this->performance = rhs.performance;
    //cout <<"qqqaa" <<endl;
    this->next = NULL;
    //cout <<"aaa" <<endl;
    if(rhs.head == NULL){
        this->head = NULL;
    }
    else {
        this->head = new Laptime(*rhs.head);
        Laptime* p = head;
        Laptime* q = rhs.head;
        q = q->getNext();
        while (q) {
            p->setNext(*(new Laptime(*q)));
            p = p->getNext();
            q = q->getNext();
        }
    }
    this->fastest_laptime = rhs.fastest_laptime;
    this->latest_laptime = rhs.latest_laptime;
    this->total_laptime = rhs.total_laptime;
    this->point = rhs.point;
}

Car::Car() {
    this->head = NULL;
    this->next = NULL;
    this->total_laptime = Laptime(LapAdder());
    this->fastest_laptime = Laptime(0);
    this->latest_laptime = Laptime(0);
    this->point = 0;
}

Car::~Car() {
    deleteLaps();
}

std::string Car::getDriverName() const {
    return driver_name;
}

double Car::getPerformance() const {
    return performance;
}

void Car::addCar(Car *next) {
    this->next = next;
}

bool Car::operator<(const Car &rhs) const {
    return total_laptime.getLaptime() < rhs.total_laptime.getLaptime();
}

bool Car::operator>(const Car &rhs) const {
    return total_laptime.getLaptime() > rhs.total_laptime.getLaptime();
}

/*
Car* p = head;
    int i = 0;
    for(i; i < car_in_position; i++){
        p = p->getNextCar();
    }
    //cout << *p << endl ;
    return *p;
*/

Laptime Car::operator[](const int lap) const {
    Laptime* p = head;
    int i = 0;
    while(i < lap  && p != NULL){
        p = p->getNext();
        i++;
    }
    //cout << *p << endl ;
    return *p;
}

void Car::Lap(const Laptime &average_laptime) {
    Laptime *p = head;
    int perf = (Utilizer::generateLaptimeVariance(performance));
    //std::cout << "a" << endl;
    if (p == NULL){
        head = new Laptime(perf +(average_laptime.getLaptime()));
        head->getNext();
        //std::cout << "hhhhh" << endl;
    }
    else {
        //std::cout << "a" << endl;
        while (p->getNext()) {
            p = p->getNext();

        }
        //std::cout << "a" << endl;
        p->setNext(Laptime(perf +(average_laptime.getLaptime())));
        //std::cout << "cccc" << endl;
    }
    //std::cout << "a" << endl;
    this->total_laptime = Laptime(LapAdder());
    this->fastest_laptime = Laptime(*(findFastest()));
    this->latest_laptime = Laptime(*(findLatest()));
}

void Car::oneLapRemover() {
    Laptime *p = head;
    if(p == NULL){}
    else if(p->getNext() == NULL){
        delete p;
        head = NULL;
    }
    else{
        while(p->getNext()->getNext()){
            p = p->getNext();
        }
        delete p->getNext();
        p->setNextLap(NULL);
    }
    this->total_laptime = Laptime(LapAdder());
    this->fastest_laptime = Laptime(*(findFastest()));
    this->latest_laptime = Laptime(*(findLatest()));
}

Laptime* Car::getHeadLap() const {
    return head;
}

Car* Car::getNextCar() const{
    return next;
}

Laptime* Car::findFastest() const{
    Laptime* p = head;
    Laptime* q = head;
    while(p){
        if(p->getLaptime() < q->getLaptime()){
            q = p;
        }
        p = p->getNext();
    }
    return q;

}

Laptime* Car::findLatest() const{
    Laptime* p = head;
    if(p != NULL) {
        while (p->getNext()) {
            p = p->getNext();
        }
        return p;
    }
}

std::ostream &operator<<(std::ostream &os, const Car &car) {
    string name = car.getDriverName();
    string surname = name.substr(name.rfind(" ")+1);

    os << surname[0] << (char)(surname[1]-32) << (char)(surname[2]-32);
    os << "--";
    os << car.getLatest();
    os << "--";
    os << car.getFastest();
    os << "--";
    os << car.getTotal();


    return os;
}

Laptime Car::getTotal() const {
    return total_laptime;
}

Laptime Car::getLatest() const {
    return latest_laptime;
}

Laptime Car::getFastest() const {
    return fastest_laptime;
}

void Car::setName(string name) {
    this->driver_name = name;
}

void Car::setPerformance(double performance) {
    this->performance = performance;
}

void Car::setNext(const Car &next) {
    this -> next = new Car(next);
}

void Car::setNextCar(Car *next) {
    this -> next =  next;
}

void Car::setLap(Laptime *head) {
    this->head = head;
}

void Car::setTotal(const Laptime total)  {
    this->total_laptime = total;
}

void Car::setFastest(const Laptime fastest) {
    this->fastest_laptime = fastest;
}

void Car::setLatest(const Laptime latest) {
    this->latest_laptime = latest;
}

int Car::lapNoCounter() const {
    Laptime* p = this->head;
    int i = 0;
    while (p){
        p = p->getNext();
        i+=1;
    }
    return i;
}

int Car::getPoint() const{
    return point;
}

void Car::setPoint(int point) {
    this->point = point;
}