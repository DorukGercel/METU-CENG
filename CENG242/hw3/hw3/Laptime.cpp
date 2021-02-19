#include "Laptime.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/

using namespace std;

Laptime::Laptime() {}

Laptime::Laptime(int laptime) {
    this->laptime = laptime;
    this->next = NULL;
}

Laptime::Laptime(const Laptime &rhs) {
    laptime = rhs.laptime;
    //std::cout << "bbbaaaa" << endl;
    next = NULL ;
    //std::cout << "ccccaa" << endl;
}

Laptime::~Laptime() {/*
    next = NULL;*/
}

void Laptime::addLaptime(Laptime *next) {
    this->next = next;
}

bool Laptime::operator<(const Laptime &rhs) const {
    return (this->laptime)<(rhs.laptime);
}

bool Laptime::operator>(const Laptime &rhs) const {
    return (this->laptime)>(rhs.laptime);
}

Laptime& Laptime::operator+(const Laptime &rhs) {
    this->next = new Laptime(rhs);
    this->laptime = laptime + next->getLaptime();
    return *this;
}

int Laptime::getLaptime() const{
    return laptime;
}

Laptime* Laptime::getNext() const{
    return next;
}

void Laptime::setNext(const Laptime &next){
    //std::cout << "aaaaaa" << endl;
    this->next = new Laptime(next);
}

void Laptime::setNextLap(Laptime *next) {
    this->next = next;
}



Laptime& Laptime::operator=(const Laptime &rhs) {
    this->laptime = rhs.getLaptime();
    this->next = rhs.getNext();
}

std::ostream &operator<<(std::ostream &os, const Laptime &laptime) {
    int lap = laptime.getLaptime();
    int min = (lap/1000)/60;
    int sec = (lap-min*60*1000)/1000;
    int milli = lap - min*60*1000 - sec*1000;
    os << min << ":" << sec << "." << milli;

    return os;
}
