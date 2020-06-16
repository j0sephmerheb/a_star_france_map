#include<stdio.h>
#include "status.h"
#include "city.h"

/**
 Main
 */
int main(int argc, char* argv[]){
    char* st_point;
    char* end_point;

    if(argc==3){
        st_point = argv[1];
        end_point = argv[2];
        read_file(st_point, end_point);
    }
    else{
        printf("Please write the names of the start city and the end city. \n");
    }

    return 0;
}

