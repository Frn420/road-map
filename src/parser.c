#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input-check.h"
#include "parser.h"

#define UNS_LIM 294967296
#define ADDR_MIN 14
#define ADDR 8
#define EXDRT_MIN 14
#define EXDRT 12
#define GTRT 20
#define NWRT_MIN 12
#define NWRT 9
#define RPRRD_MIN 15
#define RPRRD 11
#define RMVRD_MIN 13
#define RMVRD 11
#define RMVRT 12

int ignoreCommentedLine(int line) {
    int character;

    character = getchar();

    if (character == '#') {
        while (character != '\n' && character != EOF)
            character = getchar();
            
        if (character == EOF)
    	    fprintf(stderr, "ERROR %d\n", line);
    }

    return character;
}

void ignoreComments(int *line) {
    int character;

    do {
        character = ignoreCommentedLine(*line);
        
        if (character == '\n')
            (*line)++;
            
    } while (character == '\n');

    ungetc(character, stdin);
}

char* getLine(int *length) {
    char *line = NULL;
    int character = getchar(), counter = 0;

    while (character != '\n' && character != EOF) {
        line = realloc(line, (counter + 1) * sizeof(char));

        if (line) {
            ungetc(character, stdin);
            scanf("%c", &line[counter]);

            counter++;
            character = getchar();
        }
    }

    ungetc(character, stdin);
    *length = counter;

    return line;
}

bool isNumber(char arg) {
    if (arg == '0' || arg == '1' || arg == '2' || arg == '3' || arg == '4' ||
        arg == '5' || arg == '6' || arg == '7' || arg == '8' || arg == '9')
        return true;
    else
        return false;
}

int getNextSemi(char *line, int length) {
    for (int i = 0; i < length; i++) {
        if (line[i] == ';') {
            return i;
        }
    }

    return length;
}

char* getName(char *name, int length) {
    char *result = NULL;

    result = (char*)malloc((length + 1) * sizeof(char));

    if (result) {
        for (int i = 0; i < length; i++)
            result[i] = name[i];

        result[length] = '\0';
    }

    return result;
}

int charToInt(char arg) {
    return (int)(arg - '0');
}

unsigned getNumber(char *line, int length, int *index) {
    int multiplier = 1;
    unsigned result = 0;

    *index = -1;

    for (int i = 0; i < length; i++) {
        if (line[i] == ';') {
            *index = i;
            break;
        }
    }

    if (*index == -1)
        *index = length;

    if (*index <= 10) {
        for (int i = *index - 1; i >= 0; i--) {
            if (i > 0 || (result < UNS_LIM && charToInt(line[i]) == 4) ||
                charToInt(line[i]) < 4 || *index < 10) {
                result += multiplier * charToInt(line[i]);
                multiplier *= 10;
            }
            else {
                result = 0;
            }
        }
    }

    return result;
}

void freeConst(const char *name) {
    char *tmp_name = NULL;
    
    tmp_name = (char*)name;
    if (tmp_name)
	free(tmp_name);
}

bool checkAddRoadFormat(char *line, int length) {
    bool check = true;
    int counter = 0;

    if (length > ADDR_MIN && line[1] == 'd' && line[2] == 'd' && line[3] == 'R'
        && line[4] == 'o' && line[5] == 'a' && line[6] == 'd') {
        for (int i = ADDR - 1; i < length; i++) {
            if (counter == 3 && line[i] != ';' && !isNumber(line[i]))
                check = false;

            if (counter == 4  && !isNumber(line[i]))
                check = false;

            if (line[i] == ';') {
                counter++;

                if (counter == 4 && i + 1 < length && line[i + 1] == '-')
                    i++;
            }
        }

        if (counter != 4)
            check = false;
    }
    else {
        check = false;
    }

    return check;
}

