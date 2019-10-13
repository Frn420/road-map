#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cities.h"
#include "parser.h"

void deleteRoad(Road *road) {
    if (road) {
        Road *tmp = road->next;
        free(road);
        deleteRoad(tmp);
    }
}

void deleteNode(City *city) {
    deleteRoad(city->next);
    freeConst(city->city_name);
    free(city);
}

void deleteRoute(Route *route) {
    if (route) {
        Route *tmp = route->next;
        free(route);
        deleteRoute(tmp);
    }
}

bool addCity(Map *map, const char *city_name) {
    bool check = true;
    int length;
    City **memcheck = NULL;
    Map *tmp = map;

    if (map) {
        length = map->number_of_cities + 1;
        memcheck = (City**)malloc(sizeof(City*));

        if (memcheck) {
            free(memcheck);
            map->cities = realloc(map->cities, length * sizeof(City*));
            (map->number_of_cities)++;
            map->cities[length - 1] = (City*)malloc(sizeof(City));

            if (map->cities[length - 1]) {
                map->cities[length - 1]->city_name = city_name;
                map->cities[length - 1]->next = NULL;
            }
            else {
                check = false;
            }
        }
        else {
            *map = *tmp;
            check = false;
        }
    }

    return check;
}

bool createRoad(City *city, const char *city_direction,
               unsigned length, int built_year) {
    bool check = true;
    Road *tmp;

    if (!city->next) {
        city->next = (Road*)malloc(sizeof(Road));
        tmp = city->next;
    }
    else {
        tmp = city->next;

        while (tmp->next)
            tmp = tmp->next;

        tmp->next = (Road*)malloc(sizeof(Road));
        tmp = tmp->next;
    }

    if (tmp) {
        tmp->city_name = city_direction;
        tmp->length = length;
        tmp->built_year = built_year;
        tmp->next = NULL;
    }
    else {
        check = false;
    }

    return check;
}

bool modifyYear(City *city1, const char *city2, int repair_year) {
    bool check = false;
    Road *tmp = city1->next;

    while (tmp) {
        if (strcmp(tmp->city_name, city2) == 0
            && tmp->built_year <= repair_year) {
            tmp->built_year = repair_year;
            check = true;
        }

        tmp = tmp->next;
    }

    return check;
}

void changeYear(Route *route, City *city1, City *city2, int repair_year) {
    while (route->next) {
        if ((strcmp(route->city_name, city1->city_name) == 0 &&
            strcmp(route->next->city_name, city2->city_name) == 0) ||
            (strcmp(route->city_name, city2->city_name) == 0 &&
            strcmp(route->next->city_name, city1->city_name) == 0)) {
            route->next->built_year = repair_year;
        }

        route = route->next;
    }
}

Route* reverseRoute(Route *route) {
    Route *tmp1 = NULL;
    Route *tmp2;

    if(!route)
        return route;

    tmp2 = route->next;

    while(route->next){
        route->next = tmp1;
        tmp1 = route;
        route = tmp2;
        tmp2 = route->next;
    }

    route->next = tmp1;

    return route;
}

bool copyRoute(Route *route1, Route *route2) {
    bool check = true;

    while (check && route2->next) {
        route2 = route2->next;
        route1->next = (Route*)malloc(sizeof(Route));

        if (route1->next) {
            route1->next->city_name = route2->city_name;
            route1->next->length = route2->length;
            route1->next->built_year = route2->built_year;
            route1->next->route_id = route2->route_id;
            route1 = route1->next;
            route1->next = NULL;
        }
        else {
            check = false;
        }
    }

    return check;
}

void removeRoadBetween(City *city1, const char *city2) {
    Road *tmp, *tmp2;

    tmp = city1->next;

    if (strcmp(tmp->city_name, city2) != 0) {
        while (strcmp(tmp->next->city_name, city2) != 0)
            tmp = tmp->next;

        tmp2 = tmp->next;
        tmp->next = tmp2->next;
        free(tmp2);
    }
    else {
        city1->next = tmp->next;
        free(tmp);
    }
}
