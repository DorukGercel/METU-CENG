#include <iostream>
#include <string>
#include <fstream>
#include <pthread.h>
#include <vector> 

#include "monitor.h"
#include "definitions.h"

using namespace std;

class ElevatorMonitor : public Monitor{
    Condition onFloorEnter;              //for a person to check if he can enter the elevator by the floor number
    Condition onFloorLeave;              //for a person to check if he can leave the elevator by the floor number
    Condition eligibleRequest;      //for a person to check if he can make a request
    Condition hasPriority;         //for a person to check if he has priority to enter
    Condition peopleExited;        //for a person to wait for people to leave elevator before he can enter

    Condition hasRequest;           //for the elevator to check if there is any request
    Condition changeFloor;          //for the elevator to check if the floor has been changed
    Condition endOf_Leave_Enter;    //for the elevator to wait for everyone to leave or enter

    Elevator* owner_elevator;           //the elevator of the system

    int number_of_people = 0;               //the number of people in the system
    int number_of_request = 0;              //the number of total requests in the system
    
    vector<int> destinations;                //the destinations in the path of the elevator
    vector<int> floors_high_priorities;     //the number of people in the floors with high priorities
    vector<int> floors_low_priorities;      //the number of people in the floors with low priorities
    vector<int> floors_destinations;        //the number of people in the floor who wants to leave


    //Bubble sort according to the destination
    void bubbleSort(int direction){
        int size = destinations.size();
        for(int i = 0; i < size - 1; i++){
            for(int j = 0; j < size - i - 1; j++){
                if(direction == UP){
                    if(destinations[j] > destinations[j+1]){
                        int tmp = destinations[j];
                        destinations[j] = destinations[j+1];
                        destinations[j+1] = tmp;
                    }
                }
                else{
                    if(destinations[j] < destinations[j+1]){
                        int tmp = destinations[j];
                        destinations[j] = destinations[j+1];
                        destinations[j+1] = tmp;
                    }
                }
            }
        }
    }

    //Check if the dest is in the destination list
    bool inTheDestinationList(int dest){
        int no_destination = destinations.size();
        for(int i = 0; i < no_destination; i++){
            if(destinations[i] == dest){
                return true;
            }
        }
        return false;
    }

    //If elevator is IDLE, the first request decides the move direction
    void setTheRequestDestination(Person* person, FloorType type){
        int floor;
        if(type == INTITIAL){
            floor = person->initial_floor;
            if(owner_elevator->current_elevator_state == IDLE){
                if(floor < owner_elevator->current_floor){
                    owner_elevator->current_elevator_state = DOWN;
                }
                else if(floor > owner_elevator->current_floor){
                    owner_elevator->current_elevator_state = UP;
                }
                else{
                    owner_elevator->current_elevator_state = IDLE;
                }
            }
        }
        else{
            floor = person->destination_floor;
            if(floor < owner_elevator->current_floor){
                    owner_elevator->current_elevator_state = DOWN;
            }
            else{
                owner_elevator->current_elevator_state = UP;
            }
        }
    }

    //If destination is not added to the list add it to the list
    //Add person to the priority floor, when he makes a request
    //Execute when a person makes a request or enters the elevator
    void addToDestinationList(Person* person, FloorType type){
        int direction = owner_elevator->current_elevator_state;
        if(type == INTITIAL){
            int floor = person->initial_floor;
            if(!inTheDestinationList(floor)){
                destinations.push_back(floor);
                bubbleSort(direction);
            }
            if(person->priority == HIGH){
                floors_high_priorities[floor] += 1;
            }
            else{
                floors_low_priorities[floor] += 1;
            }
        }
        else{
            int floor = person->destination_floor;
            if(!inTheDestinationList(floor)){
                destinations.push_back(floor);
                bubbleSort(direction);
            }
            floors_destinations[floor]+=1;
        }
    }

