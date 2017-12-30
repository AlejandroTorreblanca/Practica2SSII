#ifndef BaseHechos_h
#define BaseHechos_h

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <queue>
#include<algorithm>
#include <string.h>
#include <string>
#include <sstream>
#include <stack>

#define MAX_BH 20

using namespace std;
typedef pair<string,string> parS;

class BaseHechos {
private:
    BaseHechos();
    static BaseHechos* unica_instancia;
	int nHechosIni,nHechos;
	parS baseHechos[MAX_BH];
    parS baseHechosIni[MAX_BH];
    char* nombreFichero;


public:

    static BaseHechos *getInstance();
    void inicializar(char* nombre);
    int funcionSeparadora(string linea, string parametros[10],char c);
    void leerBH();
    void incluirHecho(parS par);
    void incluirHechoIni(parS par);
    int buscarBaseHechos(string parametros[10], string busqueda);
    bool buscarBaseHechosIni(string busqueda);
    string to_string(int n);
};

#endif




