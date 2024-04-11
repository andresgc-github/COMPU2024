
// Inclusión de paquetes
# include <stdio.h>
# include <cstdio> // Para el FILE
# include <iostream>
# include <cmath> // Para el sqrt por ejemplo
# include <string> // Para usar strings o algo así
// Definiciones
# define SIZExy 18
// # define SIZEm 9 esto es poquísimo elegante, pero así arreglo mis problemas de sintaxis
// # define SIZEt 2

using namespace std;

// void Leer(double x[SIZExy][SIZEt],string filename); uso la de debajo por el script de python
void Leer(double x[SIZExy],string filename,int tipo);
void Reescalar(double x[SIZExy], int tipo);
void CalcularAceleracion(double a[SIZExy],double m[SIZExy],double r[SIZExy]);
void Escribir(double x[SIZExy],string filename);


int main(){

// BLOQUE 1: t=0

    // DECLARACIÓN DE VARIABLES
    // Este array almacena las 9 masas de los cuerpos del sistema solar, empezando por el Sol -- ANTIGUO
    double m[SIZExy]={1.0}; // El [SIZExy][SIZEt] lo tengo puesto por sintaxis de las funciones que uso debajo
    // Estos arrays son matrices 18x2, y almacena en este orden: x1, y1, x2, y2, etc. Hace esto para el instante t y para el instante t+h
    // Guarda las componentes x en los índices pares, y las componentes y en los índices impares
    double r[SIZExy]={1.0}; 
    double v[SIZExy]={1.0};
    double a[SIZExy]={1.0};
    // A continuación declaro strings con los nombres archivos de texto que uso para leer y escribir los datos y resultados
    string masasini="masasini";
    string posicionesini="posicionesini";
    string velocidadesini="velocidadesini";

    string posicionesR="planets_data";
    string velocidadesR="velocidadesR";
    string aceleracionesR="aceleracionesR";

    // LECTURA: ahora se inicializan todos los arrays, leyendo de los ficheros de datos
    Leer(m,masasini,2); // m[][1] sin reescalar
    Leer(r,posicionesini,2); // r[][1] sin reescalar
    Leer(v,velocidadesini,2); // v[][1] sin reescalar

    // REESCALADO: se reescalan los arrays con los datos con las unidades sugeridas en el problema
    Reescalar(m,1); 
    Reescalar(r,2);
    Reescalar(v,3);

    // Calculo a[][1] YA REESCALADO, así que no hace falta reescalarlo ahora
    CalcularAceleracion(a,m,r);

    // Ahora mismo tengo todos los arrays reescalados. Los valores x[][0] siguen indefinidos.

    // ESCRIBIR t=0: ahora se escriben todos los valores, reescalados, en los respectivos archivos para t=0
    Escribir(r,posicionesR);
    //Escribir(v,velocidadesR);
    //Escribir(a,aceleracionesR);
    // Ahora están escritas (reescaladas) r,v,a en el instante inicial t=0.

// BLOQUE 2: evolución temporal
    // DECLARACIÓN DE VARIABLES
    // Declaro la variable auxiliar w[][] del algoritmo de Verlet
    double w[SIZExy]={1.0};
    // Declaro el índice de planetas
    int i;
    // Declaro los reescalamientos para usarlos en el paso temporal
    double Ms=1.99e30; //Cambio de unidades: m'=m/Ms
    double c=149.6e9; //Cambio de unidades de las posiciones: r'=r/c
    double k=sqrt(6.67e-11*Ms/(c*c*c));
    // Declaro el tiempo y el paso temporal (unidad 58,1 días)
    double t=0.0;
    double h=80000.0; // Paso temporal. Con h=0.003, Mercurio tarda 500 iteraciones en dar 1 vuelta

    do{
        for(i=0;i<SIZExy;i++){
            //r[i][0]=r[i][1];
            //v[i][0]=v[i][1];
            //a[i][0]=a[i][1];
            r[i]+=h*v[i]+0.5*h*h*a[i];
        }
        for(i=0;i<SIZExy;i++){
            w[i]=v[i]+0.5*h*a[i];
            //w[i]=a[i];  
            //v[i]+=0.5*h*a[i];
            //r[i]+=h*v[i];
        }
        Escribir(r,posicionesR);
        CalcularAceleracion(a,m,r);
        //Escribir(a,aceleracionesR);
        for(i=0;i<SIZExy;i++){
            v[i]=w[i]+0.5*h*a[i];
            //v[i]+=0.5*h*(w[i]+a[i]);   
            //v[i]+=0.5*h*a[i];         
        }
        //Escribir(v,velocidadesR);
        t+=h;
        //cout << t << endl;

    }while(t<4000000*h);


    return 0;
}


