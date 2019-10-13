#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cities.h"
#include "input-check.h"
#include "map.h"
#include "queue.h"

#define ID_MAX 999
#define INT_MAX_LENGTH 10

Map* newMap(void) {
    Map *new_map = (Map*)malloc(sizeof(Map));

    if (new_map) {
        new_map->number_of_cities = 0;
        new_map->number_of_routes = 0;
        new_map->cities = NULL;
        new_map->routes = NULL;
    }

    return new_map;
}

void deleteMap(Map *map) {
    if (map) {
        for (int i = 0; i < map->number_of_cities; i++)
            deleteNode(map->cities[i]);

        for (int i = 0; i < map->number_of_routes; i++)
            deleteRoute(map->routes[i]);

        free(map->cities);
        free(map->routes);
        free(map);
    }
}

bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int built_year) {
    bool check = true;
    City *city1_tmp = NULL;
    City *city2_tmp = NULL;
    int city1_index, city2_index;

    if (!map || strcmp(city1, city2) == 0 || built_year == 0)
        check = false;

    if (!checkName(city1) || !checkName(city2) || length == 0)
        check = false;

    if (check && !findCity(map, city1, &city1_index)) {
        check = addCity(map, city1);
        city1_index = map->number_of_cities - 1;
    }

    if (check && !findCity(map, city2, &city2_index)) {
        check = addCity(map, city2);
        city2_index = map->number_of_cities - 1;
    }

    if (check) {
        city1_tmp = map->cities[city1_index];
        city2_tmp = map->cities[city2_index];
        check = !checkRoad(city1_tmp, city2_tmp);
    }

    if (check)
        check = createRoad(city1_tmp, city2, length, built_year);

    if (check)
        check = createRoad(city2_tmp, city1, length, built_year);

    return check;
}

bool repairRoad(Map *map, const char *city1,
                const char *city2, int repair_year) {
    bool check = true;
    int city1_index, city2_index;

    if (!map || strcmp(city1, city2) == 0 || repair_year == 0)
        check = false;

    if (!checkName(city1) || !checkName(city2))
        check = false;

    if (check && findCity(map, city1, &city1_index)
              && findCity(map, city2, &city2_index))
        check = checkRoad(map->cities[city1_index], map->cities[city2_index]);
    else
        check = false;

    if (check)
        check = modifyYear(map->cities[city1_index], city2, repair_year);

    if (check)
        check = modifyYear(map->cities[city2_index], city1, repair_year);

    if (check)
        for (int i = 0; i < map->number_of_routes; i++)
            changeYear(map->routes[i], map->cities[city1_index],
                       map->cities[city2_index], repair_year);

    return check;
}

int showYear(Map *map, const char *city1, const char *city2, unsigned length) {
    int city1_index, city2_index, result;
    Road *check;

    if (map && strcmp(city1, city2) != 0 && checkName(city1) && checkName(city2)
        && findCity(map, city1, &city1_index) &&
        findCity(map, city2, &city2_index) &&
        checkRoad(map->cities[city1_index], map->cities[city2_index])) {
        check = map->cities[city1_index]->next;

        while (strcmp(check->city_name, city2) != 0)
            check = check->next;

        if (check->length == length)
            result = check->built_year;
        else
            result = INT_MAX;
    }
    else {
        result = 0;
    }

    return result;
}

bool newRoute(Map *map, unsigned route_id,
              const char *city1, const char *city2) {
    bool check = true;
    int city1_index, city2_index, tmp;

    if (!map || route_id == 0 || route_id > ID_MAX || strcmp(city1, city2) == 0)
        check = false;

    if (check && (!checkName(city1) || !checkName(city2) ||
        findRoute(map, route_id, &tmp)))
        check = false;

    if (check && findCity(map, city1, &city1_index)
              && findCity(map, city2, &city2_index))
        check = createRoute(map, route_id, map->cities[city1_index],
                            map->cities[city2_index]);
    else
        check = false;

    return check;
}

bool extendRoute(Map *map, unsigned route_id, const char *city) {
    bool check = true;
    int city_index = -1, route_index = -1;

    if (!map || route_id == 0 || route_id > ID_MAX)
        check = false;

    if (check && (!checkName(city) || !findCity(map, city, &city_index)))
        check = false;

    if (check && !findRoute(map, route_id, &route_index))
        check = false;

    if (check && !checkCity(map->routes[route_index], city)) {
        check = extend(map, map->routes[route_index], map->cities[city_index]);
    }
    else {
        check = false;
    }

    return check;
}

