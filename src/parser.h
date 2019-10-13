#ifndef __PARSER_H__
#define __PARSER_H__

#include "map.h"

/** @brief Pomija linie zaczynające się od '#'.
 * @param[in] line - numer linii.
 * @return Kod pierwszego znaku następnej linii.
 */
int ignoreCommentedLine(int line);

/** @brief Pomija linie puste i linie zaczynające się od '#'.
 * @param[in, out] line - numer linii.
 */
void ignoreComments(int *line);

/** @brief Zwraca ciąg znaków reprezentujący linię wejścia.
 * Funkcja alokuje pamięć na ciąg znaków i zwraca długość ciągu.
 * @param[in, out] length - długość zwracanego ciągu.
 * @return Ciąg znaków.
 */
char* getLine(int *length);

/** @brief Sprawdza czy znak koduje cyfrę.
 * @param[in] arg - interpretowany znak.
 * @return Wartość @true, znak koduje cyfrę. Wartość @false w
 * przeciwnym przypadku.
 */
bool isNumber(char arg);

/** @brief Funkcja szuka najbliższego średnika w ciągu znaków.
 * @param[in] line   - ciąg znaków.
 * @param[in] length - długość ciągu.
 * @return Indeks najbliższego średnika lub długość ciągu jeśli
 * średnik nie występuje.
 */
int getNextSemi(char *line, int length);

/** @brief Kopiuje ciąg znaków.
 * @param[in] line   - ciąg znaków.
 * @param[in] length - długość ciągu.
 * @return Skopiowany ciąg znaków.
 */
char* getName(char *name, int length);

/** @brief Zwraca liczbę którą koduje znak.
 * @param[in] arg - znak.
 * @return Liczba.
 */
int charToInt(char arg);

/** @brief Zwraca liczbę zapisaną w ciągu znaków.
 * Analizuje znaki do najbliższego średnika lub do końca ciągu
 * i zwraca tą liczbę oraz indeks najbliższego średnika.
 * @param[in] line       - ciąg znaków.
 * @param[in] length     - długość ciągu.
 * @param[in, out] index - indeks najbliższego średnika.
 * @return Zwraca liczbę lub 0 jeśli przed średnikiem nie występuje liczba.
 */
unsigned getNumber(char *line, int length, int *index);

/** @brief Zwalnia pamięć typu const char.
 * @param[in] name - argument który chcemy zwolnić.
 */
void freeConst(const char *name);

/** @brief Sprawdza, czy ciąg znaków jest typu addRoad.
 * Sprawdza, czy w ciągu jest polecenie addRoad, liczbę średników
 * w ciągu oraz czy w odpowiednich miejscach są liczby.
 * @param[in] line   - ciąg znaków.
 * @param[in] length - długość ciągu.
 * @return Wartość @true jeśli polecenie jest typu addRoad.
 * Wartość @false w przeciwnym przypadku.
 */
bool checkAddRoadFormat(char *line, int length);

/** @brief Wywołuje polecenie addRoad.
 * Sprawdza polecenie, parsuje ciąg, wywołuje funkcję
 * na mapie dróg i wypisuje błąd gdy któryś parametr był niepoprawny.
 * @param[in, out] map     - mapa dróg.
 * @param[in] line         - ciąg znaków.
 * @param[in] line_length  - długość ciągu.
 * @param[in] line_counter - numer linii wejścia.
 */
void addRoadFormat(Map *map, char *line, int line_length, int line_counter);

/** @brief Sprawdza, czy ciąg znaków jest typu ectendRoute.
 * Sprawdza, czy w ciągu jest polecenie extendRoute, liczbę średników
 * w ciągu oraz czy w odpowiednich miejscach są liczby.
 * @param[in] line   - ciąg znaków.
 * @param[in] length - długość ciągu.
 * @return Wartość @true, jeśli polecenie jest typu extendRoute.
 * Wartość @false w przeciwbym przypadku.
 */
bool checkExtendRouteFormat(char *line, int length);

/** @brief Wywołuje polecenie extendRoute.
 * Sprawdza polecenie, parsuje ciąg, wywołuje funkcję na mapie dróg
 * i wypisuje błąd gdy któryś parametr był niepoprawny.
 * @param[in, out] map     - mapa dróg.
 * @param[in] line         - ciąg znaków.
 * @param[in] length       - długość ciągu.
 * @param[in] line_counter - numer linii.
 */
void extendRouteFormat(Map *map, char *line, int length, int line_counter);

/** @brief Sprawdza, czy ciąg znaków jest typu getRouteDescription.
 * Sprawdza, czy w ciągu jest polecenie getRouteDescription, liczbę
 * średników i czy w odpowiednich miejscach są licby.
 * @param[in] line   - ciąg znaków.
 * @param[in] length - długość ciągu.
 * @return Watość @true, jeśli polecenie jest typu getRouteDescription.
 * Wartość @false w przeciwnym przypadku.
 */
bool checkGetRouteFormat(char *line, int length);

/** @brief Wywołuje polecenie getRouteDescription.
 * Sprawdza polecenie, parsuje ciąg, wywołue funkcję na mapie dróg
 * i wypisuje błąd, gdy któryś parametr był niepoprawny.
 * @param[in, out] map     - mapa dróg.
 * @param[in] line         - ciąg znaków.
 * @param[in] length       - dugość ciągu.
 * @param[in] line_counter - numer linii.
 */
void getRouteFormat(Map *map, char *line, int length, int line_counter);