    //Remove the current destination from the list
    //When person enters the elevator (or cannot enter), remove it from priority floor list
    //Execute when a person enters or leaves
    void removeFromDestinationList(Person* person, FloorType type){
        int floor;
        if(type == INTITIAL){
            floor = person->initial_floor;
            if(person->priority == HIGH){
                floors_high_priorities[floor] -= 1;
            }
            else{
                floors_low_priorities[floor] -= 1;
            }
        }
        else{
            floor = person->destination_floor;
            floors_destinations[floor] -= 1;
        }
    }

    //Returns boolean if the person is eligible to make request
    bool canMakeRequest(Person* person){
        int sum_in_a_floor;
        if(owner_elevator->current_elevator_state == IDLE && number_of_request == 0){
            return true;
        }
        else if(owner_elevator->current_elevator_state == IDLE && (floors_high_priorities[owner_elevator->current_floor] + floors_low_priorities[owner_elevator->current_floor]) > 1){
            return true;
        }
        else if(owner_elevator->current_elevator_state == person->move_direction){
            if(owner_elevator->current_elevator_state == UP){
                if(person->initial_floor > owner_elevator->current_floor){
                    return true;
                }
            }
            else{
                if(person->initial_floor < owner_elevator->current_floor){
                    return true;
                }
            }
        }
        return false;
    }

    //Return for person if he has priority
    //First look at if person has high priority
    //Then looks if there is someone else with higher priority
    bool hasEnterancePriority(Person* person){
        if(person->priority == HIGH){
            return true;
        }
        else if(floors_high_priorities[person->initial_floor] == 0){
            return true;
        }
        else{
            return false;
        }
    }

    //Returns a bool value if a person can enter the elevator in Enter function
    bool canFitTheElevator(Person* person){
        if(owner_elevator -> current_elevator_state == IDLE || owner_elevator->current_elevator_state == person->move_direction)
            if(owner_elevator->current_no_people < owner_elevator->person_capacity){
                if(owner_elevator->current_weight + person->weight_person <= owner_elevator->weight_capacity){
                    return true;
                }
            }
        return false;
    }

    //Changes the current floor of the elevator according to direction
    void moveHelper(){
        if(owner_elevator->current_elevator_state == UP){
            owner_elevator->current_floor += 1;
        }
        else if(owner_elevator->current_elevator_state == DOWN){
            owner_elevator->current_floor -= 1;
        }
        else{
            ;
        }
    }

    //Print priority in characters
    void printPriority(Person* person){
        if(person->priority == HIGH){
            cout << "hp";
        }
        else{
            cout << "lp";
        }
    }

    //Print elevator state in characters
    void printElevatorState(){
        if(owner_elevator->current_elevator_state == IDLE){
            cout << "Idle";
        }
        else if(owner_elevator->current_elevator_state == UP){
            cout << "Moving-up";
        }
        else{
            cout << "Moving-down";
        }
    }

    //Print elevator destination lis
    void printElevatorDestList(){
        int size = destinations.size();
        if(size != 0){
            cout << " ";
            for(int i = 0; i < size; i++){
                cout << destinations[i];
                if(i != size-1){
                    cout << ",";
                }
            }
        }
    }

    public:
        ElevatorMonitor(): onFloorEnter(this), onFloorLeave(this), eligibleRequest(this), hasPriority(this), hasRequest(this),
                           changeFloor(this), endOf_Leave_Enter(this), peopleExited(this){

        }

        void bindTheElevator(Elevator* elevator){
            int no_floors;
            owner_elevator = elevator;
            number_of_people = owner_elevator->num_people;
            no_floors = owner_elevator->num_floors;
            
            floors_high_priorities.resize(no_floors + 1, 0);
            floors_low_priorities.resize(no_floors + 1, 0);
            floors_destinations.resize(no_floors + 1, 0);
        }
        int getNoPeople(){
            __synchronized__
            return number_of_people;
        }
        /*void elevatorMovement(){
            __synchronized__
            while(number_of_people){
                waitForRequest();
                moveElevator();
            }
        }*/

        void waitForRequest(){
            __synchronized__
            while(!number_of_request && number_of_people && owner_elevator->current_elevator_state == IDLE){                
                eligibleRequest.notifyAll();
                hasRequest.timed_wait(owner_elevator->idle_time);
            }
         }

