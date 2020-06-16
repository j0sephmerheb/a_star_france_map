
#ifndef __City_H
#define __City_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FRANCE_MAP "FRANCE.MAP"



typedef struct Neighbor{
    char *n_name;
    int distance;
    struct Neighbor *next;
} Neighbor;

typedef struct City{
    char *c_name;
    int longitude, latitude, g_nb, h_nb, f_nb;
    struct Neighbor *neighbors;
    struct City *prev;
} City;


/**
Read from the text file and store into structures
*/
void read_file(char *s, char *e);


/**
Set g = 0 for the start city, and 10000 for others. Calculate h and f.
*/
void first_round(void);


/**
Set starting City
*/
void set_starting_city(char *c);


/**
Set g,h,f the first time
*/
void set_ghf(City *c);


/**
Get the city that has the smallest f
*/
City* get_city_smallest_f();

/**
A star algorithm
*/
void a_star(void);

#endif
