#include <iostream>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"
#include <mpi.h>

using namespace std;

#define CAMARERO 10
#define LEVANTARSE 0
#define SENTARSE 1 

void Filosofo( int id, int nprocesos);
void Tenedor ( int id, int nprocesos);
void Camarero ( int id, int nprocesos);

// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int rank, size;
   
   srand(time(0));
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
   
   if( size!=11)
   {
      if( rank == 0) 
         cout<<"El numero de procesos debe ser 11" << endl << flush ;
      MPI_Finalize( ); 
      return 0; 
   }
   
   if ((rank%10) == 0)  
      Camarero(rank,size); // Los 10 son Camarero  
   else if ((rank%2) == 0)  
      Filosofo(rank,size); // Los pares son Filosofos 
   else 
      Tenedor(rank,size);  // Los impares son Tenedores 
   
   MPI_Finalize( );
   return 0;
}  
// ---------------------------------------------------------------------

void Filosofo( int id, int nprocesos )
{
   int izq = (id+1) % nprocesos;
   int der = ((id+nprocesos)-1) % nprocesos;   

   while(1)
   {
      
      // Pensando
      cout << "Filosofo " << id << " PENSANDO" << endl << flush;

      // Pensando
      MPI_Ssend( &id, 1, MPI_INT, CAMARERO, SENTARSE, MPI_COMM_WORLD );
      cout << "Filosofo " << id << " SENTANDOSE" << endl << flush;

  
      // Solicita tenedor izquierdo
      cout << "Filosofo "<<id<< " solicita tenedor izq ..." << izq << endl << flush;
      MPI_Ssend( &id, 1, MPI_INT, izq, 0, MPI_COMM_WORLD );
      
      // Solicita tenedor derecho
      cout <<"Filosofo "<<id<< " coge tenedor der ..." << der << endl << flush;
      MPI_Ssend( &id, 1, MPI_INT, der, 0, MPI_COMM_WORLD );
      
      cout<<"Filosofo "<<id<< " COMIENDO"<<endl<<flush;
      sleep((rand() % 3)+1);  //comiendo
      
      // Suelta el tenedor izquierdo
      cout <<"Filosofo "<<id<< " suelta tenedor izq ..." << izq << endl << flush;
      MPI_Ssend( &id, 1, MPI_INT, izq, 1, MPI_COMM_WORLD );
      
      // Suelta el tenedor derecho
      cout <<"Filosofo "<<id<< " suelta tenedor der ..." << der << endl << flush;
      MPI_Ssend( &id, 1, MPI_INT, der, 1, MPI_COMM_WORLD );
      
      // LEVANDANDOSE
      MPI_Ssend( &id, 1, MPI_INT, CAMARERO, LEVANTARSE, MPI_COMM_WORLD );
      cout << "Filosofo " << id << " LEVANTANDOSE" << endl << flush;
      
      // espera bloqueado durante un intervalo de tiempo aleatorio 
      // (entre una décima de segundo y un segundo)
      usleep( 1000U * (100U+(rand()%900U)) );
 }
}
// ---------------------------------------------------------------------

void Tenedor(int id, int nprocesos)
{
  MPI_Status status; 
  int Filo;
  
  while( true )
  {
    // Espera un peticion desde cualquier filosofo vecino ...
    MPI_Recv( &Filo, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );
    
    // Recibe la peticion del filosofo ...
    
    cout << "Ten. " << id << " recibe petic. de " << Filo << endl << flush;
    // Espera a que el filosofo suelte el tenedor...

    MPI_Recv( &Filo, 1, MPI_INT, Filo, 1, MPI_COMM_WORLD, &status );

    cout << "Ten. " << id << " recibe liberac. de " << Filo << endl << flush; 
  }
}

// ---------------------------------------------------------------------

void Camarero(int id, int nprocesos)
{
  int sentados = 0;
  
  while( true )
  {
    // Espera un peticion desde cualquier filosofo vecino ...
    MPI_Recv( &Filo, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );
    
    // Recibe la peticion del filosofo ...
    
    cout << "Ten. " << id << " recibe petic. de " << Filo << endl << flush;
    // Espera a que el filosofo suelte el tenedor...

    MPI_Recv( &Filo, 1, MPI_INT, Filo, 1, MPI_COMM_WORLD, &status );

    cout << "Ten. " << id << " recibe liberac. de " << Filo << endl << flush; 
  }
}

// ---------------------------------------------------------------------
