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
#include"BaseConocimientos.h"

#define MAX_ATRIBUTOS 20
#define MAX_PRIO 100
#define MAX_BH 20

using namespace std;
typedef pair<string,string> parS;
typedef pair<int,int> parI;
priority_queue<parI, deque<parI>, less<parI> > colaConflictos;


//////////////////////////////////////////////////////////////
////////////        VARIABLES GLOBALES        ////////////////
//////////////////////////////////////////////////////////////

string AtributosTipoNU[MAX_ATRIBUTOS];
int prioridades[MAX_PRIO];
string objetivo;
int nAtributos, nAtributosTipoNU;
ofstream salida1;
ofstream salida2;
BaseHechos bh;
BaseConocimientos bc;


//////////////////////////////////////////////////////////////
////////////     FUNCIONES DEL PROGRAMA       ////////////////
//////////////////////////////////////////////////////////////

/**
Funcion para procesar una linea de texto, retorna el n�mero de palabras que contiene el string que se encuentran
separadas por el caracter c. Las palabras se retornan en el array parametros[].
**/
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

/**
Funcion que procesa el fichero de configuracion.
**/
void leerfC(char* nombre )
{
    char cadena[128];
    string parametros [3];
    int nPrio; int k=0;
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
            AtributosTipoNU[k]=parametros[0];
            k++;
        }
        //cout <<"3 "<< str  << endl;
    }
    nAtributosTipoNU=k;
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
        //cout << aux <<" ";
    }
}

/**
A�adimos a la lista de conflictos las reglas que se pueden usar ya que se cumplen sus condiciones.
**/
void incluirConflictos()
{
    parI parametros [20];
    int n=bc.comprobarReglas(parametros);
    for (int i=0;i<n;i++)
    {
         colaConflictos.push(parametros[i]);
    }
}

/**
Retorna true si el objetivo se encuentra en la base de hechos, en otro caso retorna false.
**/
bool objetivoAlcanzado()
{
    string parametros[10];
    int n=bh.buscarBaseHechos(parametros,objetivo);
    if(n!=0)
        return true;
    return false;
}

/**
Se decide cual de las reglas que se encuentran en la lista de conflictos se va a utilizar, se tiene en cuenta la prioridad.
Se retorna el numero de la regla que se debe aplicar.
**/
int resolver()
{
    queue<parI> colaAux;
    parI p=colaConflictos.top();
    colaConflictos.pop();
   // cout <<" "<<endl;
   // cout << "*sacamos "<< p.second+1<<endl;
    if(!colaConflictos.empty())
    {
        parI aux=colaConflictos.top();
        if(p.first==aux.first)
        {
            colaConflictos.pop();
           // cout << "**sacamos "<< aux.second+1<<endl;
            if(!colaConflictos.empty())
            {
                while(p.first==aux.first && !colaConflictos.empty())
                {
                    if(aux.second<p.second)
                    {
                        colaAux.push(p);
                   //     cout <<aux.second+1<< " < "<< p.second+1<<endl;
                   //     cout << "-metemos auxi "<< p.second+1<<endl;
                        p=aux;
                    }
                    else
                    {
                        colaAux.push(aux);
                  //      cout <<aux.second+1<< " >= "<< p.second+1<<endl;
                  //      cout << "-----metemos auxi "<< aux.second+1<<endl;
                    }
                    aux=colaConflictos.top();
                    colaConflictos.pop();
                  //  cout << "***sacamos "<< aux.second+1<<endl;
                }
                if(aux.first<p.first)
                {
                    colaAux.push(aux);
                  //  cout <<aux.first<< " < "<< p.first<<endl;
                 //   cout << "*--metemos auxi "<< aux.second+1<<endl;
                }
                else if(aux.first>p.first)
                {
                    colaAux.push(p);
                   //  cout <<aux.first<< " > "<< p.first<<endl;
                   // cout << "**--metemos auxi "<< p.second+1<<endl;
                    p=aux;
                }
                else if(aux.first==p.first)
                {
                     //cout <<aux.first<< " = "<< p.first<<endl;
                    if(aux.second<p.second)
                    {
                        colaAux.push(p);
                        //cout <<aux.second+1<< " < "<< p.second+1<<endl;
                       // cout << "***-metemos auxi "<< p.second+1<<endl;
                        p=aux;
                    }
                    else
                    {
                        colaAux.push(aux);
                        //cout <<aux.second+1<< " >= "<< p.second+1<<endl;
                       // cout << "****-metemos auxi "<< aux.second+1<<endl;
                    }
                }

                while(!colaAux.empty())
                {
                    aux=colaAux.front();
                    colaConflictos.push(aux);
                    //cout << "---metemos desde auxi "<< aux.second+1<<endl;
                    colaAux.pop();
                }
            }
            else
            {
                if(aux.second<p.second)
                {
                    colaConflictos.push(p);
                    //cout << "----metemos "<< p.second+1<<endl;
                    p=aux;
                }
            }
        }
    }
   // cout << "Escogida: "<< p.second+1<<endl;
    return p.second;
}

