#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/param.h>

#include "lib.h"

// lookup table
double ps_i[] = {
    9.999999999999995e-05,
    0.005069057888351579,
    0.04697281138408274,
    0.1524326835113221,
    0.32668358459673175,
    0.5030663485714983,
    0.6311817405684047,
    0.7456153473400083,
    0.8284751117617429,
    0.8781121051932557,
    0.9180773502240464,
    0.9453219480606242,
    0.9611146387137549,
    0.972254390405818,
    0.9811215095713122,
    0.9871710514697591,
    0.9907105095201212,
    0.9935645408907046,
    0.9955415666031736,
    0.9967180004675154,
    0.9976829858286643,
    0.9983634255871315,
    0.9987757742587142,
    0.9990799978812611,
    0.9993352602947051,
    0.9995193858705637,
    0.9996335281494982,
    0.9997308776387785,
    0.9998022415547547,
    0.9998471946417632,
    0.9998814873225667,
    1.0,
};

// lookup table
double p_r[] = {
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.046512,
    0.046512,
    0.046512,
    0.046512,
    0.046512,
    0.293023,
    0.293023,
    0.293023,
    0.395349,
    0.395349,
    0.465116,
    0.465116,
    0.465116,
    0.477419,
    0.477419,
    0.534884,
    0.534884,
    0.557634,
};

double rand_0_1() {
    // added in a variable to not always get the same value (41)
    int r= rand();
    return (double)r / (double)(RAND_MAX-1);
}

// iid events inside cell of person of interest
double get_cell_cumulative_p_e(double p_e, int n_infected_people) {
    return 1.0 - pow(1.0 - p_e, n_infected_people);
}

// iid events for _external_ neighbourhood
double get_geometric_distribution(double p, int k) {
    return pow(1.0 - p, k - 1) * p;
}

double get_cumulative_geometric_distribution(double p, int n) {
    double p_tot = 0.0;
    for (int k = 0; k == n; k++) {
        p_tot += get_geometric_distribution(p, k);
    }
    return p_tot;
}

// for one person in cell C, determine if this person is still
// suceptible or passes to exposed
// returns the final state for this person, it can be a number or a enum

// Multiply p_E times the number of elements in E or I in its ngh
// chech with random number
State s2e(Person* person, double p_e, double p_e_cell, double p_e_neigh) {
    State final_state = SUSCEPTIBLE;
    // let n_inf_ngbh = univ.get_n_inf_ngbh(&pers.curr_pos, config);
    // // CONSIDER IF WE ONLY TAKE I OR BOTH I AND E
    // let n_inf_cell = univ.get_cell(&pers.curr_pos).n_E + univ.get_cell(&pers.curr_pos).n_I;
    double rand_numb = rand_0_1();
    // //let p_e: f32 = 0.05;
    // let p_e: f32 = config.p_e;
    // //let p_e_cell: f32 = 0.0;
    // let p_e_cell: f32 = get_cum_p_e_cell(p_e, n_inf_cell);
    // //let p_e_neigh: f32 = 0.0;
    // let p_e_neigh: f32 = get_cum_geo_distr(p_e, n_inf_ngbh);

    // union of independent events
    double tot_p_e = p_e_neigh + p_e_cell - p_e_cell * p_e_neigh;

    // here, the contagion does not depend on R0
    //let tot_p_e: f32 = n_inf_ngbh as f32 / 8.0;

    /*
    // depending the number of munimun infectious people there will be a change
    // in state or not
    if n_inf_ngbh < config.min_infectious {
        pers.set_p_state(State::S);
        return;
    }
    */

    // falta considerar el número total de personas
    // usar dens_pob*n_rows*n_cols
    //printf("tot pe: %.4f, pe: %.4f\n", tot_p_e, rand_numb);
    if (rand_numb <= tot_p_e) {
        person->st = EXPOSED;
        person->time_in_state = 0;
        final_state = EXPOSED;
    }
    return final_state;
}

State e2i(Person* person) {
    // no consideramos un tiempo
    double rand_numb = rand_0_1();
    printf("selected random number %.4f\n", rand_numb);

    if (rand_numb <= ps_i[person->time_in_state]){
        // (*V).A is equal to V->A
        (*person).st = INFECTED;
        person->time_in_state = 0;
        return INFECTED;
        //pers.set_t_state(0);
    } 
    // else {
    //     pers.add_time_state(1);
    // }
    person->time_in_state++;
    return EXPOSED;
}

State i2rf(Person* person, double fat_risk) {
    double rand_numb= rand_0_1();

    // time in days for the probability of r
    int t = fmin(28,person->time_in_state);

    printf("time in state: %d\tt: %d\n",person->time_in_state, t);

    // let fat_risk = get_p_f(pers.t_state);

    //} else if rand_numb <= config.case_fat_risk && pers.t_state >= config.t_F {
    if (rand_numb <= fat_risk) {
        person->st = DECEASED;
        person->time_in_state = 0;
        return DECEASED;
        //pers.set_t_state(0);
    //if rand_numb <= config.p_R && pers.t_state >= config.t_R {
    } else if (rand_numb <= p_r[t] + fat_risk) {
        person->st = RECOVERED;
        person->time_in_state = 0;
        // cambiar a cero o 1 el t_state
        return RECOVERED;
        //pers.set_t_state(0);
    }
    person->time_in_state++;
    return INFECTED;
    // else {
    //     pers.add_time_state(1);
    // }
    // pers.set_p_state(State::I);
}