/** @brief Sprawdza, czy ciąg znaków jest typu newRoute.
 * Sprawdza, czy w ciągu jest polecenie newRoute, liczbę średników
 * i czy w odpowiednich miejscach są liczby.
 * @param[in] line   - ciąg znaków.
 * @param[in] length - długość ciągu.
 * @return Wartość @true, jeśli polecenie jest typu newRoute.
 * Wartość @false w przeciwnym przypadku.
 */
bool checkNewRouteFormat(char *line, int length);

/** Wywołuje polecenie newRoute.
 * Sprawdza polecenie, parsuje ciąg, wywołuje funkcję na mapie dróg
 * i wypisuje błąd, gdy któryś parametr był niepoprawny.
 * @param[in, out] map     - mapa dróg.
 * @param[in] line         - ciąg znaków.
 * @param[in] length       - długość ciągu.
 * @param[in] line_counter - numer linii.
 */
void newRouteFormat(Map *map, char *line, int length, int line_counter);

/** @brief Sprawdza, czy ciąg znaków jest typu repairRoad.
 * Sprawdza, czy w ciągu jest polecenie repairRoad, liczbę średników
 * i czy w odpowiednich miejscach są liczby.
 * @param[in] line   - ciąg znaków.
 * @param[in] length - długość ciągu.
 * @return Wartość @true, jeśli polecenie jest typu repairRoad.
 * Wartość @false w przeciwnym przypadku.
 */
bool checkRepairRoadFormat(char *line, int length);

/** @brief Wywołuje polecenie repairRoad.
 * Sprawdza polecenie, parsuje ciąg, wywołuje funkcję na mapie dróg
 * i wypisuje błąd, gdy któryś parametr był niepoprawny.
 * @param[in, out] map     - mapa dróg.
 * @param[in] line         - ciąg znaków.
 * @param[in] length       - długość ciągu.
 * @param[in] line_counter - numer linii.
 */
void repairRoadFormat(Map *map, char *line, int length, int line_counter);

/** @brief Sprawdza, czy ciąg znaków jest typu removeRoad.
 * Sprawdza, czy w ciągu jest polecenie removeRoad i liczbę średników.
 * @param[in] line   - ciąg znaków.
 * @param[in] length - długość ciągu.
 * @return Wartość @true, jeśli polecenie jest typu removeRoad.
 * Wartość @false w przeciwnym przypadku.
 */
bool checkRemoveRoadFormat(char *line, int length);

/** @brief Wywołuje polecienie removeRoad.
 * Parsuje ciąg, wywołuje funkcję na mapie dróg i wypisuje błąd,
 * gdy któryś parametr był niepoprawny.
 * @param[in, out] map     - mapa dróg.
 * @param[in] line         - ciąg znaków.
 * @param[in] length       - długość ciągu.
 * @param[in] line_counter - numer linii.
 */
void removeRoadFormat(Map *map, char *line, int length, int line_counter);

/** @brief Sprawdza, czy ciąg znaków jest typu removeRoute.
 * Sprawdza, czy w ciągu jest polecenie remeveRoute, liczbę średników
 * i czy w odpowiednich miejscach są liczby.
 * @param[in] line   - ciąg znaków.
 * @param[in] length - długość ciągu.
 * @return Wartość @true, jeśli poecenie jest typu removeRoute.
 * Wartość @false w preciwnym przypadku.
 */
bool checkRemoveRouteFormat(char *line, int length);

/** @brief Wywołuje polecenie removeRoute.
 * Parsuje ciąg, wywołuje funkcję na mapie dróg i wypisuje błąd, gdy
 * któryś parametr był niepoprawny.
 * @param[in, out] map     - mapa dróg.
 * @param[in] line         - ciąg znaków.
 * @param[in] length       - długość ciągu.
 * @param[in] line_counter - numer linii.
 */
void removeRouteFormat(Map *map, char *line, int length, int line_counter);

/** @brief Sprawdza,czy wywołać któreś z poleceń remove.
 * Sprawdza, czy polecenie było typu removeRoad, czy removeRoute,
 * wywołuje je i wypisuje błąd, jeśli to nie było żadne z nich.
 * @param[in, out] map     - mapa dróg.
 * @param[in] line         - ciąg znaków.
 * @param[in] length       - długość ciągu.
 * @param[in] line_counter - numer linii.
 */
void removeFormat(Map *map, char *line, int length, int line_counter);

/** @brief Sprawdza, czy opis drogi jest poprawny.
 * Sprawdza ilość średników i czy w odpowiednich miejscach są liczby.
 * @param[in] line   - ciąg znaków.
 * @param[in] length - długość ciągu.
 * @return Wartość @true, jeśli opis jest poprawny. Wartość @false
 * w przeciwnym przypadku.
 */
bool checkDescriptionFormat(char *line, int length);

/** @brief Tworzy drogę krajowym o podanym opisie.
 * Sprawdza opis, parsuje ciąg tworzy drogę krajową na mapie dróg
 * i wywołuje błąd, gdy któryś parametr był niepoprawny.
 * @param[in, out] map     - mapa dróg.
 * @param[in] line         - ciąg znaków.
 * @param[in] line_length  - długość ciągu.
 * @param[in] line_counter - numer linii.
 */
void descriptionFormat(Map *map, char *line, int line_length, int line_counter);

/** @brief Wykonuje polecenie, lub wypisuje błąd.
 * @param[in, out] map     - mapa dróg krajowych.
 * @param[in] line         - ciąg znaków.
 * @param[in] length       - długość ciągu.
 * @param[in] line_counter - numer linii.
 */
void doOrder(Map *map, char *line, int length, int line_counter);

#endif // __PARSER_H__