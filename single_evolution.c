#include <stdio.h>

#include "single_evolution.h"
#include "lib.h"

// in each evolution, make sure that when there is a change
// in the state of a person, the dll is traversed correctly
// and the edges of the person are set appropriately

// idea, generate two universes and two populations, one 
// current and one next. At the beginning of the iteration
// the p_e_neigh is calculated for each cell in current_universe
// ...
// at the end, the currents are reset to initial values and
// currents are swapped with next to ensure clean arguments

// calculate the p_e_neigh for all elements in the universe

// iteration over all the persons



int evo_seiqsf(
    Person* curr_pop,
    // already initialized, just modifies its elements
    Person* next_pop,
    Cell* curr_univ,
    Cell* next_univ,
    size_t total_pop,
    size_t n_rows,
    size_t n_cols,
    size_t radius,
    double p_e,
    double p_q,
    double fat_risk,
    double p_S,
    int t_S
){
    State pers_state;
    Person* person;
    size_t cell_position;
    for (size_t i = 0; i < total_pop; i++) {
        // depending on the state, select the function to call
        // this step can be done with an enum
        //... but first do it with usual ifs
        person = (curr_pop + i);
        if ((person)->st == SUSCEPTIBLE) {
            // get the number of susceptibles in the cell located at the 
            // original position indicated by the examined person in the 
            // current universe
            int n_inf_cell = (curr_univ + (person)->original_location) -> total_in_state[SUSCEPTIBLE];
            double p_e_cell = get_cell_cumulative_p_e(p_e, n_inf_cell);

            double p_e_neigh = calculate_p_e_neigh(
                curr_univ,
                n_rows,
                n_cols,
                radius,
                (person) -> row,
                (person) -> col,
                p_e
            );
            // execute the transition function
            pers_state = s2e(
                (person),
                p_e,
                p_e_cell,
                p_e_neigh
            );
        } else if ((person)->st == EXPOSED) {
            pers_state = e2i((person));
        } else if ((person)->st == INFECTED) {
            pers_state = i2qrf((person), fat_risk, p_q);
        } else if ((person)->st == QUARENTINE) {
            pers_state = q2rf((person), fat_risk);
        } else if ((person)->st == RECOVERED) {
            pers_state = r2s((person), p_S, t_S);
        } else if ((person)->st == DECEASED) {
            pers_state = DECEASED;
        }
        
        // modify the person in the corresponding next population
        copy_person(person, next_pop+i);
        (next_pop + i)->st = pers_state;
        
        // update the value of the current cell where it lives
        cell_position = n_cols*((person) -> row) + (person)->col;
        (next_univ + cell_position)->total_in_state[pers_state]++;
    }
    
    return 0;
}

int evo_seisf(
    Person* curr_pop,
    Person* next_pop,
    Cell* curr_univ,
    Cell* next_univ,
    size_t total_pop,
    size_t n_rows,
    size_t n_cols,
    size_t radius,
    double p_e,
    double fat_risk
){
    State pers_state;
    Person* person;
    size_t cell_position;
    // iterate over each person
    for (size_t i = 0; i < total_pop; i++) {
        // depending on the state, select the function to call
        // this step can be done with an enum
        //... but first do it with usual ifs
        person = (curr_pop + i);
        if ((person)->st == SUSCEPTIBLE) {
            // get the number of susceptibles in the cell located at the 
            // original position indicated by the examined person in the 
            // current universe
            int n_inf_cell = (curr_univ + (person)->original_location) -> total_in_state[SUSCEPTIBLE];
            double p_e_cell = get_cell_cumulative_p_e(p_e, n_inf_cell);


            double p_e_neigh = calculate_p_e_neigh(
                curr_univ,
                n_rows,
                n_cols,
                radius,
                (person) -> row,
                (person) -> col,
                p_e
            );
            // execute the transition function
            pers_state = s2e(
                (person),
                p_e,
                p_e_cell,
                p_e_neigh
            );
        } else if ((person)->st == EXPOSED) {
            pers_state = e2i((person));
        } else if ((person)->st == INFECTED) {
            pers_state = i2sf((person), fat_risk);
        } else {
            pers_state = DECEASED;
        }
        
        // modify the person in the corresponding next population
        copy_person(person, next_pop+i);
        (next_pop + i)->st = pers_state;

        // update the value of the current cell where it lives
        cell_position = n_cols*((person) -> row) + (person)->col;
        (next_univ + cell_position)->total_in_state[pers_state]++;
    }

    return 0;
}

// iterates once over the system
int iterate_once(
    Model model,
    Person* curr_pop,
    Person* next_pop,
    Cell* curr_univ,
    Cell* next_univ,
    size_t total_pop,
    size_t n_rows,
    size_t n_cols,
    size_t radius,
    double p_e,
    double p_q,
    double fat_risk,
    double p_S,
    int t_S
){
    // temporary variables to swap population and universe later
    Person* temp_pop;
    Cell* temp_univ;

    // execute the appropriate function depending on the model passed
    switch (model)
    {
    case SEIQSF:
        evo_seiqsf(
            next_pop,
            curr_pop,
            curr_univ,
            next_univ,
            total_pop,
            n_rows,
            n_cols,
            radius,
            p_e,
            p_q,
            fat_risk,
            p_S,
            t_S
        );
        break;

    case SEISF:
        evo_seisf(
            curr_pop,
            next_pop,
            curr_univ,
            next_univ,
            total_pop,
            n_rows,
            n_cols,
            radius,
            p_e,
            fat_risk
        );
        break;
    
    default:
        printf("ERROR: Model selected not found \n");
        return 3;
        break;
    }
    
    // swap the populations and universes
    temp_pop = curr_pop;
    curr_pop = next_pop;
    next_pop = temp_pop;

    temp_univ = curr_univ;
    curr_univ = next_univ;
    next_univ = temp_univ;

    // all data in cells in the new universe data is set to zero
    for (int r = 0; r < n_rows; r++) {
        for (int c=0; c < n_cols; c++){
            restart_cell_counter(next_univ + r*n_cols + c);
        }
    }
    return 0;
}