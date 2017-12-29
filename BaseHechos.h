#ifndef BaseHechos_h
#define BaseHechos_h

#include <stdlib.h>  // Config-F.txt BC-F.txt BH-F1.txt
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
    static BaseHechos* unica_instancia;
	int nHechosIni,nHechos;
	parS baseHechos[MAX_BH];
    parS baseHechosIni[MAX_BH];
    char* nombreFichero;
    ofstream salida1;

public:
    BaseHechos();
    static BaseHechos *getInstance();
    void inicializar(char* nombre);
    int funcionSeparadora(string linea, string parametros[10],char c);
    void leerBH();
    void incluirHecho(parS par);
    int buscarBaseHechos(string parametros[10], string busqueda);
    bool buscarBaseHechosIni(string busqueda);
    void cerrarFicheros();


};



#endif




