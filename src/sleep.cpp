#if 0
We want to implement Linux sleep, for that - we have a timer (just 1 instance)
We can SET the timer and it will count backwards to 0 and then invoke the ISR callback.
for simplicity, assume we're single threaded - i.e, each process has only 1 thread.

you need to complete these 2 functions:

void sleep(uint32_t timeout)
{
  x.pid = getPID();
  x.timeout = timeout;
 
  push_in_order(list, x);
  
  if(list.front().pid == getPID())
  {
    SetTimer(list.front().timeout);
  }
  
  ProcessBlock();
}

// This function is automatically called when timer reaches 0
void timerISR(void *)
{
   auto x = list.front();
   
   ProcessResume(x->pid);
   
   list.pop_front()
   
   int elapsed = x.time;
      
   for(auto it; it!=list.end();it++)
   {
        it.timeout -= elapsed;
   }

   setTimer(list.front().timeout);
}

*bonus point : what ISR stands for ?

Available API's (Feel free to suggest more OR ask about anything)

void SetTimer(uint32_t timeout); // Like a system-call, we can call it from process context and it will make the time count down to 0.
unsigned int GetTimer(void); // return current timer value. If we called at t=0 SetTimer(10), at t=1 it will return 9.

void ProcessResume(uint32_t pid);
void ProcessBlock(); // called from process context. After this - a process is blocked i.e, OS scheduler does not give CPU time

uint32_t getPID(void); // from process context, return its PID.

Simple Case 
Let's assume that only 1 thread needs to sleep at any given time. Threads do not sleep in parallel

General Case
Multiple threads can sleep at any given time.
Do not use STD::* containers, if you wish to use some container needs to implement it.

	Example:

	t=0 pid=A sleep 10    wakeup 10      (A,10)
	t=2 pid=B sleep 12     wakeup 14     (A,8) , (B,4)
	t=4 pid=C sleep 2       wakeup 6     (A,6) , (B,4)    =>  (C,2),(A,4),(B,4)
                                                            t=6, wakeup => 
	t=7 pid=D sleep 6       wakeup 13    (3,A),(B,4)      =>  (3,A),(D,3),(B,1)
#endif
