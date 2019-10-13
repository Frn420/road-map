#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cities.h"
#include "map.h"

bool checkName(const char *city_name) {
    bool check = true;
    size_t loop_length = strlen(city_name);

    if (loop_length == 0)
        check = false;

    for (size_t i = 0; i < loop_length; i++)
        if ((city_name[i] < ' ' && city_name[i] >= '\0') || city_name[i] == ';')
            check = false;

    return check;
}

bool findCity(Map *map, const char *city_name, int *city_index) {
    bool check = false;
    int map_length = 0;

    if (map)
        map_length = map->number_of_cities;

    for (int i = 0; i < map_length; i++) {
        if (map && strcmp(map->cities[i]->city_name, city_name) == 0) {
            check = true;
            *city_index = i;
        }
    }

    return check;
}

bool checkRoad(City *city1, City *city2) {
    bool check = false;
    Road *tmp1 = NULL;
    Road *tmp2 = NULL;

    if (city1)
        tmp1 = city1->next;

    if (city2)
        tmp2 = city2->next;

    while (tmp1) {
        if (city2 && strcmp(tmp1->city_name, city2->city_name) == 0)
            check = true;

        tmp1 = tmp1->next;
    }

    while (tmp2) {
        if (city1 && strcmp(tmp2->city_name, city1->city_name) == 0)
            check = true;

        tmp2 = tmp2->next;
    }

    return check;
}

int min(int number1, int number2) {
    if (number1 > number2)
        return number2;
    else
        return number1;
}

bool findRoute(Map *map, unsigned route_id, int *route_index) {
    bool check = false;

    for (int i = 0; i < map->number_of_routes; i++) {
        if (map->routes[i]->route_id == route_id) {
            check = true;
            *route_index = i;
        }
    }

    return check;
}

bool checkCity(Route *route, const char *city_name) {
    bool check = false;

    while (route) {
        if (strcmp(route->city_name, city_name) == 0)
            check = true;

        route = route->next;
    }

    return check;
}

int getIntLength(int number) {
    int counter = 0;

    while (number > 0) {
        number /= 10;
        counter++;
    }

    return counter;
}

int getUnsignedLength(unsigned number) {
    int counter = 0;

    while (number > 0) {
        number /= 10;
        counter++;
    }

    return counter;
}