import monitor.* ;
// ****************************************************************************

class BufferHoare extends AbstractMonitor
{ 
  private int      numSlots = 0    , 
                   cont     = 0    ;   
  private double[] buffer   = null ;
  private Condition lectura      = makeCondition();
  private Condition escritura    = makeCondition();

  public BufferHoare( int p_numSlots ) 
  { 
    numSlots = p_numSlots ; 
    buffer = new double[numSlots] ;
  }
  public void depositar( double valor )
  { 
    enter();
        if( cont == numSlots )
          lectura.await();              
        buffer[cont] = valor ; 
        cont++ ;      
        escritura.signal() ; 
    leave(); 
  }

  public double extraer() 
  { 
    enter();
        double valor ;
        if( cont == 0 ) 
          escritura.await() ;
        cont--; 
        valor = buffer[cont] ;
        lectura.signal();
    leave();
    return valor;
  }
}

// ****************************************************************************

class ProductorHoare implements Runnable 
{ 
  private BufferHoare bb    ;
  private int    veces , 
                 numP  ;
  public  Thread thr   ;
  
  public ProductorHoare( BufferHoare pbb, int pveces, int pnumP ) 
  { 
    bb    = pbb;
    veces = pveces;
    numP  = pnumP ;
    thr   = new Thread(this,"productor Hoare "+numP);
  }

  public void run() 
  { 
    double item = 100*numP ;

    for( int i=0 ; i < veces ; i++ )
    { 
        System.out.println(thr.getName()+", produciendo " + item);
        bb.depositar( item++ );
    }
  }
}

// ****************************************************************************

class ConsumidorHoare implements Runnable 
{ 
  private BufferHoare  bb    ;
  private int     veces ,
                  numC  ;
  public  Thread  thr   ;
  
  public ConsumidorHoare( BufferHoare pbb, int pveces, int pnumC ) 
  { 
    bb    = pbb;
    veces = pveces;
    numC  = pnumC ;
    thr   = new Thread(this,"consumidor hoare"+numC);
  }
  public void run() 
  { 
      for( int i=0 ; i<veces ; i++ )
      { 
        double item = bb.extraer ();
        System.out.println(thr.getName()+", consumiendo "+item);
      } 
  }
}

// ****************************************************************************

class MainProductorConsumidorHoare 
{ 
  public static void main( String[] args ) 
  { 
    if ( args.length != 5 ) 
    {  
      System.err.println("Uso: ncons nprod tambuf niterp niterc");
      return ;
    }

    // leer parametros, crear vectores y buffer intermedio
      ConsumidorHoare[] cons      = new ConsumidorHoare[Integer.parseInt(args[0])] ;
	  ProductorHoare[]  prod      = new ProductorHoare[Integer.parseInt(args[1])] ;
	  BufferHoare       buffer    = new BufferHoare(Integer.parseInt(args[2]));
	  int          iter_cons = Integer.parseInt(args[3]);
	  int          iter_prod = Integer.parseInt(args[4]);
	  
    if ( cons.length*iter_cons != prod.length*iter_prod )
	  { 
      System.err.println("no coinciden número de items a producir con a cosumir");
      return ;
    }    

	  // crear hebras
	  for(int i = 0; i < cons.length; i++) 
	    cons[i] = new ConsumidorHoare(buffer,iter_cons,i) ;
	  for(int i = 0; i < prod.length; i++)
	    prod[i] = new ProductorHoare(buffer,iter_prod,i) ;

	  // poner en marcha las hebras
	  for(int i = 0; i < prod.length; i++) 
      prod[i].thr.start();
	  for(int i = 0; i < cons.length; i++) 
      cons[i].thr.start();
  }
}

