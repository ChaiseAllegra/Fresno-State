/*Chaise Allegra */


public class Csci117Lab //main class 
{ static sr1 r1 = new sr1(); 
  static sr2 r2 = new sr2();
  public static void main(String args[]) 
  {  //create and start 6 threads for 6 concurrent processes
      process num1, num2, num3, num4, num5, num6;
      System.out.println("===== Thread for process_1 created");
      System.out.println("===== Thread for process_2 created");
      System.out.println("===== Thread for process_3 created");
      System.out.println("===== Thread for process_4 created");
      System.out.println("===== Thread for process_5 created");
      System.out.println("===== Thread for process_6 created");
      num1 = new process(1, r1, r2);
      num1.start();
      num2= new process(2, r1, r2);
      num2.start();
      num3= new process(3, r1, r2);
      num3.start();
      num4 = new process(4, r1, r2);
      num4.start();
      num5 = new process(5, r1, r2);
      num5.start();
      num6 = new process(6, r1, r2);
      num6.start();
   
  }
} 
 
class sr1 //class for shared resource r1
{ private static int avail1 = 3; 
 
  //synchronized method cannot be interrupted 
  //only one thread can access at a time 
  public synchronized void acquire(int id) {
       try{
      if(avail1 == 0){
          System.out.println("Process_"+id+ " is waiting for s2");
          wait();
      }
          System.out.println("Process_"+ id+ " acquired s1");
         avail1 = avail1 -1;    
  } 
   catch(InterruptedException e){}
  }   
  //synchronized method cannot be interrupted 
  //only one thread can access at a time 
  public synchronized void release(int id) 
  { 
      System.out.println("process_"+id+" released s1");
    avail1 = avail1+1;
    notify();
  } 
}//class sr1 
 
class sr2 //class for shared resource r2
{ private static int avail2 = 2; 

  //synchronized method cannot be interrupted 
  //only one thread can access at a time 
  public synchronized void acquire(int id) {
  { 
      try{
      if(avail2 == 0){
          System.out.println("+Process_"+id+ " is waiting for s2");
          wait();
      }
          System.out.println("Process_"+ id+ " acquired s2");
         avail2 = avail2 -1;
      } 
       catch(InterruptedException e){}
  }
      
  }      
  //synchronized method cannot be interrupted 
  //only one thread can access at a time 
  public synchronized void release(int id) 
  { 
    avail2 = avail2+1;
    System.out.println("process_" +id+" released s2");
    notify();
  } 
}//class sr2 
 class TestSleepMethod1 extends Thread{  
 public void run(){  
  for(int i=1;i<5;i++){  
    try{Thread.sleep(1000);}catch(InterruptedException e){System.out.println(e);}  
    System.out.println(i);  
  }  
 }  //sleep class
 }
class process extends Thread //thread class name is "process"
{ static sr1 r1; 
  static sr2 r2;
  private int id; 

  public process(int k, sr1 r1, sr2 r2) //constructor 
  { this.r1 = r1;
    this.r2 = r2;
    this.id = k;
  } 
 
  public void run() 
  { 
        r1.acquire(id);
        r2.acquire(id);

         TestSleepMethod1 t1=new TestSleepMethod1();  
        
        System.out.printf("---- process_%s is working \n", id);
        System.out.println("Process_"+id+" is working");   
        
        r1.release(id);
        r2.release(id);
                      
  }
}//class process 

/*
OUTPUT
===== Thread for process_1 created
===== Thread for process_2 created
===== Thread for process_3 created
===== Thread for process_4 created
===== Thread for process_5 created
===== Thread for process_6 created
Process_1 acquired s1
Process_1 acquired s2
Process_2 acquired s1
Process_2 acquired s2
---- process_1 is working 
---- process_2 is working 
Process_2 is working
Process_3 acquired s1
+Process_3 is waiting for s2
Process_1 is working
process_2 released s1
process_2 released s2
Process_6 acquired s1
Process_5 is waiting for s2
Process_4 is waiting for s2
Process_6 acquired s2
---- process_6 is working 
Process_3 acquired s2
---- process_3 is working 
Process_3 is working
process_3 released s1
Process_6 is working
process_3 released s2
process_1 released s1
Process_4 acquired s1
Process_5 acquired s1
+Process_5 is waiting for s2
+Process_4 is waiting for s2
process_6 released s1
process_6 released s2
Process_5 acquired s2
---- process_5 is working 
Process_5 is working
process_5 released s1
process_5 released s2
Process_4 acquired s2
---- process_4 is working 
Process_4 is working
process_4 released s1
process_4 released s2
process_1 released s2
*/
