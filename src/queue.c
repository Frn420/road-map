#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cities.h"
#include "input-check.h"
#include "map.h"
#include "queue.h"

struct Queue {
    unsigned length;       ///< długość danegoo miasta do źródła
    int built_year;        ///< rok budowy lub ostatniego remontu danego odcinka
    int oldest_road;       ///< rok najstarszego odcinka w danej ścieżce
    struct City *current;  ///< wskaźnik na obecne miasto
    struct City *previous; ///< wskaźnik na poprzednie miasto
    struct Queue *next;    ///< wskaźnik na kolejny element kolejki priorytetowej
};

void deleteQueue(Queue *queue) {
    if (queue) {
        Queue *tmp = queue->next;
        free(queue);
        deleteQueue(tmp);
    }
}

bool checkNode(Queue *queue, const char *name) {
    bool check = false;

    while (queue) {
        if (strcmp(queue->current->city_name, name) == 0)
            check = true;

        queue = queue->next;
    }

    return check;
}

Queue* reverseQueue(Queue *queue) {
    Queue *tmp1 = NULL;
    Queue *tmp2;

    if(!queue)
        return queue;

    tmp2 = queue->next;

    while(queue->next){
        queue->next = tmp1;
        tmp1 = queue;
        queue = tmp2;
        tmp2 = queue->next;
    }

    queue->next = tmp1;

    return queue;
}

void update(Queue *queue, Road *new_node) {
    Queue *tmp = queue, *tmp2;
    unsigned new_length = queue->length + new_node->length;

    if (checkNode(queue, new_node->city_name)) {
        while (strcmp(tmp->next->current->city_name, new_node->city_name) != 0)
            tmp = tmp->next;

        if (new_length < tmp->next->length || (new_length == tmp->next->length
            && min(new_node->built_year, queue->oldest_road)
            > tmp->next->oldest_road)) {
            tmp->next->length = new_length;
            tmp->next->built_year = new_node->built_year;
            tmp->next->oldest_road = min(new_node->built_year,
                                         queue->oldest_road);
            tmp->next->previous = queue->current;

            while (queue->next->length < new_length)
                queue = queue->next;

            tmp2 = tmp->next;
            tmp->next = tmp2->next;
            tmp2->next = queue->next;
            queue->next = tmp2;
        }
        else if (new_length == tmp->next->length &&
                 min(new_node->built_year, queue->oldest_road)
                 == tmp->next->oldest_road) {
            tmp->next->previous = NULL;
        }
    }
}

bool insert(Map *map, Queue *queue, Road *new_node) {
    bool check;
    int city_index;
    int oldest;
    City *source = queue->current;
    Queue *tmp;
    unsigned new_length = queue->length + new_node->length;

    if (queue->length == 0)
        oldest = new_node->built_year;
    else
        oldest = queue->oldest_road;

    check = findCity(map, new_node->city_name, &city_index);

    while (queue->next && (queue->next->length < new_length))
        queue = queue->next;

    tmp = queue->next;

    if (check) {
        queue->next = (Queue*)malloc(sizeof(Queue));

        if (queue->next) {
            if (oldest > new_node->built_year)
                queue->next->oldest_road = new_node->built_year;
            else
                queue->next->oldest_road = oldest;

            queue->next->current = map->cities[city_index];
            queue->next->previous = source;
            queue->next->length = new_length;
            queue->next->built_year = new_node->built_year;
            queue->next->next = tmp;
        }
        else {
            check = false;
        }
    }

    return check;
}

bool createNewRoute(Queue *queue, Route *route, unsigned route_id) {
    bool check = true;
    const char *tmp;

    while (check && queue->next) {
        if (!queue->previous)
            return false;

        tmp = queue->previous->city_name;

        while (queue->next && strcmp(tmp, queue->current->city_name) != 0)
            queue = queue->next;

        route->length -= queue->length;
        route->next = (Route*)malloc(sizeof(Route));

        if (route->next) {
            route->next->city_name = queue->current->city_name;
            route->next->length = queue->length;
            route->next->built_year = queue->built_year;
            route->next->route_id = route_id;
            route = route->next;
            route->next = NULL;
        }
        else {
            check = false;
        }
    }

    return check;
}

