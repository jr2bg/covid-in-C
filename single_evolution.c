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
    for (size_t i = 0; i < total_pop; i++) {
        // depending on the state, select the function to call
        // this step can be done with an enum
        //... but first do it with usual ifs
        if ((curr_pop + i)->st == SUSCEPTIBLE) {
            // get the number of susceptibles in the cell located at the 
            // original position indicated by the examined person in the 
            // current universe
            int n_inf_cell = (curr_univ + (curr_pop + i)->original_location) -> total_in_state[SUSCEPTIBLE];
            double p_e_cell = get_cell_cumulative_p_e(p_e, n_inf_cell);


            double p_e_neigh = calculate_p_e_neigh(
                curr_univ,
                n_rows,
                n_cols,
                radius,
                (curr_pop + i) -> row,
                (curr_pop + i) -> col,
                p_e
            );
            // execute the transition function
            s2e(
                (curr_pop + i),
                p_e,
                p_e_cell,
                p_e_neigh
            );
        } else if ((curr_pop + i)->st == EXPOSED) {
            e2i((curr_pop + i));
        } else if ((curr_pop + i)->st == INFECTED) {
            i2qrf((curr_pop + i), fat_risk, p_q);
        } else if ((curr_pop + i)->st == QUARENTINE) {
            q2rf((curr_pop + i), fat_risk);
        } else if ((curr_pop + i)->st == RECOVERED) {
            r2s((curr_pop + i), p_S, t_S);
        } else if ((curr_pop + i)->st == DECEASED) {

        }
        
    }

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
    // iterate over each person
    for (size_t i = 0; i < total_pop; i++) {
        // depending on the state, select the function to call
        // this step can be done with an enum
        //... but first do it with usual ifs
        if ((curr_pop + i)->st == SUSCEPTIBLE) {
            // get the number of susceptibles in the cell located at the 
            // original position indicated by the examined person in the 
            // current universe
            int n_inf_cell = (curr_univ + (curr_pop + i)->original_location) -> total_in_state[SUSCEPTIBLE];
            double p_e_cell = get_cell_cumulative_p_e(p_e, n_inf_cell);


            double p_e_neigh = calculate_p_e_neigh(
                curr_univ,
                n_rows,
                n_cols,
                radius,
                (curr_pop + i) -> row,
                (curr_pop + i) -> col,
                p_e
            );
            // execute the transition function
            s2e(
                (curr_pop + i),
                p_e,
                p_e_cell,
                p_e_neigh
            );
        } else if ((curr_pop + i)->st == EXPOSED) {
            e2i((curr_pop + i));
        } else if ((curr_pop + i)->st == INFECTED) {
            i2sf((curr_pop + i), fat_risk);
        } else if ((curr_pop + i)->st == DECEASED) {}
        
    }
    
}