#include"BaseConocimientos.h"

BaseConocimientos* BaseConocimientos::unica_instancia = NULL;

BaseConocimientos::BaseConocimientos(void)
{
    //Constructor por defecto.
}

/**
Usamos un patron Singelton para que solo haya una unica instancia de la base de hechos.
**/
BaseConocimientos* BaseConocimientos::getInstance()
{
    if(!unica_instancia)
    {
        unica_instancia = new BaseConocimientos;
    }
    return unica_instancia;
}

void BaseConocimientos::inicializar(string AtributosNU[MAX_ATRIBUTOS], int n, int prioridades[MAX_PRIO],char* nombreBC)
{
    this->leerBC(nombreBC);
    for(int i=0;i<n;i++)
        this->AtributosTipoNU[i]= AtributosNU[i];
    this->nAtributosTipoNU=n;
    for(int i=0;i<this->nRestric;i++)
        this->prioridades[i]= prioridades[i];
    this->nAtributosTipoNU=n;

}

/**
Funcion para procesar una linea de texto, retorna el número de palabras que contiene el string que se encuentran
separadas por el caracter c. Las palabras se retornan en el array parametros[].
**/
int BaseConocimientos::funcionSeparadora(string linea, string parametros[10],char c)
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
Funcion que procesa el fichero que contiene la base de conocimientos.
**/
void BaseConocimientos::leerBC(char* nombre )
{
    string parametros [11];
    string aux [3];
    char cadena[128];

    string str, condicion, consecuncia;

    ifstream fC(nombre);
    fC.getline(cadena, 128);
    string dominioAplicacion=cadena;
    this->dominio=dominioAplicacion;
    fC.getline(cadena, 128);
    this->nRestric=atoi(cadena);
    for(int i=0; i<this->nRestric;i++)
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
        this->nCondiciones[i]=n;
        while(j<n)
        {
            //cout << "terna " << j  <<parametros[j]<< endl;
            funcionSeparadora(parametros[j],aux,' ');
            Terna t;
            t.a=aux[0];
            t.op=aux[1];
            t.b=aux[2];
            //cout << "- " << t.a <<" "<< t.op<<" "<< t.b<< endl;
            this->listaCondiciones[i][j]=t;
            j++;
        }
        funcionSeparadora(consecuncia,parametros,' ');
        parS p;
        p.first=parametros[0];
        p.second=parametros[2];
        this->listaConsecuencias[i]=p;
        //cout << "   *  " << p.first <<" "<< p.second<< endl;
    }
}


/**
Dada una condicion buscamos si, con los datos existentes en la base de hechos, se cumple y en ese caso retornamos true.
Si no se cumple retorna false.
**/
bool BaseConocimientos::comprobarCondicion(Terna t)
{
    string parametros[10];
    BaseHechos* bh=BaseHechos::getInstance();

    int n=bh->buscarBaseHechos(parametros,t.a);
   // cout << " /// n: "<<n<< " " << t.a<<endl;
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
            if(t.b.compare(parametros[i])==0)
            {
               // cout << " --+ comparamos "<< parametros[i]<< " - " <<t.b<<endl;
                return true;
            }
          //  else
              //  cout << " --- comparamos "<< parametros[i]<< " - " <<t.b<<endl;
        }
    }
    return false;
}

bool BaseConocimientos::comprobarTipoNum(string str)
{
    for(int i=0; i<this->nAtributosTipoNU; i++)
    {
        if(str.compare(this->AtributosTipoNU[i])==0)
            return true;
    }
    return false;
}