// Esta función reescala la "columna" 1 de cualquier array (x[][1]) segun el tipo de reescalado que se elija
void Reescalar(double x[SIZExy],int tipo){

    double Ms=1.99e30; //Cambio de unidades: m'=m/Ms
    double c=149.6e9; //Cambio de unidades de las posiciones: r'=r/c
    double k=sqrt((6.67e-11)*Ms/(c*c*c)); //Cambio de unidades temporales: t'=kt

    int i;

    if(tipo==1){ //Tipo 1= masas. Pasa de kg a masas solares
        for(i=0;i<SIZExy;i++){
            x[i]=x[i]/(Ms*1.0);
        }
    } else if(tipo==2){ //Tipo 2= posiciones. Pasa de m (¡no kilómetros!) a distancias Tierra-Sol
        for(i=0;i<SIZExy;i++){
            x[i]=x[i]/(c*1.0);
        }
    } else if(tipo==3){ //Tipo 3= velocidades. Pasa de m/s (¡no km/s!) a distancias Tierra-Sol entre tiempos reescalados
        for(i=0;i<SIZExy;i++){
            x[i]=x[i]*1.0/(c*k);
        }
    } else {return;} // ¿¿¿??? Me he inventado la sintaxis, pero funciona. Es redundante, though.
    

    return; // Es redundante pero lo meto por un comentario que hizo mi profesor de programación
}

// Esta función calcula la aceleración de todos los cuerpos en función de sus posiciones (Newton)
void CalcularAceleracion(double a[SIZExy],double m[SIZExy],double r[SIZExy]){
    
    int i,j;
    //double ax=0.0;
    //double ay=0.0;
    double dx=0.0;
    double dy=0.0;
    double modulo=0.0;
    double radical=0.0;
    //double G=6.67E-11;

    
    for(i=0;i<SIZExy;i+=2){ // Hay tantas iteraciones como cuerpos en i, y como cuerpos-1 en j
        //ax=0.0; ay=0.0; // Tengo que reiniciar esto para la siguiente iteración
        a[i]=0.0; a[i+1]=0.0;
        for(j=0;j<SIZExy;j+=2){ // Uso += por limpieza
            if(j!=i){
                    dx=r[i]-r[j]; // Resta de componentes X para cuerpos j-ésimo e i-ésimo, xj-xi
                    dy=r[i+1]-r[j+1]; // Resta de componentes Y para cuerpos j-ésimo e i-ésimo, yj-yi
                    radical=dx*dx+dy*dy;
                    modulo=sqrt(radical); // Módulo=|rj-ri|
                    a[i]+=-(1.0*m[j]*dx)/(modulo*modulo*modulo); // ax=sum(j!=i)mj xj-xi/|rj-ri|^3
                    a[i+1]+=-(1.0*m[j]*dy)/(modulo*modulo*modulo); // ay=sum(j!=i)mj yj-yi/|rj-ri|^3
            } else continue;
        }
        //a[i]=ax; // La fórmula es a=G*sum(j!=i) rj-ri/|rj-ri|^3. Ya tengo la suma, falta multiplicar
        //a[i+1]=ay; // Así multiplico por G una sola vez y no en cada iteración
    }
    
    return;
}

void Leer(double x[SIZExy],string filename,int tipo){

    int i;
    FILE *f1;
    filename+=".txt";/*el operador += (o el propio +) sí que funciona con */
    f1=fopen(filename.c_str(),"r"); /*este .c_str() convierte el string en un array de 
    char. Esto lo hago porque el argumento de la funcion debe ser un  array de char*//*"read"*/

    if(tipo==1){ // (Masa) Esta opción lee un fichero de 1 sola línea con datos (x1 \tab y1 \tab x2 \tab y2... x0.5*SIZExy \tab y0.5*SIZExy) y los mete en a[SIZExy][1]
        for(i=0;i<SIZExy;i++){
            fscanf(f1,"%lf\t",&x[i]);
        }
        
    } else if(tipo==2){ // (Vectores 2D) Esta opción lee un fichero de N=SIZExy/2 líneas con datos (x1 \tab y1 \n x2 \tab y2 \n... x0.5*SIZExy \tab y0.5*SIZExy) y los mete en a[SIZExy][1]
        for(i=0;i<SIZExy;i+=2){
            fscanf(f1,"%lf\t%lf\n",&x[i],&x[i+1]);
        }
    }
    fclose(f1);


    return;
}

void Escribir(double x[SIZExy],string filename){

    int i;
    FILE *f1;
    filename+=".dat";/*el operador += (o el propio +) sí que funciona con */
    f1=fopen(filename.c_str(),"a"); /*este .c_str() convierte el string en un array de 
    char. Esto lo hago porque el argumento de la funcion debe ser un  array de char*/
    /*"append": el
    write reescribe el archivo, el append añade justo al final B)*/

    for(i=0;i<SIZExy;i+=2){
            fprintf(f1,"%lf,%lf\n",x[i],x[i+1]); // El lf es el double, que en fprintf se escribe así
        }
    fprintf(f1,"\n"); // XDDDDD por la sintaxis. Escribe un enter al final de cada bloque de 9 líneas, para matchear con el script de Python
    fclose(f1);
    return;
}

/*void Inicializar(double x[SIZExy][SIZEt]){

    int i,j;
    for(i=0;i<SIZExy;i++){
        for(j=0;j<SIZEt;j++){

        }


    }


    return;
}*/