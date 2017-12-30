#include"BaseHechos.h"

BaseHechos* BaseHechos::unica_instancia = NULL;

BaseHechos::BaseHechos(void)
{
    //Constructor por defecto.
}


/**
Usamos un patron Singelton para que solo haya una unica instancia de la base de hechos.
**/
BaseHechos* BaseHechos::getInstance()
{
    if(!unica_instancia)
    {
        unica_instancia = new BaseHechos;
    }
    return unica_instancia;
}

void BaseHechos::inicializar(char* nombre)
{
    this->nombreFichero=nombre;
    string aux=this->nombreFichero;
    string str1="Salida1"+aux;

    leerBH();
}

/**
Funcion para procesar una linea de texto, retorna el número de palabras que contiene el string que se encuentran
separadas por el caracter c. Las palabras se retornan en el array parametros[].
**/
int BaseHechos::funcionSeparadora(string linea, string parametros[10],char c)
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
Funcion que procesa el fichero que contiene la base de hechos.
**/
void BaseHechos::leerBH()
{
    string parametros [3];
    char cadena[128];
    string str;

    ifstream fBH(this->nombreFichero);
    fBH.getline(cadena, 128);
    this->nHechos=atoi(cadena);
    this->nHechosIni=nHechos;
    //salida1 <<"Base de hechos inicial:"<< endl;
    for(int i=0; i<nHechos;i++)
    {
        fBH.getline(cadena, 128);
        str=cadena;
        //salida1 <<"    -"<<str<< endl;
        funcionSeparadora(str,parametros,' ');
        parS aux;
        aux.first=parametros[0];
        aux.second=parametros[2];
        //cout << "***** Lectura "<< aux.first<<endl;
        this->baseHechos[i]=aux;
        this->baseHechosIni[i]=aux;
        //cout <<"* "<< baseHechos[i].first  << endl;
    }

}
/**
Añade un hecho a la base de hechos actual.
**/
 void BaseHechos::incluirHecho(parS par)
 {
    parS p;
    p.first=par.first;
    p.second=par.second;
    this->baseHechos[this->nHechos]=p;
    this->nHechos++;
 }

  void BaseHechos::incluirHechoIni(parS par)
  {
    parS p;
    p.first=par.first;
    p.second=par.second;
    this->baseHechosIni[this->nHechosIni]=p;
    this->nHechosIni++;
  }

 /**
Retorna 0 si no se encuentra el string introducido en la base de hechos,
si lo encuentra retorna el numero de veces que se repite en la baseHechos[].
**/
int BaseHechos::buscarBaseHechos(string parametros[10], string busqueda)
{
    int suma=0;
    //cout << "Buscamos en la base la palabra: "<< busqueda<<endl;
    for (int i=0; i<this->nHechos; i++)
    {
        parS p=this->baseHechos[i];
       // cout << "comparamos: "<< busqueda<<" y "<<p.first<<endl;
        if(busqueda.compare(p.first)==0)
        {
            parametros[suma]=p.second;
            suma++;
        }
    }
     //cout << "Numero de veces encontrada: "<< suma<<endl;
    return suma;
}

/**
Retorna true si el string introducido se encuentra en la base de hechos inicial, en otro caso retorna false.
**/
bool BaseHechos::buscarBaseHechosIni(string busqueda)
{
    for (int i=0; i<this->nHechosIni; i++)
    {
        parS p=this->baseHechosIni[i];
        if(busqueda==p.second)
            return true;
    }
    return false;
}

string BaseHechos::to_string(int n)
{
    string s="";
    if(n==0)
    {
        for(int i=0; i<this->nHechos; i++)
            s=s+"    -"+this->baseHechos[i].first+ " = " + this->baseHechos[i].second +"\n";
    }
    else
    {
       for(int i=0; i<this->nHechosIni; i++)
            s=s+"    -"+this->baseHechosIni[i].first+ " = " + this->baseHechosIni[i].second +"\n";
    }
    return s;
}




