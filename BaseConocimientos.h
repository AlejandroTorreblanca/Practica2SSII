#ifndef BaseConocimientos_h
#define BaseConocimientos_h

#include <stdlib.h>  // Config-F.txt BC-F.txt BH-F1.txt
#include <iostream>
#include <fstream>
#include <queue>
#include<algorithm>
#include <string.h>
#include <string>
#include <sstream>
#include <stack>
#include"BaseHechos.h"

#define MAX_ATRIBUTOS 20
#define MAX_PRIO 100
#define MAX_BH 20

using namespace std;
typedef pair<string,string> parS;
typedef pair<int,int> parI;
struct Terna {
   string a;
   string op;
   string b;
};

class BaseConocimientos {
private:
    static BaseConocimientos* unica_instancia;
    int nRestric, nAtributosTipoNU;
    int nCondiciones[MAX_PRIO];
    int reglasMarcadas[MAX_PRIO];
    Terna listaCondiciones[MAX_PRIO][MAX_BH];
    parS listaConsecuencias[MAX_PRIO];
    int prioridades[MAX_PRIO];
    string AtributosTipoNU[MAX_ATRIBUTOS];
    ofstream salida1;
    ofstream salida2;

public:
    BaseConocimientos(void);
    static BaseConocimientos *getInstance();
    void inicializar(string AtributosNU[MAX_ATRIBUTOS], int n, int prioridades[MAX_PRIO], char* nombre);
    int funcionSeparadora(string linea, string parametros[10],char c);
    void leerBC(char* nombre );
    bool comprobarCondicion(Terna t);
    int comprobarReglas(parI parametros[10]);
    bool comprobarTipoNum(string str);
    int buscarReglas(int parametros[10], string solucion);
    bool comprobarCondicionIni(string str);
    void reconstruirSolucion(string solucion);
    void cerrarFicheros();




};



#endif