        void moveElevator(){
            __synchronized__
            while(destinations.size() > 0 || number_of_request > 0 || owner_elevator->current_elevator_state != IDLE){
                int floor = owner_elevator->current_floor;
                while((floors_high_priorities[owner_elevator->current_floor] + floors_low_priorities[owner_elevator->current_floor]) > 0){//In case smn enters at the begining
                        onFloorEnter.notifyAll();
                        endOf_Leave_Enter.timed_wait(owner_elevator->in_out_time);
                }
                while(floor == owner_elevator->current_floor){
                    changeFloor.timed_wait(owner_elevator->travel_time);
                    moveHelper();
                }
                if(inTheDestinationList(owner_elevator->current_floor)){
                    destinations.erase(destinations.begin());
                    if(destinations.size() == 0){//If elevator has no more destinations
                        owner_elevator->current_elevator_state = IDLE;
                    }
                    printElevator();
                    while(floors_destinations[owner_elevator->current_floor]){//Someone needs to leave
                        onFloorLeave.notifyAll();
                        endOf_Leave_Enter.timed_wait(owner_elevator->in_out_time);
                    }
                    while((floors_high_priorities[owner_elevator->current_floor] + floors_low_priorities[owner_elevator->current_floor]) > 0){//Someone enters
                        onFloorEnter.notifyAll();
                        endOf_Leave_Enter.timed_wait(owner_elevator->in_out_time);
                    } 
                }
                else{
                    printElevator();
                }
            }
        }

        /*
        void doPersonActions(Person* person){
            __synchronized__
            while(!(person->done)){
                makeRequest(person);
                if(enterElevator(person)){
                    leaveElevator(person);
                    return;
                }
            }
        }*/

        void makeRequest(Person* person){
            __synchronized__
            while(!canMakeRequest(person)){
                eligibleRequest.wait();
            }
            number_of_request+=1;
            if(owner_elevator->current_floor != person->initial_floor){ //On the roead entry request
                setTheRequestDestination(person, INTITIAL);
                addToDestinationList(person, INTITIAL);
            }
            else if(owner_elevator->current_floor == person->initial_floor && person->no_request == 0){ //Same floor start, request once and enter
                if(person->priority == HIGH){
                    floors_high_priorities[person->initial_floor] += 1;
                }
                else{
                    floors_low_priorities[person->initial_floor] += 1;
                }
                person->same_floor_control_flag = true;
            }
            else{   // Re-awaken from idle, was the only destination arrived
                person->same_floor_control_flag = true;
            }
            person->no_request += 1;
            printPersonRequest(person);
            printElevator();
        }

        bool enterElevator(Person* person){
            __synchronized__
            int floor = person->initial_floor;
            while(floor != owner_elevator->current_floor){
                onFloorEnter.wait();
            }
            //This is for re-awaken the IDLE in the same floor, the only case the person req is NOT reseted. Future set is if when it is idle, more people try to req in same floor
            if(owner_elevator->current_elevator_state == IDLE && !(person->same_floor_control_flag)){ 
                person->same_floor_control_flag = false;
                number_of_request-=1;
                return false;
            }
            if(owner_elevator->current_elevator_state != IDLE && owner_elevator->current_elevator_state != person->move_direction){//Prevent the early but errorful entry
                person->same_floor_control_flag = false;
                person->no_request = 0;
                number_of_request-=1;
                removeFromDestinationList(person, INTITIAL);
                return false;
            }
            while(floors_destinations[floor]){//Wait the leavers
                peopleExited.wait();
            }
            while(canFitTheElevator(person)){
                while(!hasEnterancePriority(person)){
                    hasPriority.wait();
                }
                if(canFitTheElevator(person)){
                    owner_elevator->current_no_people += 1;
                    owner_elevator->current_weight += person->weight_person;
                    number_of_request-=1;
                    person->no_request = 0;
                    removeFromDestinationList(person, INTITIAL);
                    addToDestinationList(person, DESTINATION);
                    setTheRequestDestination(person, DESTINATION);
                    printPersonEnter(person);
                    printElevator();
                    
                    hasPriority.notifyAll();
                    return true;
                }
                else{
                    person->same_floor_control_flag = false;
                    person->no_request = 0;
                    number_of_request-=1;
                    removeFromDestinationList(person, INTITIAL);
                    hasPriority.notifyAll();
                    return false;
                }
            }
            person->same_floor_control_flag = false;
            person->no_request = 0;
            number_of_request-=1;
            removeFromDestinationList(person, INTITIAL);
            return false;
        }

