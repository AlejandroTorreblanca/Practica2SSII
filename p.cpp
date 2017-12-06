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
typedef pair<string,string> parS;
typedef pair<int,int> parI;
priority_queue<parI, deque<parI>, less<parI> > colaConflictos;

struct Terna {
   string a;
   string op;
   string b;
};

//////////////////////////////////////////////////////////////
////////////        VARIABLES GLOBALES        ////////////////
//////////////////////////////////////////////////////////////

string AtributosTipoNU[MAX_ATRIBUTOS];
parS baseHechos[MAX_BH];
int nCondiciones[MAX_PRIO];
int reglasMarcadas[MAX_PRIO];
Terna listaCondiciones[MAX_PRIO][MAX_BH];
parS listaConsecuencias[MAX_PRIO];
int prioridades[MAX_PRIO];
string objetivo;
int nRestric, nHechos, nAtributos, nAtributosTipoNU;
ofstream salida1;
ofstream salida2;

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
    nHechos=atoi(cadena);
    salida1 <<"Base de hechos inicial:"<< endl;
    for(int i=0; i<nHechos;i++)
    {
        fBH.getline(cadena, 128);
        str=cadena;
        salida1 <<"    -"<<str<< endl;
        funcionSeparadora(str,parametros,' ');
        parS aux;
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
    string dominioAplicacion=cadena;
    salida1 << dominioAplicacion << endl;
    salida2 << dominioAplicacion << endl;
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
        //cout << "++++" <<condicion<< "  --  "<<consecuncia << endl;

        int n=funcionSeparadora(condicion,parametros,'y');
        int j=0;
        nCondiciones[i]=n;
        while(j<n)
        {
            //cout << "terna " << j  <<parametros[j]<< endl;
            funcionSeparadora(parametros[j],aux,' ');
            Terna t;
            t.a=aux[0];
            t.op=aux[1];
            t.b=aux[2];
            //cout << "- " << t.a <<" "<< t.op<<" "<< t.b<< endl;
            listaCondiciones[i][j]=t;
            j++;
        }
        funcionSeparadora(consecuncia,parametros,' ');
        parS p;
        p.first=parametros[0];
        p.second=parametros[2];
        listaConsecuencias[i]=p;
        //cout << "   *  " << p.first <<" "<< p.second<< endl;
    }
}

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
    salida1<< "Atributo objetivo: " << objetivo <<endl;
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
Retorna 0 si no se encuentra el string introducido,
si lo encuentra retorna el numero de veces que esta en la baseHechos[].
**/
int buscarBaseHechos(string parametros[10], string busqueda)
{
    int suma=0;
    //cout << "Buscamos en la base la palabra: "<< busqueda<<endl;
    for (int i=0; i<nHechos; i++)
    {
        parS p=baseHechos[i];
        //cout << "comparamos: "<< busqueda<<" y "<<p.first<<endl;
        if(busqueda==p.first)
        {
            parametros[suma]=p.second;
            suma++;
        }
    }
     //cout << "Numero de veces encontrada: "<< suma<<endl;
    return suma;
}

bool comprobarTipoNum(string str)
{
    for(int i=0; i<nAtributosTipoNU; i++)
    {
        if(str.compare(AtributosTipoNU[i])==0)
            return true;
    }
    return false;
}

bool comprobarCondicion(Terna t)
{
    string parametros[10];
    int n=buscarBaseHechos(parametros,t.a);
    for(int i=0; i<n; i++)
    {
        if(comprobarTipoNum(t.a))
        {
            if(t.op=="=")
            {
                if(atoi(t.b.c_str())==atoi(parametros[i].c_str()))
                    return true;
            }
            else if(t.op==">=")
            {
                if(atoi(parametros[i].c_str())>=atoi(t.b.c_str()))
                    return true;
            }
            else if(t.op=="<=")
            {
                if(atoi(parametros[i].c_str())<=atoi(t.b.c_str()))
                    return true;
            }
            else if(t.op=="<")
            {
                if(atoi(parametros[i].c_str())<atoi(t.b.c_str()))
                    return true;
            }
            else if(t.op==">")
            {
                if(atoi(parametros[i].c_str())>atoi(t.b.c_str()))
                    return true;
            }
        }
        else
        {
            if(t.b==parametros[i])
                return true;
        }
    }
    return false;
}