void addRoadFormat(Map *map, char *line, int line_length, int line_counter) {
    bool check1 = false, check2 = false;
    const char *city1, *city2;
    int build_year, semicolon, semicolon2, tmp;
    unsigned length;

    if (checkAddRoadFormat(line, line_length)) {
        semicolon = ADDR + getNextSemi(&line[ADDR], line_length - ADDR);
        city1 = getName(&line[ADDR], semicolon - ADDR);
        semicolon++;
        semicolon2 = semicolon + getNextSemi(&line[semicolon],
                                             line_length - semicolon);
        city2 = getName(&line[semicolon], semicolon2 - semicolon);
        semicolon2++;
        length = getNumber(&line[semicolon2], line_length - semicolon2,
                           &semicolon);
        semicolon += semicolon2 + 1;

        if (line_length > semicolon && line[semicolon] == '-') {
            semicolon++;
            build_year = -(int)getNumber(&line[semicolon],
                                         line_length - semicolon, &semicolon2);

            if (build_year > 0)
                build_year = 0;
        }
        else if (line_length <= semicolon) {
            build_year = 0;
        }
        else {
            build_year = (int)getNumber(&line[semicolon],
                                        line_length - semicolon, &semicolon2);

            if (build_year < 0)
                build_year = 0;
        }
        
        if (findCity(map, city1, &tmp)) {
    	    freeConst(city1);
    	    city1 = map->cities[tmp]->city_name;
        }
        else {
    	    check1 = true;
        }
        
        if (findCity(map, city2, &tmp)) {
    	    freeConst(city2);
    	    city2 = map->cities[tmp]->city_name;
        }
        else {
    	    check2 = true;
        }

        if (!addRoad(map, city1, city2, length, build_year)) {
    	    if (check1 && !findCity(map, city1, &tmp))
    		freeConst(city1);
    	    if (check2 && !findCity(map, city2, &tmp))
    		freeConst(city2);
    		
            fprintf(stderr, "ERROR %d\n", line_counter);
        }
    }
    else {
        fprintf(stderr, "ERROR %d\n", line_counter);
    }
}

bool checkExtendRouteFormat(char *line, int length) {
    bool check = true;
    int counter = 0;

    if (length > EXDRT_MIN && line[1] == 'x' && line[2] == 't' && line[3] == 'e'
        && line[4] == 'n' && line[5] == 'd' && line[6] == 'R' && line[7] == 'o'
        && line[8] == 'u' && line[9] == 't' && line[10] == 'e') {
        for (int i = EXDRT - 1; i < length; i++) {
            if (counter == 1 && line[i] != ';' && !isNumber(line[i]))
                check = false;

            if (line[i] == ';')
                counter++;
        }

        if (counter != 2)
            check = false;
    }
    else {
        check = false;
    }

    return check;
}

void extendRouteFormat(Map *map, char *line, int length, int line_counter) {
    const char *city;
    int semicolon;
    unsigned route_id;

    if (checkExtendRouteFormat(line, length)) {
        route_id = getNumber(&line[EXDRT], length - EXDRT, &semicolon);
        semicolon += EXDRT;
        semicolon++;
        city = getName(&line[semicolon], length - semicolon);

        if (!extendRoute(map, route_id, city))
            fprintf(stderr, "ERROR %d\n", line_counter);
            
        freeConst(city);
    }
    else {
        fprintf(stderr, "ERROR %d\n", line_counter);
    }
}

bool checkGetRouteFormat(char *line, int length) {
    bool check = true;
    int counter = 0;

    if (length > GTRT && line[1] == 'e' && line[2] == 't' && line[3] == 'R' &&
        line[4] == 'o' && line[5] == 'u' && line[6] == 't' && line[7] == 'e' &&
        line[8] == 'D' && line[9] == 'e' && line[10] == 's' && line[11] == 'c'
        && line[12] == 'r' && line[13] == 'i' && line[14] == 'p' &&
        line[15] == 't' && line[16] == 'i' && line[17] == 'o' &&
        line[18] == 'n') {
        for (int i = GTRT - 1; i < length; i++) {
            if (counter == 1 && !isNumber(line[i]))
                check = false;

            if (line[i] == ';')
                counter++;
        }

        if (counter != 1)
            check = false;
    }
    else {
        check = false;
    }

    return check;
}