bool createRoute(Map *map, unsigned route_id, City *city1, City *city2) {
    bool check = true;
    Queue *tmp;
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    Road *current;
    Route *new_route, **memcheck = NULL;

    if (queue) {
        queue->length = 0;
        queue->current = city1;
        queue->next = NULL;
    }
    else {
        check = false;
    }

    tmp = queue;

    while (check && strcmp(tmp->current->city_name, city2->city_name) != 0) {
        current = tmp->current->next;

        while (check && current) {
            if (checkNode(queue, current->city_name))
                update(tmp, current);
            else
                check = insert(map, tmp, current);

            current = current->next;
        }

        tmp = tmp->next;
    }

    if (check) {
        deleteQueue(tmp->next);
        tmp->next = NULL;
        queue = reverseQueue(queue);
    }

    new_route = (Route*)malloc(sizeof(Route));

    if (check && new_route) {
        new_route->city_name = queue->current->city_name;
        new_route->length = queue->length;
        new_route->built_year = queue->built_year;
        new_route->route_id = route_id;
        new_route->next = NULL;
    }
    else {
        check = false;
    }

    if (check)
        check = createNewRoute(queue, new_route, route_id);

    if (check) {
        memcheck = (Route**)malloc(sizeof(Route*));

        if (memcheck) {
            free(memcheck);
            (map->number_of_routes)++;
            map->routes = realloc(map->routes,
                                  map->number_of_routes * sizeof(Route*));
            map->routes[map->number_of_routes - 1] = reverseRoute(new_route);
        }
        else {
            deleteRoute(new_route);
            check = false;
        }
    }
    else {
        deleteRoute(new_route);
    }

    deleteQueue(queue);

    return check;
}

bool findNewRoute(Map *map, Route *route, Queue *queue,
                  City *destination, int *length) {
    bool check = true;
    Queue *tmp = queue;
    Road *current;

    while (tmp &&
           strcmp(tmp->current->city_name, destination->city_name) != 0) {
        current = tmp->current->next;

        while (current) {
            if (!checkCity(route, current->city_name)) {
                if (checkNode(queue, current->city_name))
                    update(tmp, current);
                else
                    insert(map, tmp, current);
            }

            current = current->next;
        }

        tmp = tmp->next;

        if (tmp &&
            strcmp(tmp->current->city_name, destination->city_name) == 0) {
            *length = (int)tmp->length;
            check = true;
        }
        else {
            check = false;
        }
    }

    return check;
}

int findOldest(Queue *queue, const char *name) {
    while (strcmp(queue->current->city_name, name) != 0)
        queue = queue->next;

    return queue->oldest_road;
}