State i2qrf(Person* person, double fat_risk, double p_q) {
    double rand_numb = rand_0_1();
    
    // time in days for the probability of r
    int t = fmin(28,person->time_in_state);
    // let fat_risk = get_p_f(pers.t_state);

    //} else if rand_numb <= config.case_fat_risk && pers.t_state >= config.t_F {
    if (rand_numb <= fat_risk) {
        person->st = DECEASED;
        person->time_in_state = 0;
        return DECEASED;
        //pers.set_t_state(0);
    //if rand_numb <= config.p_R && pers.t_state >= config.t_R {
    } else if (rand_numb <= p_r[t] + fat_risk) {
        person->st = RECOVERED;
        person->time_in_state = 0;
        // cambiar a cero o 1 el t_state
        return RECOVERED;
        //pers.set_t_state(0);
    } else if (rand_numb <= p_r[t] + fat_risk + p_q){
        person->st = QUARENTINE;
        person->time_in_state = 0;
        return QUARENTINE;
        //pers.set_t_state(0);
    }
    person->time_in_state++;
    return INFECTED;
    // else {
    //     pers.add_time_state(1);
    // }
    // pers.set_p_state(State::I);
}

State q2rf(Person* person, double fat_risk) {
    double rand_numb = rand_0_1();

    // time in days for the probability of r
    int t = fmin(28,person->time_in_state);

    // let fat_risk = get_p_f(pers.t_state);

    //} else if rand_numb <= config.case_fat_risk && pers.t_state >= config.t_F {
    if (rand_numb <= fat_risk) {
        person->st = DECEASED;
        person->time_in_state=0;
        return DECEASED;
        //pers.set_t_state(0);
    //if rand_numb <= config.p_R && pers.t_state >= config.t_R {
    } else if (rand_numb <= p_r[t] + fat_risk) {
        person->st = RECOVERED;
        person->time_in_state = 0;
        // cambiar a cero o 1 el t_state
        return RECOVERED;
        //pers.set_t_state(0);
    }
    person->time_in_state++;
    return QUARENTINE;
    // else {
    //     pers.add_time_state(1);
    // }
    // pers.set_p_state(State::I);
}

// seisf transition
State i2sf(Person* person, double fat_risk) {
    double rand_numb = rand_0_1();

    // time in days for the probability of r
    int t = fmin(28,person->time_in_state);

    if (rand_numb <= fat_risk) {
        person->st = DECEASED;
        person->time_in_state = 0;
        return DECEASED;
        //pers.set_t_state(0);
    } else if (rand_numb <= p_r[t] + fat_risk) {
    //} else if rand_numb <= 0.25 + config.case_fat_risk {
        person->st = SUSCEPTIBLE;
        person->time_in_state = 0;
        // cambiar a cero o 1 el t_state
        return SUSCEPTIBLE;
        //pers.set_t_state(0);
    }
    person->time_in_state++;
    return INFECTED;
    //  else {
    //     pers.add_time_state(1);
    // }
    // pers.set_p_state(State::I);
}

State r2s(Person* person, double p_S, int t_S) {
    double rand_numb = rand_0_1();
    if (rand_numb <= p_S && person->time_in_state >= t_S) {
        return SUSCEPTIBLE;
    }
    return RECOVERED;
}

// // seisf transition
// pub fn r2s(pers: &mut Pers, config: &Config) {
//     let rand_numb: f32 = thread_rng().gen();
//     if rand_numb <= config.p_S && pers.t_state >= config.t_S {
//         pers.set_state(State::S);
//     }
//     pers.set_p_state(State::R);
// }

//
// cells
//

// initialization of a cell with default values
Cell init_cell() {
    Cell cell = {
        .total_in_state = {0,0,0,0,0,0},
        .p_e_neigh = 0.,
    };
    
    return cell;
}

// initialize the universe with given dimensions
// the final array is 1D
Cell* generate_universe(size_t rows, size_t cols) {

    // get the number of elements in the universe
    size_t total_cells = rows * cols;

    // memory allocation for the whole universe
    Cell* universe = (Cell*)malloc(sizeof(Cell) * total_cells);

    // return an error if there was no memory allocated
    if (universe == NULL) {
        printf("ERROR: Allocation for the universe failed");
        exit(0);
    }

    // iterate to initialize the cells
    for (size_t i = 0; i < total_cells; i++) {
        universe[i] = init_cell();
    }

    return universe;
}

double calculate_p_e_neigh(
    Cell* universe,
    size_t n_rows,
    size_t n_cols,
    size_t radius,
    size_t row,
    size_t col,
    double p_e
) {
    // calculates the number of infected for a given neighbourhood for a 
    // specific position
    size_t size_univ = n_rows * n_cols;
    int n_infected = 0;
    int lineal_pos;

    // get the neighbourhood for the cell position
    for (int i = -radius; i<= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            // as the interest is only in the neighbourhood,
            // the cell being used as origin is discarded
            if (i == 0 && j == 0) {
                continue;
            }

            // get the position
            lineal_pos = (row + i) * n_cols + (col + j);

            // consider only positions that are present in the universe
            if (0 <= lineal_pos && lineal_pos < size_univ) {
                n_infected += (universe + lineal_pos)-> total_in_state[INFECTED];
            }
        }
    }

    // get the cummulative geometric distribution
    double p_e_neigh = get_cumulative_geometric_distribution(p_e, n_infected);

    // associate the element in the neighbourhood with a value
    universe -> p_e_neigh = p_e_neigh;

    return p_e_neigh;
}

// add a total of m people in a State randomly into the universe

// function to add a determined number of people in a state
// according to the number, eg 500 infected and 200 exposed

// change a Person from one universe to another, in the specified state
// for the given cell (making the move before the end of the evolution will
// introduce errors)
// change to iterate over people instead of cells
// the iteration over cells will be done to get the p_e_neigh