void getRouteFormat(Map *map, char *line, int length, int line_counter) {
    char *description = NULL;
    int tmp;
    unsigned route_id;

    if (checkGetRouteFormat(line, length)) {
        route_id = getNumber(&line[GTRT], length - GTRT, &tmp);
        description = (char*)getRouteDescription(map, route_id);

        if (strlen(description) > 0)
            printf("%s\n", description);
        else
            fprintf(stderr, "ERROR %d\n", line_counter);
    }
    else {
        fprintf(stderr, "ERROR %d\n", line_counter);
    }
    
    if (description)
	free(description);
}

bool checkNewRouteFormat(char *line, int length) {
    bool check = true;
    int counter = 0;

    if (length > NWRT_MIN && line[1] == 'e' && line[2] == 'w' && line[3] == 'R'
        && line[4] == 'o' && line[5] == 'u' && line[6] == 't' &&
        line [7] == 'e') {
        for (int i = NWRT - 1; i < length; i++) {
            if (counter == 1 && line[i] != ';' && !isNumber(line[i]))
                check = false;

            if (line[i] == ';')
                counter++;
        }

        if (counter != 3)
            check = false;
    }
    else {
        check = false;
    }

    return check;
}

void newRouteFormat(Map *map, char *line, int length, int line_counter) {
    const char *city1, *city2;
    int semicolon, semicolon2;
    unsigned route_id;

    if (checkNewRouteFormat(line, length)) {
        route_id = getNumber(&line[NWRT], length - NWRT, &semicolon);
        semicolon += NWRT;
        semicolon++;
        semicolon2 = semicolon + getNextSemi(&line[semicolon],
                                             length - semicolon);
        city1 = getName(&line[semicolon], semicolon2 - semicolon);
        semicolon2++;
        semicolon = semicolon2 + getNextSemi(&line[semicolon2],
                                             length - semicolon2);
        city2 = getName(&line[semicolon2], semicolon - semicolon2);

        if (!newRoute(map, route_id, city1, city2))
            fprintf(stderr, "ERROR %d\n", line_counter);
            
        freeConst(city1);
        freeConst(city2);
    }
    else {
        fprintf(stderr, "ERROR %d\n", line_counter);
    }
}

bool checkRepairRoadFormat(char *line, int length) {
    bool check = true;
    int counter = 0;

    if (length > RPRRD_MIN && line[3] == 'a' && line[4] == 'i' && line[5] == 'r'
        && line[6] == 'R' && line[7] == 'o' && line[8] == 'a' &&
        line[9] == 'd') {
        for (int i = RPRRD - 1; i < length; i++) {
            if (counter == 3 && !isNumber(line[i]))
                check = false;

            if (line[i] == ';') {
                counter++;

                if (counter == 3 && i + 1 < length && line[i + 1] == '-')
                    i++;
            }
        }

        if (counter != 3)
            check = false;
    }
    else {
        check = false;
    }

    return check;
}

