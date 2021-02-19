#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H

enum ElevatorState{
    IDLE,
    UP,
    DOWN
};

enum Priority{
    NONE = 0,
    HIGH = 1,
    LOW = 2
};

enum FloorType{
    INTITIAL,
    DESTINATION
};

struct Elevator{
    int num_floors, num_people, weight_capacity, person_capacity, travel_time, idle_time, in_out_time,
    current_floor = 0, current_elevator_state = IDLE, current_no_people = 0, current_weight = 0;
    bool set = false;
};

struct Person{
    int personID, weight_person, initial_floor, destination_floor, priority, move_direction, no_request = 0;
    bool same_floor_control_flag = false;
    bool done = false;
};

#endif