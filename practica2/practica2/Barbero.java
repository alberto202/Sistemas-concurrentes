import monitor.* ;


class Barberia extends AbstractMonitor
{ 
    private Condition clienteEnSilla;
    private Condition clientesDisponible;
    private Condition barbero;   
        
    private int nClientes;

    public Barberia(int nCliente)
    {
        this.nClientes = nClientes;
        
        clienteEnSilla = makeCondition();
        clientesDisponible = makeCondition();
        barbero = makeCondition();

    }    

    // invocado por los clientes para cortarse el pelo
    public void  cortarPelo()
    { 
        enter();
        System.out.println("El "+Thread.currentThread().getName()+" en espera");
        barbero.signal();
        clientesDisponible.await();    //Añadimos el cliente a la cola de espera      
        System.out.println("El "+Thread.currentThread().getName()+" esperando a que la silla esté libre");   
        clienteEnSilla.await();         //El cliente espera a que la silla esté libre
        System.out.println("El "+Thread.currentThread().getName()+" se está pelando");   
        //if(barbero.isEmpty()){   //Si no hay clientes en la silla despierto al barbero y el barbero está dormido
        
        //}
        
        leave();
    }

    // invocado por el barbero para esperar (si procede) a un nuevo cliente y sentarlo para el corte
    public void siguienteCliente()
    {  
        enter();
        //System.out.println(clientesDisponible.count());
        if(clientesDisponible.isEmpty()){   //Si no hay nadie, duermo al barbero
            System.out.println("No hay clientes disponibles");            
            barbero.await();                       
        }

        clientesDisponible.signal();   
        leave();
    }

    //  invocado por el barbero para indicar que ha terminado de cortar el pelo
    public void finCliente()
    { 
        enter();
        clienteEnSilla.signal();         //La silla está disponible     
        leave();
    }
}

class Cliente implements Runnable
{

    public Thread thr;
    public Barberia barberia;
    int numero;

    public Cliente( int i, Barberia b)
    { 
        thr = new Thread(this,"Cliente "+i);
        this.barberia = b;
        int numero = i;
    }

    public void run(){
        while(true) {
            barberia.cortarPelo();      //el cliente espera (si procede) y se corta el pelo            
            aux.dormir_max( 60000 );     // el cliente está fuera de la barberia un tiempo           
        }
    }
}

class Barbero implements Runnable
{

    public Thread thr;
    public Barberia barberia;

    public Barbero(Barberia b){
        thr = new Thread(this,"Barbero ");
        this.barberia = b;
    }
    public void run () { 
        
        while(true) {
            barberia.siguienteCliente();
            aux.dormir_max( 2500 );     // el barbero está cortando el pelo
            barberia.finCliente();
        }
    }
}


class MainBarbero
{ 
  public static void main( String[] args ) 
  { 
    if ( args.length != 1 ) 
    {  
      System.err.println("Uso: nClientes");
      return ;
    }
    int nClientes = Integer.parseInt(args[0]);
    // leer parametros, crear vectores y hebras
      Barberia barberia = new Barberia(nClientes);


      Barbero barbero = new Barbero(barberia);
      Cliente[] clientes = new Cliente[nClientes];
      
	  for(int i = 0; i < nClientes; i++) 
	    clientes[i] = new Cliente(i, barberia) ;
	  
	  // poner en marcha las hebras
      barbero.thr.start();
	  for(int i = 0; i < nClientes; i++) 
        clientes[i].thr.start();

  }
}