void repairRoadFormat(Map *map, char *line, int length, int line_counter) {
    const char *city1, *city2;
    int repair_year, semicolon, semicolon2;

    if (checkRepairRoadFormat(line, length)) {
        semicolon = RPRRD + getNextSemi(&line[RPRRD], length - RPRRD);
        city1 = getName(&line[RPRRD], semicolon - RPRRD);
        semicolon++;
        semicolon2 = semicolon + getNextSemi(&line[semicolon],
                                             length - semicolon);
        city2 = getName(&line[semicolon], semicolon2 - semicolon);
        semicolon2++;

        if (length > semicolon2 && line[semicolon2] == '-') {
            semicolon2++;
            repair_year = -(int)getNumber(&line[semicolon2],
                                          length - semicolon2, &semicolon);

            if (repair_year > 0)
                repair_year = 0;
        }
        else if (length <= semicolon) {
            repair_year = 0;
        }
        else {
            repair_year = (int)getNumber(&line[semicolon2], length - semicolon2,
                                         &semicolon);

            if (repair_year < 0)
                repair_year = 0;
        }

        if (!repairRoad(map, city1, city2, repair_year))
            fprintf(stderr, "ERROR %d\n", line_counter);
            
        freeConst(city1);
        freeConst(city2);
    }
    else {
        fprintf(stderr, "ERROR %d\n", line_counter);
    }
}

bool checkRemoveRoadFormat(char *line, int length) {
    bool check = true;
    int counter = 0;

    if (length > RMVRD_MIN && line[1] == 'e' && line[2] == 'm' && line[3] == 'o'
        && line[4] == 'v' && line[5] == 'e' && line[6] == 'R' && line[7] == 'o'
        && line[8] == 'a' && line[9] == 'd') {
        for (int i = RMVRD - 1; i < length; i++)
            if (line[i] == ';')
                counter++;

        if (counter != 2)
            check = false;
    }
    else {
        check = false;
    }

    return check;
}

void removeRoadFormat(Map *map, char *line, int length, int line_counter) {
    const char *city1, *city2;
    int semicolon;

    semicolon = RMVRD + getNextSemi(&line[RMVRD], length - RMVRD);
    city1 = getName(&line[RMVRD], semicolon - RMVRD);
    semicolon++;
    city2 = getName(&line[semicolon], length - semicolon);

    if (!removeRoad(map, city1, city2))
        fprintf(stderr, "ERROR %d\n", line_counter);
        
    freeConst(city1);
    freeConst(city2);
}

bool checkRemoveRouteFormat(char *line, int length) {
    bool check = true;
    int counter = 0;

    if (length > RMVRT && line[1] == 'e' && line[2] == 'm' && line[3] == 'o'
        && line[4] == 'v' && line[5] == 'e' && line[6] == 'R' && line[7] == 'o'
        && line[8] == 'u' && line[9] == 't' && line[10] == 'e') {
        for (int i = RMVRT - 1; i < length; i++) {
            if (counter == 1 && !isNumber(line[i]))
                check = false;

            if (line[i] == ';')
                counter++;
        }

        if (counter != 1)
            check = false;
    }
    else {
        check = false;
    }

    return check;
}

void removeRouteFormat(Map *map, char *line, int length, int line_counter) {
    int tmp;
    unsigned route_id = getNumber(&line[RMVRT], length - RMVRT, &tmp);

    if (!removeRoute(map, route_id))
        fprintf(stderr, "ERROR %d\n", line_counter);
}

void removeFormat(Map *map, char *line, int length, int line_counter) {
    if (checkRemoveRoadFormat(line, length))
        removeRoadFormat(map, line, length, line_counter);
    else if (checkRemoveRouteFormat(line, length))
        removeRouteFormat(map, line, length, line_counter);
    else
        fprintf(stderr, "ERROR %d\n", line_counter);
}

bool checkDescriptionFormat(char *line, int length) {
    bool check = true;
    int counter = 0;

    for (int i = 0; i < length; i++) {
        if ((counter % 3 == 0 || counter % 3 == 2) &&
            line[i] != ';' && !isNumber(line[i]))
            check = false;

        if (line[i] == ';') {
            counter++;

            if (counter % 3 == 0 && i + 1 < length && line[i + 1] == '-')
                i++;
        }
    }

    if (counter == 1 || counter % 3 != 1)
        check = false;

    return check;
}

