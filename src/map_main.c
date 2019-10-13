#include <stdio.h>
#include <stdlib.h>

#include "map.h"
#include "parser.h"

int main() {
    char *line;
    int character, length, line_counter = 1;
    Map *map = newMap();

    if (map) {
        character = getchar();

        while (character != EOF) {
            ungetc(character, stdin);

            ignoreComments(&line_counter);
            line = getLine(&length);

            if (line) {
                doOrder(map, line, length, line_counter);

                free(line);
                character = getchar();
            }
            else {
                break;
            }
        }
    }

    deleteMap(map);

    return 0;
}