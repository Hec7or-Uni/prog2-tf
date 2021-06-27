# include <iostream>
# include <fstream>
# include <time.h>
# include <stdlib.h>
# include "maxsolape.hpp"

using namespace std;

const string nombreFichero = "tfb.txt";
const string var = "Grafica";

int main(int numArg, char* args[]) {
    if ((var == args[1]) || (numArg == 3) || (numArg == 5)) {
        tpInter indinters[N]; 
        if ((numArg == 2)) {  // HACE LA GRAFICA DE COSTES
            system("gnuplot -e \"set terminal gif; set style data lines; plot 'tfb.txt' \" > tfb.gif");
        } else if (numArg == 3) {     // ALGORITMO DE FUERZA BRUTA
            int ni = atoi(args[1]);     // Numero de intervalos inicial
            int nf = atoi(args[2]);     // Numero de intervalos final
            ofstream datos(nombreFichero);
            if (datos.is_open()) {
                for (ni; ni <= nf; ni++) {
                    clock_t TICKS = 0;
                    double inters[N][2];
                    crearvind(inters, indinters, ni);
                    clock_t TICKS1 = clock();   // Primera medida del tiempo
                    maxSolFBruta(inters, ni);
                    clock_t TICKS2 = clock();   // Segunda medida del tiempo
                    TICKS += (TICKS2 - TICKS1); // Tiempo en TICKS
                    datos << ni << "\t" << double(1000000/CLOCKS_PER_SEC * double(TICKS)) << endl;
                }
            }
            datos.close();
        } else {    // ALGORITMO DE DIVIDE Y VENCERAS
            int ni = atoi(args[1]);     // Numero de intervalos inicial
            int nf = atoi(args[2]);     // Numero de intervalos final
            int p = atoi(args[3]);
            int f = atoi(args[4]);
            if ((p >= 0) && (p <= 100) && (f >= 0) && (f <= 100)) {
                double inters[N][2];
                ofstream datos(nombreFichero);
                if (datos.is_open()) {
                    // LOS VALORES DE P Y F LOS HE DEJADO FIJADOS EN 0 Y NI PARA LA REALIZACION DE LAS GRAFICAS
                    cout << "AVISO: p y f han sido fijados a '0' y 'ni' respectivamente para la realizacion de los costes." << endl;
                    p = 0;
                    for (ni; ni <= nf; ni++) {
                        f = ni;   
                        clock_t TICKS = 0;
                        crearvind(inters, indinters, ni);
                        mergesortIndInters(indinters, 0, ni);
                        clock_t TICKS1 = clock();   // Primera medida del tiempo
                        maxSolDyV(indinters, p, f);
                        clock_t TICKS2 = clock();   // Segunda medida del tiempo
                        TICKS += (TICKS2 - TICKS1); // Tiempo en TICKS
                        datos << ni << "\t" << double(1000000/CLOCKS_PER_SEC * double(TICKS)) << endl;
                    }
                }
                datos.close();
            }
        }
    } else {
        cerr << "ERROR > Ha habido un error al introducir los parametros:" << endl;
        cerr << "~Algoritmo FB  ->  ./costeLab <Num Inters Ini> <Num Inters Fin>" << endl;   // 3 Argumentos
        cerr << "~Algoritmo DyV ->  ./costeLab <Num Inters Ini> <Num Inters Fin> <Inicio Solapes> <Final Solapes>"  << endl;    // 5 Argumentos
        cerr << "    Inicio Solape = Primer intervalo desde el que queremos calcular el solape." << endl;
        cerr << "    Final Solape  = último intervalo con el que queremos calcular el solape."   << endl;
        cerr << "~Gráfica       -> ./costeLab <Grafica> " << endl << endl;   // 2 argumentos   
    }
    return 0;
}
