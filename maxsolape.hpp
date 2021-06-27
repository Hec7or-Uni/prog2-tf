/****************************************************************************\
 * Curso de Programación 2. Trabajo obligatorio
 * Autor: Héctor Toral Pallás
 * Ultima revisión: 18 de Junio de 2020
 * Resumen: Fichero de interfaz «maxsolape.cpp» que define dos
 *          funciónes denominadas «maxSolFBruta» y «maxSolDyV» que, 
 *          dada una serie de intervalos definidos en los reales, 
 *          devuelve la pareja de intevalos cuyo solape es máximo.
 * Codificación de caracteres original de este fichero: UTF-8 sin BOM
\****************************************************************************/

#include <iostream>
#include <algorithm>    // std::max
#include <ctime>

using namespace std;

const int N = 100000;       // Maximo numero de intervalos
const double minini = 0;    // Minimo valor de inicio para los intervalos
const double maxfin = 100;  // Maximo valor de fin para los intervalos

struct tpSolape{
    int interA, interB; // Indice de los intervalos
    double solape;      // Solape de los intervalos
};

struct tpInter{
    int ind;            // Indice del intervalo en la matriz inicial
    double ini, fin;    // Inicio y fin del intervalo
};

// Cada fila de inters representa un intervalo. La primera columna
// es el inicio del intervalo, y la segunda el final. Por ejemplo:
//    double inters[N][2] = {
//        {1.5, 8.0},
//        {0.0, 4.5},
//        {2.0, 4.0},
//        {1.0, 6.0},
//        {3.5, 7.0}
//    };
// tiene cinco intervalos, el primero empieza en 1.5 y termina en 8.0.
// maxSolFBruta devuelve un registro tpSolape en el que el campo solape
// es el maximo solape entre parejas de los n primeros intervalos de inters,
// y los campos interA e interB son los indices de dichos intervalos.
// Para la matriz inters de ejemplo, el resultado es solape=4.5, interA=0,
// interB=3
// (los valores de interA e interB pueden estar intercambiados, es decir,
// el resultado para el ejemplo anterior también puede ser solape=4.5,
// interA=3, interB=0).

/* Pre:  1 <= n ∧ ∀ α ∈ [0,n-1].(inters[α][0] <= inters[α][1])
 * Post: maxSolDyV(inters, n) = tpS.solape ∧ 0 <= tpS.interA ∧ tpS.interB < n ∧
 *                              n = 1 -> tpS.solape = 0 ∧ tpS.interA = 0 ∧ tpS.interB = 0 ∧
 *                              n > 1 -> tpS.solape = ∀ (α ∈ [0,n-1] ∧ β ∈ [0,n-1]).((Min(inters[α][1], inters[β][1]) - Max(inters[α][0], inters[β][0])) ∧ tpS.interA = α ∧ tpS.interB = β)
 */
tpSolape maxSolFBruta(double inters[N][2], int n);

// Crea un vector de tpInter con los n primeros intervalos de inters.
// Por ejemplo para la matrix inters de la funcion anterior y n=5, los
// valores de indinters seran:
// [{ind: 0, ini: 1.5, fin: 8.0},
//  {ind: 1, ini: 0.0, fin: 4.5},
//  {ind: 2, ini: 2.0, fin: 4.0},
//  {ind: 3, ini: 1.0, fin: 6.0},
//  {ind: 4, ini: 3.5, fin: 7.0}]

/* Pre:  1 <= n < N
 * Post: ∀ α ∈ [0,n-1].(indinters[α].ind = α ∧ indinters[α].ini = inters[α][0] ∧ indinters[α].fin = inters[α][1])
 */
void crearvind(double inters[N][2], tpInter indinters[N], int n);

// Ordena con el algoritmo mergesort los intervalos de indinters
// comprendidos entre p y f de acuerdo al valor de inicio de los intervalos
// (orden creciente).
// Por ejemplo, para el vector de la funcion anterior, p=0 y f=4, el vector
// ordenado sera:
// [{ind: 1, ini: 0.0, fin: 4.5},
//  {ind: 3, ini: 1.0, fin: 6.0},
//  {ind: 0, ini: 1.5, fin: 8.0},
//  {ind: 2, ini: 2.0, fin: 4.0},
//  {ind: 4, ini: 3.5, fin: 7.0}]

/* Pre:  0 <= p < f < N ∧ indinters = indintersOriginal
 * Post: ∀ α ∈ [p,f-1].(indinters[α].ini <= indinters[α+1].ini) ∧
 *       ∀ α ∈ [p,f].((Num β ∈ [p,f].indinters[α] = indinters[β]) = (Num β ∈ [p,f].indinters[α] = indinters[β]))
 */
void mergesortIndInters(tpInter indinters[N], int p, int f);

// Dado un vector indinters, utiliza la tecnica de Divide y Venceras para
// devolver el maximo solape entre parejas de intervalos comprendidos
// entre p y f. Por ejemplo, para el vector del procedimiento anterior,
// el resultado es solape=4.5, interA=0, interB=3

/* Pre:  0 <= p < f < N ∧ ∀ α ∈ [0,n-1].(indinters[α].ini <= indinters[α+1].ini)
 * Post: maxSolDyV(indinters, p, f) = tpS.solape ∧ 0 <= tpS.interA ∧ tpS.interB < n ∧
 *                                    n = 1 -> tpS.solape = 0 ∧ tpS.interA = 0 ∧ tpS.interB = 0 ∧
 *                                    n > 1 -> tpS.solape = ∀ (α ∈ [0,n-1] ∧ β ∈ [0,n-1]).((Min(indinters[α].fin, indinters[β].fin) - Max(indinters[α].ini, indinters[β].ini)) ∧ tpS.interA = α ∧ tpS.interB = β)
 */
tpSolape maxSolDyV(tpInter indinters[N], int p, int f);