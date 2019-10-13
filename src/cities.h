/** @file
 * Interfejs klasy przechowującej miasta, drogi i drogi krajowe.
 *
 * @author Franciszek Roszkiewicz <fr406319@students.mimuw.edu.pl>
 * @date 29.04.2019
 */

#ifndef __CITIES_H__
#define __CITIES_H__

#include <stdbool.h>
#include "map.h"

/**
 * Struktura przechowująca odcinek drogi krajowej.
 */
typedef struct Route {
    unsigned route_id;     ///< numer drogi krajowej
    const char *city_name; ///< wskaźnik na napis reprezentujący nazwę miasta
    unsigned length;       ///< długość odcinka drogi krajowej
    int built_year;        ///< rok budowy lub ostatniego remontu
    struct Route *next;    ///< wskaźnik na kolejny odcinek drogi krajowej
} Route;

/**
 * Struktura przechwująca drogę.
 */
typedef struct Road {
    const char *city_name; ///< wskaźnik na napis reprezentujący nazwę miasta
    unsigned length;       ///< dugość odcinka drogi krajowej
    int built_year;        ///< rok budowy lub ostatniego remontu
    struct Road *next;     ///< wskaźnik na kolejną drogę
} Road;

/**
 * Struktura przechowująca miasto
 */
typedef struct City {
    const char *city_name; ///< wskażnik na napis reprezentujący nazwę miasta
    struct Road *next;     ///< wskaźnik na drogę wychodzącą z miasta
} City;

/** @brief Usuwa z pamięci odcinki między miastami.
 * @param[in] road - wskaźnik na strukturę przechowującą odcinek drogi.
 */
void deleteRoad(Road *road);

/** @brief Usuwa z pamięci miasto.
 * @param[in] city - wskaźnik na strukturę przechowującą miasto.
 */
void deleteNode(City *city);

/** @brief Usuwa z pamięci drogę krajową.
 * @param[in] route - wskaźnik na strukturę przechowującą drogę krajową.
 */
void deleteRoute(Route *route);

/** @brief Dodaje miasto o podanej nazwie do mapy dróg.
 * @param[in, out] map  - wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city_name - wskaźnik na napis reprezentujący nazwę miasta.
 */
bool addCity(Map *map, const char *city_name);

/** @brief Dodaje do miasta odcinek drogi.
 * Dodaje do miasta odcinek o podanym kierunku, długości i roku budowy.
 * @param[in, out] city      - miasto.
 * @param[in] city_dirextion - miasto docelowe.
 * @param[in] length         - długość odcinka.
 * @param[in] built_year     - rok budowy.
 * @return Wartość @true, jeśli operacja się powiodła. Wartość @false,
 * jeśli nie udało się zaalokować pamięci.
 */
bool createRoad(City *city, const char *city_direction,
               unsigned length, int built_year);
/** @brief Zmienia rok remontu odcinka drogi między miastami.
 * @param[in, out] city1  - miasto.
 * @param[in] city2       - nazwa miasta docelowego.
 * @param[in] repair_year - rok remontu.
 * @return Wartość @true, jeśli operacja się powiedzie. Wartość
 * @false, jeśli któryś z parametrów ma niepoprawną wartość lub
 * podany rok nie był większy od dotychczasowego.
 */
bool modifyYear(City *city1, const char *city2, int repair_year);

/** @brief Zmienia rok remontu odcinka drogi w drodze krajowej.
 * Zmienia rok remotu odcinka drogi pomiędzy podanymi miastami.
 * @param[in, out] route  - droga krajowa.
 * @param[in] city1       - miasto.
 * @param[in] city2       - miasto.
 * @param[in] repair_year - rok remontu.
 */
void changeYear(Route *route, City *city1, City *city2, int repair_year);

/** @brief Zmienia kierunek biegu drogi krajowej.
 * @param[in, out] route - droga krajowa.
 * @return Wsaźnik na stukturę reprezentującą odwróconą drogę krajową.
 */
Route* reverseRoute(Route *route);

/** @brief Kopiuje drogę krajową.
 * Alokuje pamięć i zwraca drogę krajową w parametrze @p route2.
 * @param[in] route1      - droga do skopiowania.
 * @param[in, out] route2 - skopiowana droga krajowa.
 * @return Wartość @true, jeśli operacją się powiodła. Wartość @false,
 * jeśli nie udoało się zaalokować pamięci.
 */
bool copyRoute(Route *route1, Route *route2);

/** Usuwa odcinek drogi między podanymi miastami.
 * @param[in, out] city1 - miasto, z którego usuwamy wychodzącą drogę.
 * @param[in] city2      - nazwa miasta docelowego.
 */
void removeRoadBetween(City *city1, const char *city2);

#endif // __CITIES_H__