bool addToRoute(Map *map, unsigned route_id,
                const char *city1, const char *city2) {
    bool check = true;
    int city1_index, city2_index, route_index;
    Road *road = NULL;
    Route **tmp, *route;

    if (!map || route_id == 0 || route_id > ID_MAX)
        check = false;

    if (check && (!checkName(city1) || !findCity(map, city1, &city1_index)))
        check = false;

    if (check && (!checkName(city2) || !findCity(map, city2, &city2_index)))
        check = false;

    if (check) {
        road = map->cities[city1_index]->next;
        while (strcmp(road->city_name,
               map->cities[city2_index]->city_name) != 0)
            road = road->next;
    }

    if (check && !findRoute(map, route_id, &route_index)) {
        route = (Route*)malloc(sizeof(Route));

        if (route) {
            route->length = 0;
            route->city_name = map->cities[city1_index]->city_name;
            route->route_id = route_id;
            route->next = (Route*)malloc(sizeof(Route));

            if (route->next) {
                route->next->built_year = road->built_year;
                route->next->length = road->length;
                route->next->city_name = map->cities[city2_index]->city_name;
                route->next->route_id = route_id;
                route->next->next = NULL;
            }
        }

        tmp = (Route**)malloc(sizeof(Route*));

        if (route && tmp) {
            free(tmp);

            (map->number_of_routes)++;
            map->routes = realloc(map->routes,
                                  map->number_of_routes * sizeof(Route*));
            map->routes[map->number_of_routes - 1] = route;
        }
        else if (route) {
            deleteRoute(route);
        }
    }
    else if (check && findRoute(map, route_id, &route_index)) {
        route = map->routes[route_index];

        while (route->next) {
            route = route->next;

            if (strcmp(route->city_name,
                       map->cities[city2_index]->city_name) == 0)
                check = false;
        }

        if (check && strcmp(city1, route->city_name) == 0) {
            route->next = (Route*)malloc(sizeof(Route));
            route->next->built_year = road->built_year;
            route->next->length = road->length;
            route->next->city_name = map->cities[city2_index]->city_name;
            route->next->route_id = route_id;
            route->next->next = NULL;
        }
        else {
            check = false;
        }
    }

    return check;
}

bool removeRoad(Map *map, const char *city1, const char *city2) {
    bool check = true;
    int city1_index, city2_index = -1;
    Route *tmp[map->number_of_routes];

    for (int i = 0; check && i < map->number_of_routes; i++) {
        tmp[i] = (Route*)malloc(sizeof(Route));

        if (tmp[i]) {
            tmp[i]->city_name = map->routes[i]->city_name;
            tmp[i]->length = map->routes[i]->length;
            tmp[i]->built_year = map->routes[i]->built_year;
            tmp[i]->route_id = map->routes[i]->route_id;
            tmp[i]->next = NULL;
        }
        else {
            check = false;
        }

        if (check) {
            check = copyRoute(tmp[i], map->routes[i]);
        }
    }

    if (!map || !checkName(city1) || !checkName(city2))
        check = false;

    if (check && (!findCity(map, city1, &city1_index) ||
        !findCity(map, city2, &city2_index)))
        check = false;

    if (check)
        check = checkRoad(map->cities[city1_index], map->cities[city2_index]);

    if (check)
        for (int i = 0; i < map->number_of_routes; i++)
            if (checkCity(map->routes[i], city1) &&
                checkCity(map->routes[i], city2))
                if (!completeRoute(map, map->routes[i],
                    map->cities[city1_index],map->cities[city2_index]))
                    check = false;

    if (check) {
        for (int i = 0; i < map->number_of_routes; i++) {
            deleteRoute(tmp[i]);
        }

        removeRoadBetween(map->cities[city1_index], city2);
        removeRoadBetween(map->cities[city2_index], city1);
    }
    else {
        for (int i = 0; i < map->number_of_routes; i++) {
            deleteRoute(map->routes[i]);
            map->routes[i] = tmp[i];
        }
    }

    return check;
}

char const* getRouteDescription(Map *map, unsigned route_id) {
    bool check = true;
    char *description = (char*)malloc(sizeof(char));
    char number[INT_MAX_LENGTH];
    int length = 0;
    Route *route = NULL;

    if (description)
        description[0] = '\0';

    if (!map)
        check = false;

    if (check)
        for (int i = 0; i < map->number_of_routes; i++)
            if (map->routes[i]->route_id == route_id)
                route = map->routes[i];

    if (!route)
        check = false;

    if (check) {
      length += getUnsignedLength(route_id);
      description = realloc(description, 2 * length * sizeof(char));
      sprintf(number, "%u", route_id);
      strcat(description, number);
      length++;
      description = realloc(description, 2 * length * sizeof(char));
      strcat(description, ";");

      while (route->next) {
          length += strlen(route->city_name);
          description = realloc(description, 2 * length * sizeof(char));
          strcat(description, route->city_name);
          length++;
          strcat(description, ";");
          length += getUnsignedLength(route->next->length);
          description = realloc(description, 2 * length * sizeof(char));
          sprintf(number, "%u", route->next->length);
          strcat(description, number);
          length++;
          strcat(description, ";");
          length += getIntLength(route->next->built_year);
          description = realloc(description, 2 * length * sizeof(char));
          sprintf(number, "%d", route->next->built_year);
          strcat(description, number);
          length++;
          strcat(description, ";");

          route = route->next;
        }

      length += strlen(route->city_name);
      description = realloc(description, 2 * length * sizeof(char*));
      strcat(description, route->city_name);
    }

    return description;
}

bool removeRoute(Map *map, unsigned route_id) {
    bool check = true;
    int index;

    if (!map || route_id == 0 || route_id > ID_MAX)
        check = false;

    if (check && findRoute(map, route_id, &index)) {
        deleteRoute(map->routes[index]);

        for (int i = index + 1; i < map->number_of_routes; i++)
            map->routes[i - 1] = map->routes[i];

        (map->number_of_routes)--;
    }
    else {
        check = false;
    }

    return check;
}