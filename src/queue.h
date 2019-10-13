/** @file
 * Interfejs klasy wykorzystującej kolejkę priorytetową.
 *
 * @author Franciszek Roszkiewicz <fr406319@students.mimuw.edu.pl>
 * @date 29.04.2019
 */

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>

#include "cities.h"
#include "map.h"

/**
 * Struktura reprezentująca kolejkę priorytetową.
 */
typedef struct Queue Queue;

/** Usuwa z pamięci kolejkę priorytetową.
 * @param[in] queue - wskaźnik na strukturę reprezentującą kolejkę priorytetową.
 */
void deleteQueue(Queue *queue);

/** Sprawdza, czy wierzcołek o podanej nazwie jest w kolejce priorytetowej.
 * @param[in] queue - kolejka priorytetowa.
 * @param[in] name  - nazwa wierzchołka.
 * @return Wartość @true, jeśli wierzchołek jest elementem kolejki.
 * Wartość @false w przeciwnym przypadku
 */
bool checkNode(Queue *queue, const char *name);

/** @brief Odwraca kolejkę priorytetową.
 * @param[in] queue - kolejka priorytetowa.
 * @return Wskaźnik na odwróconą kolejkę priorytetową.
 */
Queue* reverseQueue(Queue *queue);

/** @brief Aktualizuje długość do podanego miasta w kolejce priorytetowej.
 * Jeśli nowa długość jest mniejsza od poprzedniej lub gdy długość jest
 * taka sama to, jeśli najstarsza droga jest młodsza od poprzedniej, 
 * aktualizuje długość, rok najstarszej drogi oraz poprzednie miasto
 * w ścieżce.
 * @param[in, out] queue - kolejka priorytetowa.
 * @param[in] new_node   - odcinek drogi do miasta docelowego.
 */
void update(Queue *queue, Road *new_node);

/** @brief Dołącza do kolejki nowe miasto.
 * Dołącza w odpowiednim miejscu kolejki miasto i alokuje pamięć
 * na nowy wierzchołek kolejki priorytetowej.
 * @param[in] map        - mapa dróg.
 * @param[in, out] queue - kolejka priorytetowa.
 * @param[in] new_node   - odcinek drogi do miasta docelowego.
 * @return Wartość @true, jeśli operacja się powiedzie. Wartość @false,
 * jeśli miasto docelowe nie znajduje się na mapie dróg lub nie
 * udało się zaalokować pamięci na nowy wieżchołek.
 */
bool insert(Map *map, Queue *queue, Road *new_node);

/** @brief Tworzy nową drogę krajową na podstawie najkrótszej scieżki.
 * Analizuje kolejkę prioryterową i alokuje pamięć na drogę krajową.
 * @param[in] queue      - kolejka priorytetowa.
 * @param[in, out] route - droga krajowa.
 * @param[in] route_id   - numer drogi krajowej.
 * @return Wartość @true, jeśli powstanie droga krajowa. Wartość
 * @false, jeśli nie można jednoznacznie wyznaczyć drogi krajowej
 * lub nie udało się zaalokować pamięci.
 */
bool createNewRoute(Queue *queue, Route *route, unsigned route_id);

/** @brief Tworzy nową drogę krajową na mapie dróg.
 * Szuka najkrótszej ścieżki i tworzy drogę krajową o podanym numerze
 * między podanymi miastami.
 * @param[in, out] map - mapa dróg.
 * @param[in] route_id - numer drogi krajowej.
 * @param[in] city1    - miasto startowe.
 * @param[in] city2    - miasto docelowe.
 * @return Wartość @true, jeśli droga powstała i została dodana do mapy
 * dróg. Wartość @false, jeśli nie można jednoznacznie wyznaczyć drogi
 * krajowej lub nie udało się zaalokować pamięci.
 */
bool createRoute(Map *map, unsigned route_id, City *city1, City *city2);

/** @brief Szuka nowej drogi objazdowej.
 * Szuka najkrótszej ścieżki nie wykożystując odcinków podanych
 * w parametrze route.
 * @param[in] map         - mapa dróg krajowych.
 * @param[in] route       - droga krajowa.
 * @param[in, out] queue  - kolejka priorytetowa.
 * @param[in] destination - miasto docelowe.
 * @param[in, out] length - długość powstałej ścieżki
 * @return Wartość @true, jeśli operacja się powiodła. Wartość @false,
 * jeśli nie ma innej ścieżki do danego miasta.
 */
bool findNewRoute(Map *map, Route *route, Queue *queue,
                 City *destination, int *length);

/** @brief Szuka najstarszego odcinka w ścieżce do podanego miasta.
 * @param[in] queue - kolejka priorytetowa.
 * @param[in] name  - miasto.
 * @return Najstarszy odcinek drogi w ścieżce.
 */
int findOldest(Queue *queue, const char *name);

/** @brief Wyłuża drogę krajową do podanego miasta.
 * Szuka najkrótszą ścieżkę od końca drogi krajowej do miasta docelowego,
 * nie wykożystując odcinków w drodze krajowej i modyfikuje drogę na
 * mapie dróg.
 * @param[in, out] map - mapa dróg.
 * @param[in] route    - droga krajowa.
 * @param[in] city     - miasto docelowe.
 * @return Wartość @true, jeśli operacja się powiedzie. Wartość
 * @false, jeśli nie ma nowej ścieżki, nie można jednoznacznie wyznaczyć
 * nowej ścieżki lub nie udało się zaalokować pamięci na rozszerzenie drogi.
 */
bool extend(Map *map, Route *route, City *city);

/** @brief Uzupełnia drogę krajową między podanymi miastami.
 * Szuka najkrószej ścieżki między podanymi miastami, nie wykorzystując
 * odinków w drodze krajowej i modyfikuje ją na mapie dróg.
 * @param[in, out] map - mapa dróg.
 * @param[in] route    - droga krajowa.
 * @param[in] city1    - miasto.
 * @param[in] city2    - miasto.
 * @return Wartość @true, jeśli operacja się powiodła. Wartość @false,
 * jeśli nie ma ścieżki pomiędzy miastami, nie można jednoznacznie wyznaczyć
 * drogi lub nie udało się zaalokować pamięci.
 */
bool completeRoute(Map *map, Route *route, City *city1, City *city2);

#endif // __QUEUE_H__