void incluirConflictos()
{
    for(int i=0; i<nRestric; i++)
    {
        if(reglasMarcadas[i]==0)
        {
            int suma=0;
            bool parada=true;
            //cout<<"*** intentamos incluir conflicto " << i+1<<endl;
            while(suma<nCondiciones[i] && parada )
            {

                if(comprobarCondicion(listaCondiciones[i][suma]))
                {
                    Terna t=listaCondiciones[i][suma];
                   // cout << "condicion "<< t.a<< t.op <<t.b<< " correcta"<<endl;
                    suma++;
                }
                else
                {
                    Terna t=listaCondiciones[i][suma];
                   // cout << "condicion "<< t.a<< t.op <<t.b<< " incorrecta"<<endl;
                    parada=false;
                }
            }
            if(suma==nCondiciones[i])
            {
                salida2 << "Incluimos el conflicto: "<< i+1 << " prioridad " <<prioridades[i]<<endl;
                reglasMarcadas[i]=1;
                parI par;
                par.first=prioridades[i];
                par.second=i;
                colaConflictos.push(par);

            }
        }
    }
}

bool objetivoAlcanzado()
{
    string parametros[10];
    int n=buscarBaseHechos(parametros,objetivo);
    if(n!=0)
        return true;
    return false;
}

int resolver() //Se eliminan algunos que no deberian����
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
                     cout <<aux.first<< " = "<< p.first<<endl;
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
                    cout << "----metemos "<< p.second+1<<endl;
                    p=aux;
                }
            }
        }
    }
   // cout << "Escogida: "<< p.second+1<<endl;
    return p.second;
}

bool encadenamientoHaciaDelante()
{
    salida1<<" "<<endl;
    salida1<< "El razonamiento seguido al intentar obtener una solucion es el siguiente:"<<endl;
    incluirConflictos();
    while(!colaConflictos.empty())
    {
        int r=resolver();
        salida1<< "    Se aplica la regla: R" << r+1 <<endl;
        parS hecho=listaConsecuencias[r];
        baseHechos[nHechos]=hecho;
        salida1 << "    Se a�ade el hecho: " << hecho.first<<"="<<hecho.second <<endl;
        nHechos++;
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

     if (argc != 4) {
        cout << "- error: "  << endl;
        cout << "Argumentos incorrectos" << endl;
        cout << "Forma de uso: ficheroBC ficheroConfiguracion  ficheroBH" << endl;
        return -1;
    }

    salida1.open("Salida1.txt",ios::trunc);
    salida2.open("Salida2.txt",ios::trunc);

    leerBC(argv[1]);
    leerfC(argv[2]);
    leerBH(argv[3]);

    memset(reglasMarcadas, 0, sizeof(reglasMarcadas));

    if (encadenamientoHaciaDelante())
    {
        string parametros[10];
        salida2 << "Objetivo: " << objetivo << endl;
        int n=buscarBaseHechos(parametros,objetivo);
        if (n>1)
        {
            salida2<< "Solucion multiple: ";
            cout<< "Solucion multiple: ";
            for(int i=0; i<n-1; i++)
            {
                salida2 << parametros[i] <<", ";
                cout << parametros[i] <<", ";
            }
            salida2 << parametros[n-1] <<endl;
            cout << parametros[n-1] <<endl;
        }
        else{
            salida2<< "Solucion: " << parametros[0] << endl;
            cout<< "Solucion: " << parametros[0] << endl;
        }


    }
    else
         cout << "Sin solucion." << endl;

    salida1.close();
    salida2.close();

}
