#ifndef BaseConocimientos_h
#define BaseConocimientos_h

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
    BaseConocimientos(void);
    static BaseConocimientos* unica_instancia;
    int nRestric, nAtributosTipoNU;
    int nCondiciones[MAX_PRIO];
    int reglasMarcadas[MAX_PRIO];
    Terna listaCondiciones[MAX_PRIO][MAX_BH];
    parS listaConsecuencias[MAX_PRIO];
    int prioridades[MAX_PRIO];
    string AtributosTipoNU[MAX_ATRIBUTOS];
    string dominio;


public:
    static BaseConocimientos *getInstance();
    void inicializar(string AtributosNU[MAX_ATRIBUTOS], int n, int prioridades[MAX_PRIO],char* nombreBC);
    int funcionSeparadora(string linea, string parametros[10],char c);
    void leerBC(char* nombre );
    bool comprobarCondicion(Terna t);
    int comprobarReglas(parI parametros[10]);
    bool comprobarTipoNum(string str);
    int buscarReglas(int parametros[10], string solucion);
    bool comprobarCondicionIni(string str);
    string reconstruirSolucion(string solucion);
    parS getConsecuencia(int r);
    string getDominioAplicacion();
};



#endif