/**
Retorna true si se alcanza el objetivo en alguna regla, false en otro caso.
**/
bool encadenamientoHaciaDelante()
{
    salida1<<" "<<endl;
    salida1<< "El razonamiento seguido al intentar obtener una solucion es el siguiente:"<<endl;
    incluirConflictos();
    while(!colaConflictos.empty())
    {
        int r=resolver();;
        parS hecho=listaConsecuencias[r];
        bh.incluirHecho(hecho);
        incluirConflictos();
    }
    if(objetivoAlcanzado())
    {
        return true;
    }
    return false;
}


//////////////////////////////////////////////////////////////
////////////        PROGRAMA PRINCIPAL        ////////////////
//////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    //Comprobamos que el numero de argumentos es correcto.
    if (argc != 4) {
        cout << "- error: "  << endl;
        cout << "Argumentos incorrectos" << endl;
        cout << "Forma de uso: ficheroBC ficheroConfiguracion  ficheroBH" << endl;
        return -1;
    }

    string aux=argv[3];
    string str1="Salida1"+aux+".txt";
    salida1.open(str1.c_str(),ios::app);
    string str2="Salida2"+aux+".txt";
    salida2.open(str2.c_str(),ios::app);

    leerfC(argv[2]);
    bh=BaseHechos::getInstance();
    bh.inicializar(argv[3]);
    bc=BaseConocimientos::getInstance();
    bc.inicializar(AtributosTipoNU, nAtributosTipoNU,argv[3]);
    bh.leerBH();

    if (encadenamientoHaciaDelante(bc))
    {   //Si encontramos una soluci�n para el problema.
        string parametros[10];
        salida1 << "Objetivo: " << objetivo << endl;
        salida2 << "Objetivo: " << objetivo << endl;
        int n=bh.buscarBaseHechos(parametros,objetivo);
        if (n>1)
        {
            string aux=" ";
            salida2<< "Solucion multiple: ";
            for(int i=0; i<n-1; i++)
            {
                salida2 << parametros[i] <<", ";
                if(aux.compare(parametros[i])!=0)
                    reconstruirSolucion(parametros[i]);
                aux=parametros[i];
            }
            salida2 << parametros[n-1] <<endl;
            if(aux.compare(parametros[n-1])!=0)
                    reconstruirSolucion(parametros[n-1]);
        }
        else{
            salida2<< "Solucion: " << parametros[0] << endl;
            reconstruirSolucion(parametros[0]);
        }
    }
    else
    {
        salida2<< "No se ha entontrado ninguna solucion. "<< endl;
    }

    salida1.close();
    salida2.close();
    bh.cerrarFicheros();
    bc.cerrarFicheros();

}
