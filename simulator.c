#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lib.h"
#include "single_evolution.h"
#include "config_reader.h"

typedef unsigned char ubyte;
typedef unsigned int uint;
// Y-coordinates y0, y1 and y2 are already pre-multiplied with world width.
static inline ubyte countAliveCells(ubyte* m_data,size_t x0, size_t x1, size_t x2,
    size_t y0, size_t y1, size_t y2) {
return m_data[x0 + y0] + m_data[x1 + y0] + m_data[x2 + y0]
    + m_data[x0 + y1] + m_data[x2 + y1]
    + m_data[x0 + y2] + m_data[x1 + y2] + m_data[x2 + y2];
}

int computeIterationSerial(
    ubyte* m_data,
    ubyte* m_resultData,
    size_t m_worldWidth,
    size_t m_worldHeight,
    size_t m_dataLength
) {
    ubyte temp;
    // use linear distribution of 2D array
	for (size_t y = 0; y < m_worldHeight; ++y) {
        // y positions of the neighbourhood using 1D array
		size_t y0 = ((y + m_worldHeight - 1) % m_worldHeight) * m_worldWidth;
		size_t y1 = y * m_worldWidth;
		size_t y2 = ((y + 1) % m_worldHeight) * m_worldWidth;

		for (size_t x = 0; x < m_worldWidth; ++x) {
			size_t x0 = (x + m_worldWidth - 1) % m_worldWidth;
			size_t x2 = (x + 1) % m_worldWidth;

			ubyte aliveCells = countAliveCells(m_data, x0, x, x2, y0, y1, y2);
			m_resultData[y1 + x] =
				aliveCells == 3 || (aliveCells == 2 && m_data[x + y1]) ? 1 : 0;
		}
	}

    // swap the variables
    temp = *m_data;
    *m_data = *m_resultData;
    *m_resultData = temp;

    return 0;
}

int main(int argc, char *argv[]){

    if (argc != 2) {
        printf("Number of arguments does not match. ");
        printf("Please use the format `program.exe <seisf|seiqsf>`\n");
        return 3;
    }
    // at most three entries, model and cloud and help
    char* mod = argv[1];
    Model model;

    // universes
    Cell *curr_univ, *next_univ;
    
    // populations
    Person *curr_pop, *next_pop;

    // select the model depending on the value
    if (strcmp(mod, "seisf") == 0) {
        model = SEISF;
    } else if (strcmp(mod, "seiqsf") == 0) {
        model = SEIQSF;
    } else {
        printf("Model provided not found!");
        return 3;
    }

    // read the information from the ini file
    Settings settings = get_settings();

    // results of the deceased
    uint32_t results_dec[settings.n_cycles + 1];
    results_dec[0] = 0;

    // total universe population
    size_t tot_pop = settings.n_cols *settings.n_cols*settings.pop_dens;

    // generate universe
    curr_univ = generate_universe(settings.n_rows, settings.n_cols);
    next_univ = generate_universe(settings.n_rows, settings.n_cols);

    // generate population
    curr_pop = initialize_population(
        tot_pop,
        settings.n_rows,
        settings.n_cols,
        settings.E_in,
        settings.I_in,
        false);
        
    next_pop = initialize_population(
        tot_pop,
        settings.n_rows,
        settings.n_cols,
        settings.E_in,
        settings.I_in,
        true);

    // iterations
    for (uint i = 1; i <= settings.n_cycles; i++){
        results_dec[i] = iterate_once(
            model,
            curr_pop,
            next_pop,
            curr_univ,
            next_univ,
            tot_pop,
            settings.n_rows,
            settings.n_cols,
            settings.radius,
            settings.p_e,
            settings.p_q,
            settings.case_fat_risk,
            settings.p_S,
            settings.t_S
        );
        if (i % 100 == 0) {
            printf("%d iteration\n", i);
        }
    }

    // write file
    FILE *fptr;
    fptr = fopen("results.csv", "w");
    for (uint i = 0; i <= settings.n_cycles; i++){
        fprintf(fptr,"%d\n", results_dec[i]);
    }
    fclose(fptr);


    // deallocate memory for safety
    free(curr_univ);
    free(next_univ);
    free(curr_pop);
    free(next_pop);

    // 
    return 0;
}