        void leaveElevator(Person* person){
            __synchronized__
            int floor = person->destination_floor;
            while(floor != owner_elevator->current_floor){
                onFloorLeave.wait();
            }
            removeFromDestinationList(person, DESTINATION);
            person->done = true;
            number_of_people -= 1;
            owner_elevator->current_no_people -= 1;
            owner_elevator->current_weight -= person->weight_person;
            peopleExited.notifyAll();

            printPersonLeave(person);
            printElevator();
        }

        void printElevator(){
            cout << "Elevator (";
            printElevatorState();
            cout << ", " << owner_elevator->current_weight << ", " << owner_elevator->current_no_people << ", " << owner_elevator->current_floor << " ->";
            printElevatorDestList();
            cout << ")"<< endl;
        }

        void printPersonRequest(Person* person){
            cout << "Person (" << person->personID << ", ";
            printPriority(person);
            cout << ", " << person->initial_floor << " -> " << person->destination_floor << ", " << person->weight_person << ") made a request" << endl;
        }

        void printPersonEnter(Person* person){
            cout << "Person (" << person->personID << ", ";
            printPriority(person);
            cout << ", " << person->initial_floor << " -> " << person->destination_floor << ", " << person->weight_person << ") entered the elevator" << endl;
        }

        void printPersonLeave(Person* person){
            cout << "Person (" << person->personID << ", ";
            printPriority(person);
            cout << ", " << person->initial_floor << " -> " << person->destination_floor << ", " << person->weight_person << ") has left the elevator" << endl;
        }
    
        
}elevator_monitor;


void* personThread(void* personInfo){
    Person* person = (Person*) personInfo;
    while(!(person->done)){
                elevator_monitor.makeRequest(person);
                if(elevator_monitor.enterElevator(person)){
                    elevator_monitor.leaveElevator(person);
                    return 0;
                }
    }
    return 0;
}

void* elevatorControllerThread(void* elevatorInfo){
    while(elevator_monitor.getNoPeople()){
                elevator_monitor.waitForRequest();
                elevator_monitor.moveElevator();
    }
    return 0;
}

int main(int argc, char const *argv[])
{   
    struct Elevator elevator; //elevator struct
    pthread_t elevator_thread;//thread for elevator

    //File read operations for elevator
    ifstream myfile;
    myfile.open(argv[1]);
    if(myfile.is_open()){
        myfile >> elevator.num_floors >> elevator.num_people >> elevator.weight_capacity >> elevator.person_capacity
        >> elevator.travel_time >> elevator.idle_time >> elevator.in_out_time; 
    }

    //Bind the elevator to the monitor
    elevator_monitor.bindTheElevator(&elevator);

    //Create elevator thread
    pthread_create(&elevator_thread,NULL, elevatorControllerThread, (void*)&elevator);
    
    struct Person people[elevator.num_people];//people initialised
    pthread_t people_thread[elevator.num_people];//thread for people

    //File read operations for people and create its thread
    for(int i = 0; i < elevator.num_people; i++){
        people[i].personID = i;
        myfile >> people[i].weight_person >> people[i].initial_floor >> people[i].destination_floor >> people[i].priority;
        if(people[i].destination_floor > people[i].initial_floor){
            people[i].move_direction = UP;
        }
        else
        {
            people[i].move_direction = DOWN;
        }
        pthread_create(people_thread + i, NULL, personThread, (void *)(people+i));
    }

    pthread_join(elevator_thread, NULL);

    for(int i = 0; i < elevator.num_people; i++){
        pthread_join(people_thread[i], NULL);
    }

    return 0;
}
