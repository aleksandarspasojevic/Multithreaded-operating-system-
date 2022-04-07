//pretpostavljeni memorijski model: huge

#include <dos.h>

#include "PCB.h"
#include "timer.h"
#include "SCHEDULER.h"
#include "Idle.h"
#include "KSem.h"


unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina
volatile int Timer::counter = 20;
volatile int Timer::contextOnDemand = 0;

//pomocne promenljive za prekid tajmera
unsigned tsp;
unsigned tss;
unsigned tbp;


void tick();

Timer::Timer(){
	Timer::inic();
}


Timer::~Timer(){
	Timer::restore();
	delete PCB::idleThread;
}

void interrupt timer(){	// prekidna rutina
	if (!Timer::contextOnDemand){
		KernelSem::updateWaitingThreads();
		Timer::counter--;
	}
	if (Timer::counter == 0 || Timer::contextOnDemand) {
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;


//		if(PCB::running->state != DONE && PCB::running->state != BLOCKED)
//			Scheduler::put((PCB*) PCB::running);

		if(PCB::running->state == RUNNING){
			PCB::running->state = READY;              //nit koju ubacujemo u SCHEDULER je markirana kao spremna
			Scheduler::put((PCB*) PCB::running);
		}

		PCB::running = Scheduler::get();

		if(PCB::running != 0){
			PCB::running->state = RUNNING;            //odabrana nit iz SCHEDULERa se markira kao RUNNING
		}else{                                        //SCHEDULER nema spremnih niti za izvrsavanje
			PCB::setIdle();                           //running pokazuje na pcb idle niti
		}

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		Timer::counter = PCB::running->timeSlice;

		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp, tbp
		}
	}

	// poziv stare prekidne rutine koja se
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida
	if(!Timer::contextOnDemand){
		tick();
		asm int 60h;
	}

	Timer::contextOnDemand = 0;
}




// postavlja novu prekidnu rutinu
void Timer::inic(){
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

// vraca staru prekidnu rutinu
void Timer::restore(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}









