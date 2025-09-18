#include <stdio.h>
#include <windows.h>

#include "common.h"
#include "config_reader.h"

char* read_ini_key(LPCSTR file, LPCSTR key) {
    // define parts to be considered
    LPCSTR section = "Settings";
    char* returnValue;
    returnValue = (char*)malloc(100 * sizeof(char));

    DWORD result = GetPrivateProfileString(
        section,
        TEXT(key),
        "",
        returnValue,
        100,
        TEXT(file));
    if (result > 0) {
        //printf("file read\n");
        printf("%s\n", returnValue);
    } else {
        printf("FILE COULD NOT BE READ\n");
        exit(0);
    }
    return returnValue;
}
/*
Settings read_ini(LPCSTR file) {
    Settings settings;
    // define parts to be considered
    LPCSTR section = "Settings";
    LPSTR key_val;
    char returnValue[1000];

    DWORD result = GetPrivateProfileSection(
        section,
        key_val,
        1000,
        TEXT(file));
    printf("%s\n", key_val[0]);
    if (result > 0) {
        printf("file read\n");
        printf("%s\n", key_val[0]);
    } else {
        printf("FILE COULD NOT BE READ\n");
        exit(0);
    }
}
*/
Settings get_settings() {
    // get the settings from the config file    
    Settings settings;
    // read the configs
    settings.n_rows = atoi(read_ini_key(".\\config.ini", "n_rows"));
    settings.n_cols = atoi(read_ini_key(".\\config.ini", "n_cols"));
    settings.radius = atoi(read_ini_key(".\\config.ini", "radius"));
    // to double
    settings.pop_dens = atof(read_ini_key(".\\config.ini", "pop_dens"));
    settings.n_cycles = atoi(read_ini_key(".\\config.ini", "n_cycles"));
    settings.R_0 = atof(read_ini_key(".\\config.ini", "R_0"));
    settings.time_contagious = atoi(read_ini_key(".\\config.ini", "time_contagious"));
    settings.case_fat_risk = atof(read_ini_key(".\\config.ini", "case_fat_risk"));
    settings.t_I = atoi(read_ini_key(".\\config.ini", "t_I"));
    settings.p_R = atof(read_ini_key(".\\config.ini", "p_R"));
    settings.t_F = atoi(read_ini_key(".\\config.ini", "t_F"));
    settings.t_L = atoi(read_ini_key(".\\config.ini", "t_L"));
    settings.t_R = atoi(read_ini_key(".\\config.ini", "t_R"));
    settings.p_S = atof(read_ini_key(".\\config.ini", "p_S"));
    settings.t_S = atoi(read_ini_key(".\\config.ini", "t_S"));
    settings.E_in = atoi(read_ini_key(".\\config.ini", "E_in"));
    settings.I_in = atoi(read_ini_key(".\\config.ini", "I_in"));
    settings.p_displ = atof(read_ini_key(".\\config.ini", "p_displ"));
    settings.max_people = atoi(read_ini_key(".\\config.ini", "max_people"));
    settings.p_e = atof(read_ini_key(".\\config.ini", "p_e"));
    settings.min_infectious = atoi(read_ini_key(".\\config.ini", "min_infectious"));
    settings.p_q = atof(read_ini_key(".\\config.ini", "p_q"));

    return settings;
}

/*
Settings get_settings() {
    Settings settings;
    const char* filename = "config.ini";
    FILE* file = fopen(filename, "r");
    // if (file == NULL) {
    //     return -1;
    // }
    char* fmt = (char*)malloc(1000);
    strcpy(fmt, "[Settings]\n");
    strcat(fmt, "n_rows=%d\n");
    strcat(fmt, "n_cols=%d\n");
    strcat(fmt, "radius=%d\n");
    strcat(fmt, "pop_dens=%f\n");
    strcat(fmt, "n_cycles=%d\n");
    strcat(fmt, "R_0=%f\n");
    strcat(fmt, "time_contagious=%d\n");
    strcat(fmt, "case_fat_risk=%f\n");
    strcat(fmt, "t_I=%d\n");
    strcat(fmt, "p_R=%f\n");
    strcat(fmt, "t_F=%d\n");
    strcat(fmt, "t_L=%d\n");
    strcat(fmt, "t_R=%d\n");
    strcat(fmt, "p_S=%f\n");
    strcat(fmt, "E_in=%d\n");
    strcat(fmt, "I_in=%d\n");
    strcat(fmt, "p_displ=%f\n");
    strcat(fmt, "max_people=%d\n");
    strcat(fmt, "p_e=%f\n");
    strcat(fmt, "min_infectious=%d\n");
    strcat(fmt, "p_q=%f");

    if (
        fscanf(
            file,
            fmt,
            &settings.n_rows,
            &settings.n_cols,
            &settings.radius,
            &settings.pop_dens,
            &settings.n_cycles,
            &settings.R_0,
            &settings.time_contagious,
            &settings.case_fat_risk,
            &settings.t_I,
            &settings.p_R,
            &settings.t_F,
            &settings.t_L,
            &settings.t_R,
            &settings.p_S,
            &settings.t_S,
            &settings.E_in,
            &settings.I_in,
            &settings.p_displ,
            &settings.max_people,
            &settings.p_e,
            &settings.min_infectious,
            &settings.p_q)== 22) {
            printf("Successfully read.\np_q:%f\n",settings.p_q);
    }
    printf("Successfully read.\npop_dens:%f\n",settings.pop_dens);
    fclose(file);
    return settings;
}

int main(){
    get_settings();
    return 0;
}
*/