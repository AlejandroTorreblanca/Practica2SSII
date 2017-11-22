#include <stdlib.h>  // Config-F.txt BC-F.txt BH-F1.txt
#include <iostream>
#include <fstream>
#include <queue>
#include<algorithm>
#include <string.h>
#include <string>
#include <sstream>
#define MAX_ATRIBUTOS 20
#define MAX_PRIO 100
#define MAX_BH 20
using namespace std;

typedef pair<string,string> par;

struct Terna {
   string a;
   string op;
   string b;
};


//////////////////////////////////////////////////////////////
////////////        VARIABLES GLOBALES        ////////////////
//////////////////////////////////////////////////////////////



string tipoNUMAtributo[MAX_ATRIBUTOS];
par baseHechos[MAX_BH];
Terna listaCondiciones[MAX_PRIO][MAX_BH];
par listaConsecuencias[MAX_PRIO];
int prioridades[MAX_PRIO];
string objetivo;
int nRestric, nHechos;

//////////////////////////////////////////////////////////////
////////////     FUNCIONES DEL PROGRAMA       ////////////////
//////////////////////////////////////////////////////////////

int funcionSeparadora(string linea, string parametros[10],char c)
{
	int numParametros=0;
	string parametroactual="";
	if (linea=="");
	else
	{
	    int j=0;

	    if(linea[j]==' ')
            j++;
		for ( unsigned int i=j; i<linea.length()-1; i++)
		{

			if (linea[i]==c)
			{
				parametros[numParametros]= parametroactual;
				parametroactual="";
				numParametros++;
			}
			else parametroactual= parametroactual + linea[i];

		}

		if(linea[linea.length()-1]!=' ')
            parametroactual= parametroactual + linea[linea.length()-1];
		parametros[numParametros]= parametroactual;
	}
	numParametros++;
	return numParametros;
}

void leerBH(char* nombre )
{
    string parametros [3];
    char cadena[128];
    string str;

    ifstream fBH(nombre);
    fBH.getline(cadena, 128);
    int nHechos=atoi(cadena);
    for(int i=0; i<nHechos;i++)
    {
        fBH.getline(cadena, 128);
        str=cadena;
        funcionSeparadora(str,parametros,' ');
        par aux;
        aux.first=parametros[0];
        aux.second=parametros[2];
        baseHechos[i]=aux;
        //cout <<"* "<< baseHechos[i].first  << endl;
    }

}

void leerBC(char* nombre )
{
    string parametros [11];
    string aux [3];
    char cadena[128];

    string str, condicion, consecuncia;

    ifstream fC(nombre);
    fC.getline(cadena, 128);
    fC.getline(cadena, 128);
    nRestric=atoi(cadena);
    for(int i=0; i<nRestric;i++)
    {
        fC.getline(cadena, 128);
        str=cadena;

        size_t pos1 = str.find("Si");
        size_t pos2 = str.find("Entonces");
        condicion = str.substr (pos1+3,pos2-8);
        consecuncia = str.substr (pos2+9);
        cout << "++++" <<condicion<< "  --  "<<consecuncia << endl;

        int n=funcionSeparadora(condicion,parametros,'y');
        int j=0;
        while(j<n)
        {
            //cout << "terna " << j  <<parametros[j]<< endl;
            funcionSeparadora(parametros[j],aux,' ');
            Terna t;
            t.a=aux[0];
            t.op=aux[1];
            t.b=aux[2];
            cout << "   - " << t.a <<" "<< t.op<<" "<< t.b<< endl;
            listaCondiciones[i][j]=t;
            j++;
        }
        funcionSeparadora(consecuncia,parametros,' ');
        par p;
        p.first=parametros[0];
        p.second=parametros[2];
        listaConsecuencias[i]=p;
        cout << "   * " << p.first <<" "<< p.second<< endl;
    }
}

void leerfC(char* nombre )
{
    char cadena[128];
    string parametros [3];
    int nAtributos, nPrio; int k=0;
    string str;

    ifstream fBC(nombre);

    fBC.getline(cadena, 128);
    //cout <<"1 "<< cadena  << endl;
    fBC.getline(cadena, 128);

    nAtributos=atoi(cadena);
    //cout <<"2 "<< nAtributos  << endl;
    //Leemos atributos
    for(int i=0; i<nAtributos;i++)
    {
        fBC.getline(cadena, 128);
        str=cadena;
        funcionSeparadora(str, parametros,' ');
        if(parametros[1].compare("NU")==0)
        {
            //cout <<"4 "<< parametros[0]  << endl;
            tipoNUMAtributo[k]=parametros[0];
            k++;
        }
        //cout <<"3 "<< str  << endl;
    }
    //Leemos objetivo
    fBC.getline(cadena, 128);
    fBC.getline(cadena, 128);
    objetivo=cadena;
    //cout <<"objetivo: "<< objetivo  << endl;
    //Leemos prioridades
    fBC.getline(cadena, 128);
    fBC.getline(cadena, 128);
    nPrio=atoi(cadena);
    for(int i=0; i<nPrio;i++)
    {
        fBC.getline(cadena, 128);
        int aux=atoi(cadena);
        prioridades[i]=aux;
        cout << aux <<" ";
    }
}

//////////////////////////////////////////////////////////////
////////////        PROGRAMA PRINCIPAL        ////////////////
//////////////////////////////////////////////////////////////

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
