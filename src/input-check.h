/** @file
 * Interfejs funkcji pomocniczych sprawdzających poprawność danych wejściowych
 *
 * @author Franciszek Roszkiewicz <fr406319@stdents.mimuw.edu.pl>
 * @date 29.04.2019
 */

#ifndef __INPUT_CHECK_H__
#define __INPUT_CHECK_H__

#include <stdbool.h>

#include "cities.h"
#include "map.h"

/** @brief Sprawdza czy nazwa miasta zawiera poprawne znaki.
 * @param[in] city_name   - wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli napis jest poprawny.
 * Wartość @p false, jeśli w napisie są niepoprawne znaki.
 */
bool checkName(const char *city_name);

/** @brief Sprawdza czy na mapie znajduje się miasto o podanej nazwie.
 * Szuka na mapie miasta i jeśli istnieje, zwraca indeks tego miasta.
 * @param[in] map             - wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city_name       - wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in, out] city_index - wskaźnik na wartość indeksu miasta.
 * @return Wartość @p true, jeśli miasto istnieje.
 * Wartość @p false, jeśli miasto nie istnieje.
 */
bool findCity(Map *map, const char *city_name, int *city_index);

/** @brief Sprawdza, czy istnieje droga między dwoma różnymi miastami.
 * @param[in] city1 - wskaźnik na strukturę przechowującą miasto;
 * @param[in] city2 - wskaźnik na strukturę przechowującą miasto.
 * @return Wartość @p true, jeśli istnieje droga pomiędzy miastami.
 * Wartość @p false, jeśli miasta nie były połączone drogą.
 */
bool checkRoad(City *city1, City *city2);

/** @brief Zwraca mniejszą z dwóch liczb.
 * @param[in] number1 - liczba;
 * @param[in] number2 - liczba.
 * @return Liczba mniejsza z number1, number2.
 */
int min(int number1, int number2);

/** @brief Sprawdza, czy istnieje droga o podanym numerze.
 * @param[in] map              - wskaźnik na stukturę przechowującą mapę dróg;
 * @param[in] route_id         - numer drogi krajowej;
 * @param[in, out] route_index - wzkaźnik na numer indeksu drogi krajowej.
 * @return Wartość @p true, jeśli droga już istnieje.
 * Wartość @p false, jeśli droga jeszcze nie powstała.
 */ 
bool findRoute(Map *map, unsigned route_id, int *route_index);

/** @brief Sprawdza, czy miasto o podanej nazwie jest na danej drodze krajowej.
 * @param[in] route     - wskaźnik na strukturę przechowującą drogę krajową;
 * @param[in] city_name - wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli podane miasto jest częścią drogi krajowej.
 * Wartość @p false, jeśli miasto nie znajduje się na drodze krajowej.
 */
bool checkCity(Route *route, const char *city_name);

/** @brief Oblicza długość liczby.
 * @param[in] number - Liczba.
 * @return Długość liczby w zapisie dziesiętnym.
 */
int getIntLength(int number);

/** @brief Oblicza długość liczby.
 * @param[in] number - Liczba.
 * @return Długość liczby w zapisie dziesiętnym.
 */
int getUnsignedLength(unsigned number);

#endif // __INPUT_CHECK_H__
