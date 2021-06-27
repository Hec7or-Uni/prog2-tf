/****************************************************************************\
 * Curso de Programación 2. Trabajo obligatorio
 * Autor: Héctor Toral Pallás
 * Ultima revisión: 16 de Junio de 2020
 * Resumen: Fichero de implementación «maxsolape.cpp» que define dos
 *          funciónes denominadas «maxSolFBruta» y «maxSolDyV» que, 
 *          dada una serie de intervalos definidos en los reales, 
 *          devuelve la pareja de intevalos cuyo solape es máximo.
 * Codificación de caracteres original de este fichero: UTF-8 sin BOM
\****************************************************************************/
#include <iostream>
#include <stdlib.h>     // srand()
#include <time.h>       // time()
#include <iomanip>      // setprecition()
#include <cassert>      // assert()
#include <algorithm>    // std::max
#include "maxsolape.hpp"

using namespace std;

tpSolape maxSolFBruta(double inters[N][2], int n) {
    tpSolape maximo;
    maximo.solape = 0;
    maximo.interA = 0;
    maximo.interB = 1;
    
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < n; j++) {
            if (i != j) {
                double solape = min(inters[i][1], inters[j][1]) - max(inters[i][0], inters[j][0]);
                if (solape > maximo.solape) {
                    maximo.solape = solape;
                    maximo.interA = i;
                    maximo.interB = j;
                }
            }
        }
    }
    return maximo;
}

void crearMints(double inters[N][2], int n) {
    int i = 0;
    time_t semilla = time(nullptr);
    srand(semilla);
    while (i < n) {
        double inicio = (maxfin - minini) * ((double)rand() / (double)RAND_MAX) + minini;
        double fin    = (maxfin - minini) * ((double)rand() / (double)RAND_MAX) + minini;
        if ((inicio != fin) && (minini <= (inicio && fin) <= maxfin)) {
            if (inicio < fin) {
                inters[i][0] = inicio;
                inters[i][1] = fin;
            } else {
                inters[i][0] = fin;
                inters[i][1] = inicio;
            }
            i++;
        }
    }
}

void crearvind(double inters[N][2], tpInter indinters[N], int n) {
    crearMints(inters, n);
    for (int i = 0; i < n; i++) {
        indinters[i].ind = i;
        indinters[i].ini = inters[i][0];
        indinters[i].fin = inters[i][1];
    }
}

void mergeIndInters(tpInter indinters[N], const int p, const int mid, const int f) {
    tpInter* temp = new tpInter[N];
    int merge_Pos = 0;
    int izq_Pos   = p;
    int dcha_Pos  = mid + 1;

    // MEZCLO LOS DOS LADOS HASTA QUE ALGÚN LADO TERMINE
    while ((izq_Pos <= mid) && (dcha_Pos <= f)) {
        if (indinters[izq_Pos].ini < indinters[dcha_Pos].ini) {
            temp[merge_Pos++] = indinters[izq_Pos++];
        } else {
            temp[merge_Pos++] = indinters[dcha_Pos++];
        }
    }
    // TERMINO LOS DE LA PARTE IZQ
    while (izq_Pos <= mid) {
        temp[merge_Pos++] = indinters[izq_Pos++];
    }
    // TERMINO LOS DE LA PARTE DERECHA
    while (dcha_Pos <= f) {
        temp[merge_Pos++] = indinters[dcha_Pos++];
    }
    // LOS GUARDO TODOS EN LA ESTRUCTURA
    for (merge_Pos = p; merge_Pos <= f; merge_Pos++) {
        indinters[merge_Pos] = temp[merge_Pos-p];
    }
    delete temp;
}

void mergesortIndInters(tpInter indinters[N], int p, int f) {
	if(p < f){
		int mid = (p + f) / 2;
		mergesortIndInters(indinters, p, mid);       // Ordenamos los intervalos de la izquierda.
		mergesortIndInters(indinters, mid + 1, f);   // Ordenamos los intervalos de la derecha.
		mergeIndInters(indinters, p, mid, f);        // Unimos los intervalos ordenados.
	}
}

tpSolape maxSolDyV(tpInter indinters[N], int p, int f) {
    tpSolape maximo;
    if (p == f-1) {     // CASO BASE: Cuando solo tiene 2 intervalos
        maximo.interA = indinters[p].ind;
        maximo.interB = indinters[f].ind;
        maximo.solape = min(indinters[p].fin, indinters[f].fin) - max(indinters[p].ini, indinters[f].ini);
        if (maximo.solape < 0) {
            maximo.solape = 0;
        }
        return maximo;
    } else {
        int mid = (p + f) / 2;
        // INICIO: SOLAPE MAXIMO IZQ && DCHA
        maximo = maxSolDyV(indinters, p, mid);      // Calculo el máximo solape en la mitad izq
        if (mid + 1 < f) {                              // Si hay más de 1 elemento en la parte derecha calculo el máximo solape de la derecha:
            tpSolape posibleMax;
            posibleMax = maxSolDyV(indinters, mid+1, f);
            if (posibleMax.solape > maximo.solape) {        // Si el solape de la derecha es mayor que el de la izquierda:
                maximo = posibleMax;                        // Solape_Izq = Solape_Dcha
            }
        }
        // FIN: SOLAPE MAXIMO IZQ && DCHA
        // INICIO: SOLAPE MAXIMO IZQ CON LOS DE DERECHA
        double maximoSec = maximo.solape;       // Guardo el valor solape [MAX(IZQ, DCHA)] en maximoSec | Pongo Sec de sección izquierda o derecha
        int indice = p;
        for (int i = p + 1; i <= mid; i++) {                  // Calcula el indice.fin mayor de la sección izquierda
            if (indinters[i].fin > indinters[indice].fin) {     // (El que termina más lejos en la sección derecha)
                indice = i;
            }
        }
        for (int i = mid + 1; i <= f; i++) {    // Comparo el intervalo con el indice.fin mayor con todos los de la derecha
            double solape = min(indinters[indice].fin, indinters[i].fin) - max(indinters[indice].ini, indinters[i].ini);
            if (solape > maximo.solape) {       // Si el nuevo solape es mayor se cambia
                maximo.interB = indinters[i].ind;
                maximo.solape = solape;
            }
        }
        if (maximoSec < maximo.solape) {
            maximo.interA = indinters[indice].ind;
        }
        // FIN: SOLAPE MAXIMO IZQ CON LOS DE DERECHA
        return maximo;
    }
}