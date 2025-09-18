#ifndef config_reader_h
#define config_reader_h

#include <windows.h>

#include "common.h"

// structure to include the information in the config.ini file
typedef struct Settings {
    size_t n_rows;
    size_t n_cols;
    size_t radius;
    double pop_dens;
    uint32_t n_cycles;
    double R_0;
    uint32_t time_contagious;
    double case_fat_risk;
    uint32_t t_I;
    double p_R;
    uint32_t t_F;
    uint32_t t_L;
    uint32_t t_R;
    double p_S;
    uint32_t t_S;
    uint32_t E_in;
    uint32_t I_in;
    double p_displ;
    uint32_t max_people;
    double p_e;
    uint32_t min_infectious;
    double p_q;
} Settings;

Settings get_settings();

#endif