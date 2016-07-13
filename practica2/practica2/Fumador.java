import monitor.* ;

class Estanco extends AbstractMonitor
{ 
    private Condition[] fumadores;
    private Condition estanquero;
    private int ingrediente = -1;

    public Estanco(int numIngredientes)
    {
        fumadores = new Condition[numIngredientes];
        for(int i=0; i<numIngredientes; i++)
            fumadores[i] = makeCondition();
        estanquero = makeCondition();
    }    

    //invocado por cada fumador, indicando su ingrediente o numero
    public void obtenerIngrediente( int miIngrediente )
    { 
        enter();

        if(miIngrediente != ingrediente)
        {
            fumadores[miIngrediente].await();           
        }

        System.out.println("El fumador "+ingrediente+" está fumando.");
        ingrediente = -1;
        estanquero.signal();
        leave();
    }
    // invocado por el estanquero, indicando el ingrediente que pone
    public void ponerIngrediente( int i )
    {  
        enter();
            if(ingrediente == -1){
                ingrediente = i;
                System.out.println("El estanquero ha puesto el ingrediente "+ingrediente);

                fumadores[ingrediente].signal();
            }
        leave();
    }

    // invocado por el estanquero
    public void esperarRecogidaIngrediente()
    { 
        enter();


        leave();
    }
}

class Fumador implements Runnable
{
    int miIngrediente;
    public Thread thr;
    public Estanco estanco;

    public Fumador( int p_miIngrediente, Estanco e )
    { 
        miIngrediente = p_miIngrediente;
        estanco = e;
        thr = new Thread(this,"Fumador "+miIngrediente);
    }

    public void run()
    {
        while ( true )
        {
            estanco.obtenerIngrediente( miIngrediente );
            aux.dormir_max( 2000 );
        }
    }
}

class Estanquero implements Runnable
{
    Estanco estanco;
    public Thread thr;
    
    public Estanquero(Estanco e){
        estanco = e;
        thr = new Thread(this,"Estanquero ");
    }
    public void run () { 
        int ingrediente ;   
        while (true)
        {
            ingrediente = ( int ) (Math.random () * 3.0);  // 0,1 o 2
            estanco.ponerIngrediente( ingrediente );
            estanco.esperarRecogidaIngrediente() ;
        }
    }
}


class MainFumador
{ 
  public static void main( String[] args ) 
  { 
    if ( args.length != 1 ) 
    {  
      System.err.println("Uso: nfumadores");
      return ;
    }
    int contFumadores = Integer.parseInt(args[0]);
    // leer parametros, crear vectores y hebras
      Estanco estanco = new Estanco(contFumadores);
      Estanquero estanquero = new Estanquero(estanco);
      Fumador[] fumadores = new Fumador[contFumadores];
      
	  for(int i = 0; i < fumadores.length; i++) 
	    fumadores[i] = new Fumador(i, estanco) ;
	  
	  // poner en marcha las hebras
      estanquero.thr.start();
	  for(int i = 0; i < fumadores.length; i++) 
        fumadores[i].thr.start();

  }
}