void descriptionFormat(Map *map, char *line,
                       int line_length, int line_counter) {
    bool check1 = true, check2 = false;
    const char *city1, *city2;
    int built_year, current_year, semicolon, semicolon2, tmp;
    unsigned length, route_id;

    if (checkDescriptionFormat(line, line_length)) {
        route_id = getNumber(line, line_length, &semicolon);
        semicolon++;
        semicolon2 = semicolon + getNextSemi(&line[semicolon],
                                             line_length - semicolon);
        city2 = getName(&line[semicolon], semicolon2 - semicolon);
        semicolon2++;
        
        if (findCity(map, city2, &tmp)) {
    	    freeConst(city2);
    	    city2 = map->cities[tmp]->city_name;
        }
        else {
    	    check1 = addCity(map, city2);
    	    
    	    if (!check1) {
    		freeConst(city2);
    		fprintf(stderr, "ERROR %d\n", line_counter);
    	    }
        }

        while (semicolon2 < line_length) {
            city1 = city2;
            length = getNumber(&line[semicolon2],
                               line_length - semicolon2, &semicolon);
            semicolon += semicolon2 + 1;

            if (line_length > semicolon && line[semicolon] == '-') {
                semicolon++;
                built_year = -(int)getNumber(&line[semicolon],
                                             line_length - semicolon,
                                             &semicolon2);
                if (built_year > 0)
                    built_year = 0;
            }
            else if (line_length <= semicolon) {
                built_year = 0;
            }
            else {
                built_year = (int)getNumber(&line[semicolon],
                                            line_length - semicolon,
                                            &semicolon2);

                if (built_year < 0)
                    built_year = 0;
            }

            semicolon2 += semicolon + 1;
            semicolon = semicolon2 + getNextSemi(&line[semicolon2],
                                                 line_length - semicolon2);
            city2 = getName(&line[semicolon2], semicolon - semicolon2);
            semicolon++;

            current_year = showYear(map, city1, city2, length);
            
            if (findCity(map, city2, &tmp)) {
        	freeConst(city2);
        	city2 = map->cities[tmp]->city_name;
            }
            else {
        	check2 = true;
            }

            if (current_year < built_year && current_year != 0) {
                if (!repairRoad(map, city1, city2, built_year)) {
                    fprintf(stderr, "ERROR %d\n", line_counter);
                    break;
                }
            }
            else if (current_year == 0) {
                if (!addRoad(map, city1, city2, length, built_year)) {
            	    if (check2 && !findCity(map, city2, &tmp))
            		freeConst(city2);
            		
                    fprintf(stderr, "ERROR %d\n", line_counter);
                    break;
                }
            }
            else if (current_year != built_year) {
                fprintf(stderr, "ERROR %d\n", line_counter);
                break;
            }

            if (!addToRoute(map, route_id, city1, city2)) {
                fprintf(stderr, "ERROR %d\n", line_counter);
                break;
            }
            
            check2 = false;
            tmp = semicolon;
            semicolon = semicolon2;
            semicolon2 = tmp;
        }
    }
    else {
        fprintf(stderr, "ERROR %d\n", line_counter);
    }
}

void doOrder(Map *map, char *line, int length, int line_counter) {
    if (length > 0) {
        if (line[0] == 'a')
            addRoadFormat(map, line, length, line_counter);
        else if (line[0] == 'e')
            extendRouteFormat(map, line, length, line_counter);
        else if (line[0] == 'g')
            getRouteFormat(map, line, length, line_counter);
        else if (line[0] == 'n')
            newRouteFormat(map, line, length, line_counter);
        else if (length > 3 && line[0] == 'r' && line[1] == 'e' &&
                 line[2] == 'p')
            repairRoadFormat(map, line, length, line_counter);
        else if (line[0] == 'r')
            removeFormat(map, line, length, line_counter);
        else if (isNumber(line[0]))
            descriptionFormat(map, line, length, line_counter);
        else
            fprintf(stderr, "ERROR %d\n", line_counter);
    }
}
