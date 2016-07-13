#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      
#include <unistd.h>    
#include <stdlib.h>    

using namespace std;

const unsigned num_fumadores = 3;
sem_t puede_poner_incredientes, puede_fumar[num_fumadores];
sem_t mutex_pantalla; 
// ----------------------------------------------------------------------------
// función que simula la acción de fumar  como un retardo aleatorio de la hebra

void fumar() 
{ 
   //  inicializa la semilla aleatoria  (solo la primera vez)
   static bool primera_vez = true ;
   if ( primera_vez )
   {   primera_vez = false ;
      srand( time(NULL) );    
   }
   
   // calcular un numero aleatorio de milisegundos (entre 1/10 y 2 segundos)
   const unsigned miliseg = 100U + (rand() % 1900U) ; 

   // retraso bloqueado durante 'miliseg' milisegundos
   usleep( 1000U*miliseg ); 
}

void* estanquero(void *)
{

    while(true)
    {
        unsigned k = rand() % 3;
        sem_wait(&puede_poner_incredientes);        
        /*
        if(k == 1){ cout << "Estanquero:........ Poniendo tabaco para Fumador "<<k<<endl; }
        else if(k == 2) {cout << "Estanquero:........ Poniendo cerillas para Fumador "<<k<<endl; }
        else if(k == 3) {cout << "Estanquero:........ Poniendo papel para Fumador "<<k<<endl; }
        */

        switch (k)   
        {
            case 0:
                cout << "Estanquero:........ Poniendo tabaco para Fumador "<<k<<endl;
                break;
            case 1:
                cout << "Estanquero:........ Poniendo cerillas para Fumador "<<k<<endl;
                break;
            case 2:
                cout << "Estanquero:........ Poniendo papel para Fumador "<<k<<endl;
                break;
            default:
                break;
        }


        sem_post(&puede_fumar[k]);                
    }
    
    return NULL;
}

void* fumador(void* i)
{
    while(true){
        unsigned long per_fumador = ( (unsigned long) i);

        sem_wait(&puede_fumar[per_fumador]);
        
        cout << "El fumador " << per_fumador << " puede fumar" << endl;
        fumar();

        sem_post(&puede_poner_incredientes);    
    }
    return NULL;
}


int main()
{
    pthread_t hebra_estanquero, hebra_fumadores[num_fumadores];
    
    sem_init( &puede_poner_incredientes, 0, 1 );          // semaforo para estanquero: inicializado a 1.
    sem_init(&mutex_pantalla,0,1);
    for( unsigned long i = 0 ; i < num_fumadores ; i++ )
        sem_init( &puede_fumar[i], 0, 0);                // inicialmente ningun fumador puede fumar.

    pthread_create( &(hebra_estanquero), NULL, estanquero, NULL );
    for( unsigned long i = 0 ; i < num_fumadores ; i++ )
       pthread_create( &(hebra_fumadores[i]), NULL, fumador, (void *)i );
    
    for(unsigned i=0; i < num_fumadores; i++)
        pthread_join(hebra_fumadores[i], NULL);

  return 0 ;
}