bool extend(Map *map, Route *route, City *city) {
    bool check, check2 = false;
    int city_index = -1, length, length2, oldest, oldest2, route_index;
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    Queue *queue2 = (Queue*)malloc(sizeof(Queue));
    Queue *tmp_queue;
    Route *new_route, *tmp = route;

    check = findCity(map, route->city_name, &city_index);

    if (check && queue) {
        queue->length = 0;
        queue->current = map->cities[city_index];
        queue->next = NULL;
    }
    else {
        check = false;
    }

    if (check)
        check2 = findNewRoute(map, route, queue, city, &length);

    while (tmp->next)
        tmp = tmp->next;

    findCity(map, tmp->city_name, &city_index);

    if (queue2) {
        queue2->length = 0;
        queue2->current = map->cities[city_index];
        queue2->next = NULL;
    }
    else {
        check = false;
    }

    if (check)
        check = findNewRoute(map, route, queue2, city, &length2);

    if (check && check2) {
        if (length > length2) {
            deleteQueue(queue);
            queue = queue2;
            check2 = false;
        }
        else if (length < length2) {
            deleteQueue(queue2);
        }
        else {
            oldest = findOldest(queue, city->city_name);
            oldest2 = findOldest(queue2, city->city_name);
            if (oldest > oldest2) {
                deleteQueue(queue2);
            }
            else if (oldest < oldest2) {
                deleteQueue(queue);
                queue = queue2;
                check2 = false;
            }
            else {
                check = false;
            }
        }
    }
    else if (check) {
        deleteQueue(queue);
        queue = queue2;
    }
    else if (check2) {
        check = true;
        deleteQueue(queue2);
    }

    tmp_queue = queue;

    if (check) {
        while (strcmp(tmp_queue->current->city_name, city->city_name) != 0)
            tmp_queue = tmp_queue->next;

        deleteQueue(tmp_queue->next);
        tmp_queue->next = NULL;
        queue = reverseQueue(queue);

        new_route = (Route*)malloc(sizeof(Route));

        if (new_route) {
            new_route->city_name = queue->current->city_name;
            new_route->length = queue->length;
            new_route->built_year = queue->built_year;
            new_route->route_id = route->route_id;
            new_route->next = NULL;
        }
        else {
            check = false;
        }

        if (check)
            check = createNewRoute(queue, new_route, new_route->route_id);

        if (check) {
            new_route = reverseRoute(new_route);
        }
        else {
            deleteRoute(new_route);
            return check;
        }

        if (check && check2) {
            while (new_route->next) {
                route->length = new_route->next->length;
                route->built_year = new_route->next->built_year;

                tmp = route;
                route = new_route;
                new_route = new_route->next;
                route->next = tmp;

                route->city_name = new_route->city_name;
            }
            
            findRoute(map, route->route_id, &route_index);
            map->routes[route_index] = route;
        }
        else {
            tmp->next = new_route->next;
            free(new_route);
        }
    }

    deleteQueue(queue);

    return check;
}

bool completeRoute(Map *map, Route *route, City *city1, City *city2) {
    bool check = true;
    Queue *tmp;
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    Road *current;
    Route *new_route, *tmp_route, *tmp2_route;

    if (queue) {
        queue->length = 0;
        queue->current = city1;
        queue->next = NULL;
    }
    else {
        check = false;
    }

    tmp = queue;

    while (check && tmp &&
           strcmp(tmp->current->city_name, city2->city_name) != 0) {
        current = tmp->current->next;

        while (check && current) {
            if (!checkCity(route, current->city_name) ||
                (strcmp(current->city_name, city2->city_name) == 0
                 && tmp != queue)) {
                if (checkNode(queue, current->city_name))
                    update(tmp, current);
                else
                    check = insert(map, tmp, current);
            }

            current = current->next;
        }

        tmp = tmp->next;
    }

    if (!tmp)
        check = false;

    if (check) {
        deleteQueue(tmp->next);
        tmp->next = NULL;
        queue = reverseQueue(queue);
    }

    if (check) {
        new_route = (Route*)malloc(sizeof(Route));

        if (new_route) {
            new_route->city_name = queue->current->city_name;
            new_route->length = queue->length;
            new_route->built_year = queue->built_year;
            new_route->route_id = route->route_id;
            new_route->next = NULL;
        }
        else {
            check = false;
        }

        if (check)
            check = createNewRoute(queue, new_route, route->route_id);

        if (!check) {
            deleteRoute(new_route);
        }
        else {
            while (strcmp(route->city_name, city1->city_name) != 0 &&
                   strcmp(route->city_name, city2->city_name) != 0)
                route = route->next;

            tmp_route = route->next;
            tmp_route->built_year = new_route->built_year;
            tmp_route->length = new_route->length;

            if (strcmp(route->city_name, city1->city_name) == 0) {
                new_route = reverseRoute(new_route->next);
                route->next = new_route->next;
                free(new_route);

                while (route->next && strcmp(route->next->city_name, city2->city_name) != 0)
                    route = route->next;
            } else {
                tmp2_route = new_route;
                route->next = new_route->next;
                free(tmp2_route);

                while (strcmp(route->next->city_name, city1->city_name) != 0)
                    route = route->next;
            }

            route->next = tmp_route;
        }
    }

    deleteQueue(queue);

    return check;
}
