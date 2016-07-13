#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>

using namespace std ;

// ---------------------------------------------------------------------
// constantes 
const unsigned 
  num_items  = 40 ,
  tam_vector = 10 ;   

int buffer[tam_vector];
unsigned  consumido = 0;       

sem_t mutex;
sem_t puedo_escribir;
sem_t puedo_leer;        

// ---------------------------------------------------------------------

unsigned producir_dato()
{
  static int contador = 0 ;
  cout << "producido: " << contador << endl << flush ;
  return contador++ ;
}
// ---------------------------------------------------------------------

void consumir_dato( int dato )
{
    cout << "dato recibido: " << dato << endl ;
}
// ---------------------------------------------------------------------

void * productor( void * )
{   
  for( unsigned i = 0 ; i < num_items ; i++ )
  { 
    
    sem_wait(&puedo_escribir);
    sem_wait(&mutex);
    if(consumido < tam_vector)
    {
        int dato = producir_dato() ;
        buffer[consumido] = dato;
        consumido++;
    }
    sem_post(&mutex);
    sem_post(&puedo_leer);

  }
  return NULL ;
}
// ---------------------------------------------------------------------

void * consumidor( void * )
{   
  for( unsigned i = 0 ; i < num_items ; i++ )
  {   
    int dato ;
    
    sem_wait(&puedo_leer);
    sem_wait(&mutex);
        consumido--;
        dato = buffer[consumido] ;
        consumir_dato( dato ) ;
    sem_post(&mutex);
    sem_post(&puedo_escribir);


  }
  return NULL ;
}
//----------------------------------------------------------------------

int main()
{
    sem_init( &mutex,          0, 1 ); // semaforo para EM: inicializado a 1
    sem_init( &puedo_escribir, 0, tam_vector ); // inicialmente se puede escribir hasta tam_vector elementos
    sem_init( &puedo_leer,     0, 0 ); // inicialmente no se puede leer

    // declaración de variables de tipo hebra
    pthread_t hebra_productor, hebra_consumidor ;

    // pone en marcha la ejecución de las hebras
    pthread_create(&hebra_productor,NULL,productor,NULL);
    pthread_create(&hebra_consumidor,NULL,consumidor,NULL);

    // esperar a que finalizen las dos hebras:
    pthread_join(hebra_productor,NULL);
    pthread_join(hebra_consumidor,NULL);

   return 0 ; 
}