int BaseConocimientos::comprobarReglas(parI parametros[10])
{
    int reglas=0;
    for(int i=0; i<this->nRestric; i++) //Para cada una de las reglas
    {
        if(this->reglasMarcadas[i]==0)    //Si no ha sido ya marcada, lo querria decir que no se ha usado aun
        {
            int suma=0;
            bool parada=true;
            //cout<<"*** intentamos incluir conflicto " << i+1<<endl;
            while(suma<this->nCondiciones[i] && parada )  //Comprobamos que se cumple cada una de las condiciones de la regla
            {

                if(this->comprobarCondicion(this->listaCondiciones[i][suma]))
                {
                    Terna t=this->listaCondiciones[i][suma];
                    //cout << "  condicion "<< t.a<< t.op <<t.b<< " correcta"<<endl;
                    suma++;
                }
                else
                {
                    Terna t=this->listaCondiciones[i][suma]; //**********Sobra?
                    //cout << "     condicion "<< t.a<< t.op <<t.b<< " incorrecta"<<endl;
                    parada=false;
                }
            }
            if(suma==nCondiciones[i])   //Si se han cumplido todas las condiciones se añade a la lista de conflictos
            {
                this->reglasMarcadas[i]=1;
                parI par;
                par.first=prioridades[i];
                par.second=i;
                parametros[reglas]=par;
                reglas++;
            }
        }
    }
    return reglas;
}

/**

**/
int BaseConocimientos::buscarReglas(int parametros[10], string solucion)
{
    int n=0;
    for(int i=0;i<this->nRestric;i++)
    {
        if(this->reglasMarcadas[i]==1)
        {
            parS p= this->listaConsecuencias[i];
            if(solucion.compare(p.second)==0)
            {
                parametros[n]=i;
                n++;
                this->reglasMarcadas[i]++;
            }
        }
    }
    return n;
}

bool BaseConocimientos::comprobarCondicionIni(string str)
{
    if(BaseHechos::getInstance()->buscarBaseHechosIni(str))
    {
        return true;
    }

    return false;
}

/**
Reconstrimos la solucion pasada como parametro y la mostramos en los ficheros de salida.
**/
string BaseConocimientos::reconstruirSolucion(string solucion)
{
    int parametros[10];
    stack<int> pilaR;
    queue<int> colaR;

    int m=buscarReglas(parametros, solucion);
    for(int i=0;i<m;i++)
    {
        pilaR.push(parametros[i]);
        colaR.push(parametros[i]);
    }

    while(!colaR.empty())
    {
        int restriccion=colaR.front();
        colaR.pop();

        for(int i=0;i<this->nCondiciones[restriccion];i++)
        {
            Terna t=this->listaCondiciones[restriccion][i];
            if(!comprobarCondicionIni(t.b))
            {
                m=buscarReglas(parametros, t.b);
                for(int i=0;i<m;i++)
                {
                    pilaR.push(parametros[i]);
                    colaR.push(parametros[i]);
                }
            }
        }
    }
    string str="";
    while(!pilaR.empty())
    {
        int r=pilaR.top();
        pilaR.pop();
        ostringstream str1;
        str1 << r+1;
        string aux = str1.str();

        str=str+ "-Se aplica la regla R" + aux + ":\n";
        str=str+ "    Si ";
        for(int i=0; i<this->nCondiciones[r]-1;i++)
        {
           str=str+ this->listaCondiciones[r][i].a+ " " + this->listaCondiciones[r][i].op + " " + this->listaCondiciones[r][i].b + " y ";
        }
        str=str+ this->listaCondiciones[r][nCondiciones[r]-1].a + " " + this->listaCondiciones[r][nCondiciones[r]-1].op + " " + this->listaCondiciones[r][nCondiciones[r]-1].b + " entonces ";
        str=str+ this->listaConsecuencias[r].first + " = " + this->listaConsecuencias[r].second;
        str=str+ "\n-Como consecuencia se añade el hecho: \n    " + this->listaConsecuencias[r].first + " = " + this->listaConsecuencias[r].second + "\n\n";
        BaseHechos::getInstance()->incluirHechoIni(this->listaConsecuencias[r]);
    }
    return str;
}

parS BaseConocimientos::getConsecuencia(int r)
{
    return this->listaConsecuencias[r];
}

string BaseConocimientos::getDominioAplicacion()
{
    return this->dominio;
}











