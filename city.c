#include <stdio.h>
#include <string.h>

#include "list.h"
#include "city.h"


/* Open List */
List* list_open;

/* Closed List */
List* list_closed;

/* List of cities */
List* list_cities;

/* Starting City */
City *starting_city = NULL;

/* Ending City */
City *ending_city = NULL;



/**
Compare 2 lists of cities
*/
int compList(void *c1, void *c2){
    City* city1 = (City*) c1;
    City* city2 = (City*) c2;

    return strcmp(city1->c_name, city2->c_name);
}


/**
Return city Name from a list
*/
static void prList(void * s) {
    City *city = s;
    Neighbor *neighbors = city->neighbors;
    printf("%s", city->c_name);
    while(neighbors != NULL){
        neighbors = neighbors->next;
    }
}

/**
Compare int
*/
int compInt (void * i1, void * i2) {
    int val1 = (int)i1;
    int val2 = (int)i2;

    if (val1<val2){
        return -1;
    }
    else if (val1 == val2){
        return 0;
    }
    else if (val1>val2 ){
        return 1;
    }
}

/**
Preview int
*/
static void prInt(void * i) {
  printf("%d", i);
}


/**
 Read FRANCE.MAP
 */
void read_file(char* st_point, char* end_point) {
    FILE *france_map;
    france_map = fopen(FRANCE_MAP, "r");

    char line[60];
    char name[30];
    int c_long, c_lat, n_distance;

    list_cities = newList(compList,prList);

    City *city;

    while (fgets(line, sizeof line, france_map) != NULL){
        if (sscanf(line, "%s %d %d", name, &c_long, &c_lat) == 3){
            city = malloc(sizeof(City));
            city->c_name = malloc(100);
            strcpy(city->c_name, name);
            city->longitude = c_long;
            city->latitude = c_lat;
            city->h_nb = 0;
            city->g_nb = 10000;
            city->f_nb = 0;
            city->neighbors = NULL;
            city->prev = NULL;

            addList(list_cities, city);
        }
        else if (sscanf(line, "%s %d", name, &n_distance) == 2){
            Neighbor *n, *tmp;
            tmp = malloc(sizeof(Neighbor));
            tmp->n_name = malloc(100);
            strcpy(tmp->n_name, name);
            tmp->distance = n_distance;
            tmp->next = NULL;

            if(city->neighbors == NULL){
                city->neighbors = tmp;
            }
            else{
                n=city->neighbors;
                while(n->next != NULL){
                    n = n->next;
                }
                n->next = tmp;
            }
        }
    };


    /* Close file */
    fclose(france_map);

    /* Set starting and ending cities */
    set_starting_city(st_point);
    set_ending_city(end_point);

    /* First Iteration */
    first_round();
}


/**
Set starting city
*/
void set_starting_city(char* c){
    for(int i = 1; i< lengthList(list_cities) + 1; i++){
        City* s = NULL;
        nthInList(list_cities, i, &s);

        if(strcmp(c, s->c_name) == 0 ){
            starting_city = s;
        }
    }

    if (starting_city == NULL){
        printf("Starting city is incorrect. \n");
        exit(0);
    }
}

/**
Set ending city
*/
void set_ending_city(char* c){
    for(int i = 1; i< lengthList(list_cities) + 1; i++){
        City* s = NULL;
        nthInList(list_cities, i, &s);

        if(strcmp(c, s->c_name) == 0 ){
            ending_city = s;
        }
    }

    if (ending_city == NULL){
        printf("Ending city is incorrect. \n");
        exit(0);
    }
}


/**
Set g,h,f for the first time
*/
void first_round(){
    list_open = newList(compList,prList);
    list_closed = newList(compList,prList);


    /* add the first city to the open list */
    addList(list_open, starting_city);


    /* Set G for the starting city */
    starting_city->g_nb = 0;


    /* Set G H F for all cities */
    forEach(list_cities, set_ghf);


    /* Start the A star algorithm */
    a_star();
};


/**
Set g,h,f for each city
*/
void set_ghf(City *c){
    int la1, lo1, la2, lo2;

    la1 = ending_city->latitude;
    lo1 = ending_city->longitude;

    la2 = c->latitude;
    lo2 = c->longitude;

    c->h_nb = (abs(la1-la2)+abs(lo1-lo2))/4;
    c->f_nb = c->h_nb + c->g_nb;
}


/** Get the city with the smallest f from the cities list */
City* get_city_smallest_f(){
    if(lengthList(list_open) > 1){
        List* tmp_list_f2 = newList(compInt, prInt);

        // Putting all the f of the cities in 1 sorted list
        for(int i = 1; i<lengthList(list_open)+1; i++){
            City* c = NULL;
            nthInList(list_open, i, &c);
            int f = c->f_nb;
            addList(tmp_list_f2, f);
        }

        int smallest_f = NULL;
        nthInList(tmp_list_f2, 1, &smallest_f);

        for(int i = 1; i<lengthList(list_cities)+1; i++){
            City* new_c = NULL;
            nthInList(list_cities, i, &new_c);
            if(new_c->f_nb == smallest_f){
                return new_c;
                delList(tmp_list_f2);
            }
        }
        return NULL;
    }
    /* We only have one city */
    else{
        City* c = NULL;
        nthInList(list_open, 1, &c);
        return c;
    }
}



/**
A-Star algorithm
*/
void a_star(){
    /* if OPEN is NOT empty */
    if(lengthList(list_open) != 0){
        // Get the city with smallest f from the open list
        City* c = get_city_smallest_f();

        while(c != ending_city){
            // Remove city from the OPEN list
            remFromList(list_open, c);

            // Add city to the CLOSED list
            addList(list_closed,c);

            Neighbor *neighbors = c->neighbors;

            // Iterating over beighbors
            while(neighbors != NULL){
                int n_distance = neighbors->distance;
                char* n_name = neighbors->n_name;

                // Comparing cities to neighbors
                for(int i = 1; i<lengthList(list_cities)+1; i++){
                    City* relevant_city = NULL;
                    nthInList(list_cities, i, &relevant_city);

                    // Getting relevant city from the neighbor name
                    if(strcmp(n_name, relevant_city->c_name) == 0 ){
                        int new_g = n_distance + c->g_nb;
                        int new_f = new_g + relevant_city->h_nb;

                        if((!isInList(list_open, relevant_city) || !isInList(list_closed, relevant_city)) &&  new_g < relevant_city->g_nb){
                            relevant_city->g_nb = new_g;
                            relevant_city->f_nb = new_f;
                            addList(list_open, relevant_city);

                            relevant_city->prev = c;
                        }
                    }
                }

                // Move to the next neighbor
                neighbors = neighbors->next;
            }

            // Next City
            City* old_city = c;
            c = get_city_smallest_f();
                if(c != ending_city){
                c->prev = old_city;
            }
        }

        // Reached the goal
        printf("Reached the Goal: %s \n", ending_city->c_name);
        printf("Starting City: %s \n", starting_city->c_name);

        // Our Cost
        printf("Cost: %d \n\n", c->g_nb);

        //Our Backpath
        printf("Packpath: \n");
        printf("%s <- ", ending_city->c_name);
        while(c != starting_city && c->prev != c){
            printf("%s <- ", c->prev->c_name);
            City* tmp = c->prev;
            c = tmp;
        }

        printf("\n");

   }
}
