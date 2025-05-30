#include <stdio.h>
#include <stdlib.h>

#include "lib.h"
#include "config_reader.h"
#include "single_evolution.h"


int main() {

    // for susceptible
    // for (int i = 0; i < 10; i++) {
    //     //printf("%f\n", rand_0_1());
    //     printf("Final State: %d\n", (int)s2e(0.1,0.3,0.4));
    // }

    // // for exposed
    // Person person = {.original_location= 5, .time_in_state = 0, .st=EXPOSED};
    // for (int i = 0; i < 15; i++) {
    //     printf("STATE: %d\n", e2i(&person));
    //     printf("time in state: %d\n\n", person.time_in_state);
    // }
    
    
    // for infected
    // Person person = {.original_location= 5, .time_in_state = 0, .st=EXPOSED};
    // for (int i = 0; i < 50; i++) {
    //     printf("STATE: %d\n", i2rf(person,0.5));
    //     person.time_in_state++;
    // }

    // for generate universe
    // Cell* universe = generate_universe(1000,1000);
    // printf("init number of susc: %d\n", universe[0].total_in_state[SUSCEPTIBLE]);
    // universe[0].total_in_state[SUSCEPTIBLE] = 50;
    // printf("init number of susc: %d\n", universe[0].total_in_state[SUSCEPTIBLE]);

    // to check the counter
    // counter cell = {0,5,3,6,7,2};
    // printf("total_infected in cell: %d\n", cell[INFECTED]);

    // read the settings
    Settings setts = get_settings();
    //printf("%d %d\n", setts.n_rows, setts.n_cols);

    // generate a universe based on the dimensions provided
    Cell* universe = generate_universe(setts.n_rows, setts.n_cols);

    //
    
    return 0;
}