#ifndef single_evolution_h
#define single_evolution_h

#include "lib.h"

uint32_t iterate_once(
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
);

#endif
