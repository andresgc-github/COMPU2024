
# include <stdio.h>
# include <cstdio> 
# include <iostream>
# include <cmath>
# include <string> 

# define SIZExy 18

using namespace std;

void Leer(double x[SIZExy],string filename,int tipo);
void Escribir(double x[SIZExy],string filename);
void Reescalar(double x[SIZExy], int tipo);
void CalcularAceleracion(double a[SIZExy],double m[SIZExy],double r[SIZExy]);
void CalcularEnergia(double E[SIZExy],double m[SIZExy],double r[SIZExy],double v[SIZExy]);


int main(){

    double Ms=1.99E30;
    double c=149.6E9;
    double k=sqrt(6.67E-11*Ms/(c*c*c));

    double m[SIZExy]={0.0}; 
    double r[SIZExy]={0.0}; 
    double v[SIZExy]={0.0};
    double w[SIZExy]={0.0};
    double a[SIZExy]={0.0};
    double E[SIZExy]={0.0};
    
    string masasini="masasini";
    string posicionesini="posicionesini";
    string velocidadesini="velocidadesini";

    string posicionesR="planets_data"; 
    string velocidadesR="velocidadesR";
    string aceleracionesR="aceleracionesR";
    string energiasR="energy_data";
    
    Leer(m,masasini,2); 
    Leer(r,posicionesini,2); 
    Leer(v,velocidadesini,2);

    Reescalar(m,1); 
    Reescalar(r,2);
    Reescalar(v,3);

    CalcularAceleracion(a,m,r);

    Escribir(r,posicionesR);
    //Escribir(v,velocidadesR);
    //Escribir(a,aceleracionesR);
    
    //////////////////////////////////////////////////
    
    int i; int contador=0;  
    double t=0.0; 
    double h=0.01;

    do{
        CalcularEnergia(E,m,r,v);    
        // cout << E[17] << endl; //da segmentation fault      

        for(i=0;i<SIZExy;i++){
            r[i]+=h*v[i]+0.5*h*h*a[i];
            w[i]=v[i]+0.5*h*a[i];
        }
        //if(contador%1000==0){
            Escribir(E,energiasR);
            Escribir(r,posicionesR);
        //}
        CalcularAceleracion(a,m,r);
        //Escribir(a,aceleracionesR);
        for(i=0;i<SIZExy;i++){
            v[i]=w[i]+0.5*h*a[i];
        }
        //Escribir(v,velocidadesR);
        t+=h;
        contador++;
        cout << t << endl;
        // cout << E[17] << endl;
    }while(t<20000000);

    return 0;
}

// Esta función calcula y almacena en un array E[] tanto la energía de cada planeta como la energía total, en la última componente del array

void CalcularEnergia(double E[SIZExy],double m[SIZExy],double r[SIZExy],double v[SIZExy]){

    int i,j;
    double dx=0.0; // Resta de componentes X para cuerpos j-ésimo e i-ésimo, xj-xi
    double dy=0.0; // Resta de componentes Y para cuerpos j-ésimo e i-ésimo, yj-yi
    double modulo=0.0; // Módulo=|rj-ri|
    double K; // Energía cinética K=m*v*v/2
    double U; // Energía potencial V=mi*mj/|rj-ri| (G=1)

    E[17]=0.0;
    for(i=0;i<SIZExy;i+=2){ // Hay tantas iteraciones como cuerpos en i, y como cuerpos-1 en j
        K=0.5*m[i]*v[i]*v[i]; 
        U=0.0; 
        for(j=0;j<SIZExy;j+=2){ // Uso += por limpieza
            if(j!=i){
                    dx=r[j]-r[i]; // Resta de componentes X para cuerpos j-ésimo e i-ésimo, xj-xi
                    dy=r[j+1]-r[i+1]; // Resta de componentes Y para cuerpos j-ésimo e i-ésimo, yj-yi
                    modulo=sqrt(dx*dx+dy*dy); // Módulo=|rj-ri|
                    U+=m[i]*m[j]/modulo; //                    
            } else continue;
        }
        E[i]=K+U;
        E[17]+=E[i]; // La energía total del sistema solar se almacena en la última componente del vector E[]
    }

    return;
}

// Esta función calcula la aceleración de todos los cuerpos en función de sus posiciones (Newton)
void CalcularAceleracion(double a[SIZExy],double m[SIZExy],double r[SIZExy]){
    
    int i,j;
    double dx=0.0;
    double dy=0.0;
    double modulo=0.0;
    
    for(i=0;i<SIZExy;i+=2){ // Hay tantas iteraciones como cuerpos en i, y como cuerpos-1 en j
        a[i]=0.0; a[i+1]=0.0; // Tengo que reiniciar esto para la siguiente iteración
        for(j=0;j<SIZExy;j+=2){ // Uso += por limpieza
            if(j!=i){
                    dx=r[j]-r[i]; // Resta de componentes X para cuerpos j-ésimo e i-ésimo, xj-xi
                    dy=r[j+1]-r[i+1]; // Resta de componentes Y para cuerpos j-ésimo e i-ésimo, yj-yi
                    modulo=sqrt(dx*dx+dy*dy); // Módulo=|rj-ri|
                    a[i]+=m[j]*dx/(modulo*modulo*modulo); // ax=sum(j!=i)mj xj-xi/|rj-ri|^3
                    a[i+1]+=m[j]*dy/(modulo*modulo*modulo); // ay=sum(j!=i)mj yj-yi/|rj-ri|^3
            } else continue;
        }
    }
    
    return;
}

// Esta función reescala la "columna" 1 de cualquier array (x[][1]) segun el tipo de reescalado que se elija
void Reescalar(double x[SIZExy],int tipo){

    double Ms=1.99E30; //Cambio de unidades: m'=m/Ms
    double c=149.6E9; //Cambio de unidades de las posiciones: r'=r/c
    double k=sqrt(6.67E-11*Ms/(c*c*c)); //Cambio de unidades temporales: t'=kt

    int i;

    if(tipo==1){ //Tipo 1= masas. Pasa de kg a masas solares
        for(i=0;i<SIZExy;i++){
            x[i]=x[i]/Ms;
        }
    } else if(tipo==2){ //Tipo 2= posiciones. Pasa de m (¡no kilómetros!) a distancias Tierra-Sol
        for(i=0;i<SIZExy;i++){
            x[i]=x[i]/c;
        }
    } else if(tipo==3){ //Tipo 3= velocidades. Pasa de m/s (¡no km/s!) a distancias Tierra-Sol entre tiempos reescalados
        for(i=0;i<SIZExy;i++){
            x[i]=x[i]*1.0/(c*k);
        }
    } else {return;} // ¿¿¿??? Me he inventado la sintaxis, pero funciona. Es redundante, though.
    

    return; // Es redundante pero lo meto por un comentario que hizo mi profesor de programación
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