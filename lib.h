#ifndef lib_h
#define lib_h

#include "common.h"
// enumeration for the models
typedef enum {
    SEIQSF,
    SEISF
} Model;

// enumeration for the states
typedef enum {
    SUSCEPTIBLE,
    EXPOSED,
    INFECTED,
    RECOVERED,
    DECEASED,
    QUARENTINE
} State;

// structure for the person
// contains the time in the current state and the location
typedef struct Person {
    size_t row;
    size_t col;
    size_t original_location;
    int time_in_state;
    State st;
    // for double ll
    struct Person* prev_person;
    struct Person* next_person;
} Person;

//uint32_t in [0, 2^32 − 1 which equals 4,294,967,295]
// to use cells with length the enum State
typedef uint32_t counter[6];

// for the cell
typedef struct Cell {
    
    // number of persons in each state
    counter total_in_state;

    // number of p_e_neigh
    double p_e_neigh;
    
} Cell;


double rand_0_1();

// function naming?
State s2e(Person* person, double p_e, double p_e_cell, double p_e_neigh);
State e2i(Person* person);
State i2rf(Person* person, double fat_risk);
State i2qrf(Person* person, double fat_risk, double p_q);
State q2rf(Person* person, double fat_risk);
State i2sf(Person* person, double fat_risk);
State r2s(Person* person, double p_S, int t_S);

Cell* generate_universe(size_t rows, size_t cols);

double calculate_p_e_neigh(
    Cell* universe,
    size_t n_rows,
    size_t n_cols,
    size_t radius,
    size_t row,
    size_t col,
    double p_e
);

double get_cell_cumulative_p_e(double p_e, int n_infected_people);

int copy_person(Person* source, Person* destination);

int restart_cell_counter(Cell* cell);

#endif