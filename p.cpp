#include <stdlib.h>  // Funcion exit
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>
using namespace std;

void leerBH(char* nombre )
{
    ifstream fBH(nombre);
    char cadena[128];
    fBH.getline(cadena, 128);
    //cout << cadena  << endl;
    fBH.getline(cadena, 128,'=');
    cout << cadena  << endl;
    fBH.getline(cadena, 128,'=');
    cout << cadena  << endl;
//    for (unsigned int i=0; i<linea.length(); i++)
//    {
//
//    }
}

void leerBC(char* nombre )
{
    ifstream fBC(nombre);

}

void leerfC(char* nombre )
{
    ifstream fC(nombre);

}

int main(int argc, char **argv)
{

     if (argc != 4) {
        cout << "- error: "  << endl;
        cout << "Argumentos incorrectos" << endl;
        cout << "Forma de uso: ficheroConfiguracion ficheroBC ficheroBH" << endl;
        return -1;
    }

    leerfC(argv[1]);
    leerBC(argv[2]);
    leerBH(argv[3]);











}
