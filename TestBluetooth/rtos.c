#ifndef BOOTLOADER

#include <avr/io.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <math.h>
#include <avr/sleep.h>

#include "rtos.h"

#include "avr_compiler.h"
#include "wdt_driver.h"


//-----------------------------------------------------------------------------------------------------------
// DEFINES

//-----------------------------------------------------------------------------------------------------------
// RTOS ERROR CODES

#define ERR__getSema            0x11      //argument passed to function is out of range in __getSema
#define ERR__releaseSema        0x12      //argument passed to function is out of range in __releaseSema
#define ERR__releaseSemaTN      0x13      //non-owner tried to release semaphore in __releaseSema
#define ERR__set_rtr            0x14      //invalid task number passed to function
#define ERR__clear_rtr          0x15      //invalid task number passed to function
#define ERR__get_priority       0x16      //invalid task number passed to function
#define ERR__set_priority       0x17      //invalid task number passed to function
#define ERR__set_priorityP      0x18      //invalid priority number passed to function


//-----------------------------------------------------------------------------------------------------------
// defines for status byte in task info struct

#define RTR_ON                  0x80      // mask bit for rtr status of task,  0=stopped, 1=run
#define RTR_OFF                 0x00      // used to set rtr initially off
#define SEMA_BITS               0x0F      // mask bits 0-3 in status for semaphore number
                                          // 0 = none wanted, 1-15 = semaphore wanted
//-----------------------------------------------------------------------------------------------------------
// GLOBALS

//-----------------------------------------------------------------------------------------------------------
// myTasks[] array of task control blocks stores info about each task

struct ts {
	uint8_t *goto_label;                    // store goto label address for resume of task
	volatile uint8_t status;                // store status data for a task, rtr/semaphore
	uint8_t sch_count;                      // how many times waiting in the scheduler
	volatile uint8_t ticks;                 // store wait time for the tasks
} myTasks[nTasks];

struct ts *currentTaskPtr = myTasks; //points to the current task struct, initialize with address of the task structure array 'myTasks'
uint8_t currentTaskNum;	// the number of the running task

// keep track of the owner of the semaphores
#if (SEMAPHORES_ON)
uint8_t semaOwner[nSemas];
#endif

/*-----------------------------------------------------------------------------------------------------------
 store entry address and startup status for each task in program memory
 supports 16 tasks, expand as needed
-----------------------------------------------------------------------------------------------------------*/

const struct PROGMEM ta {
	uint16_t addr;                                  // address of task (function)
	uint8_t init;                                   // initial rtr and priority
} taskAddress[nTasks] = {

	#ifdef __Task0
	{ (uint16_t)__Task0, (__Task0_init | ((__Task0_priority & 7) << 4)) },
	#endif
	#ifdef __Task1
	{ (uint16_t)__Task1, (__Task1_init | ((__Task1_priority & 7) << 4)) },
	#endif
	#ifdef __Task2
	{ (uint16_t)__Task2, (__Task2_init | ((__Task2_priority & 7) << 4)) },
	#endif
	#ifdef __Task3
	{ (uint16_t)__Task3, (__Task3_init | ((__Task3_priority & 7) << 4)) },
	#endif
	#ifdef __Task4
	{ (uint16_t)__Task4, (__Task4_init | ((__Task4_priority & 7) << 4)) },
	#endif
	#ifdef __Task5
	{ (uint16_t)__Task5, (__Task5_init | ((__Task5_priority & 7) << 4)) },
	#endif
	#ifdef __Task6
	{ (uint16_t)__Task6, (__Task6_init | ((__Task6_priority & 7) << 4)) },
	#endif
	#ifdef __Task7
	{ (uint16_t)__Task7, (__Task7_init | ((__Task7_priority & 7) << 4)) },
	#endif
	#ifdef __Task8
	{ (uint16_t)__Task8, (__Task8_init | ((__Task8_priority & 7) << 4)) },
	#endif
	#ifdef __Task9
	{ (uint16_t)__Task9, (__Task9_init | ((__Task9_priority & 7) << 4)) },
	#endif
	#ifdef __Task10
	{ (uint16_t)__Task10,(__Task10_init | ((__Task10_priority & 7) << 4)) },
	#endif
	#ifdef __Task11
	{ (uint16_t)__Task11,(__Task11_init | ((__Task11_priority & 7) << 4)) },
	#endif
	#ifdef __Task12
	{ (uint16_t)__Task12,(__Task12_init | ((__Task12_priority & 7) << 4)) },
	#endif
	#ifdef __Task13
	{ (uint16_t)__Task13,(__Task13_init | ((__Task13_priority & 7) << 4)) },
	#endif
	#ifdef __Task14
	{ (uint16_t)__Task14,(__Task14_init | ((__Task14_priority & 7) << 4)) },
	#endif
	#ifdef __Task15
	{ (uint16_t)__Task15,(__Task15_init | ((__Task15_priority & 7) << 4)) },
	#endif

};


//###########################################################################################################
// RTOS PRIVATE FUNCTIONS

//-----------------------------------------------------------------------------------------------------------
// error handling function, use to troubleshoot problems
static void __osError(uint8_t err,uint8_t bad_num){        // error code + bad number
  cli();    // no irq's while __osError does its thing
  while(1){}; //just halt here, do what you want
}

#if (SEMAPHORES_ON)
//-----------------------------------------------------------------------------------------------------------
// restore priority number of a task from program memory (original setting)
// only used by __releaseSema, so no error checking on tn done
static void __restore_priority(uint8_t tn){
  __set_priority(tn, (pgm_read_byte(&taskAddress[tn].init))>>4 & 7 );
}
#endif

//-----------------------------------------------------------------------------------------------------------
//save 'goto label address' for returning to task after os call
void __save_goto(uint8_t *gl){
	currentTaskPtr->goto_label = gl;
}

/*-----------------------------------------------------------------------------------------------------------
 increase sch_count of task (keep track of how many times a task was ready, but was not run because of a
 higher priority task that was also ready, or was waiting for a semaphore)
-----------------------------------------------------------------------------------------------------------*/

static void __inc_sch_count(uint8_t tn){
  struct ts *mt;                            // pointer to myTasks[tn] struct
  mt = &myTasks[tn];                        // get address of myTasks[tn]
  if(mt->sch_count < 0xFF){                 // only increase count if not 0xFF (prevent rollover)
      mt->sch_count++;                      // increase the sch_count of task
  }
}


/*-----------------------------------------------------------------------------------------------------------
 scheduler finds the highest priority task ready to run (if any), loops if none found
 tasks that have been waiting longest, are given priority for same priority tasks
 if a task is ready, find task address from pgm memory, then call it (goto number passed to task)
-----------------------------------------------------------------------------------------------------------*/

static void __schedule(void){
  uint8_t highestPriority;                  // keep track of highest priority task found
  uint8_t i;                                // loop counter
  uint8_t this_pri;                         // priority of myTasks[i] (current task in loop)
  uint8_t save_tn=0;                        // task number of task to run
  uint8_t this_cnt;                         // sch_count of myTasks[i] (how many times waiting)
  uint8_t save_cnt=0;                       // sch_count of task to run

  while(1){                                 // loop in scheduler forever

    //---------------------------------------------------------------------------------------------------
    // set priority higher than any task
    //---------------------------------------------------------------------------------------------------
    highestPriority=8;                      // start 1 more than possible

    //---------------------------------------------------------------------------------------------------
    // loop through all tasks to find a task to run
    //---------------------------------------------------------------------------------------------------
    for(i=0; i < nTasks; i++){              // loop through all tasks
      this_pri = myTasks[i].status >> 4;    // get status byte, move high nibble to low nibble
      this_cnt = myTasks[i].sch_count;      // get sch_count of task

      //-----------------------------------------------------------------------------------------------
      // if a task has the rtr bit set
      //-----------------------------------------------------------------------------------------------
      if(this_pri & 8){                 // if rtr bit set
        this_pri &= 7;                  // mask off rtr bit, only priority number is left

        //-------------------------------------------------------------------------------------------
        // first, find out if this is the highest priority task
        // the first rtr task will always be the highest priority (is > 8)
        //-------------------------------------------------------------------------------------------
        if(this_pri < highestPriority){ // if higher priority task

          //---------------------------------------------------------------------------------------
          // found a higher priority task, so now see if this was not the first
          // task found (highestPriority will not be 8), if was not the first, the previously
          // saved task needs its sch_count increased, since it will not run
          //---------------------------------------------------------------------------------------
          if(highestPriority!=8){       // this is not the first task found
            __inc_sch_count(save_tn);   // increase sch_count of previous task
          }
          highestPriority = this_pri;   // save this priority number as the highest found
          save_tn = i;                  // save task number of this task
          save_cnt = this_cnt;          // save sch_count of this task
        }

        //-------------------------------------------------------------------------------------------
        // was not a higher priority, so now check if it is the same priority as the highest
        // priority previously found
        //-------------------------------------------------------------------------------------------
        else if(this_pri == highestPriority){ // this must be at least the second task found ready

          //---------------------------------------------------------------------------------------
          // same priority, so now check if this task has a higher sch_count (waiting longer)
          //---------------------------------------------------------------------------------------
          if(this_cnt > save_cnt){      // if been waiting longer,
            __inc_sch_count(save_tn);   // increase sch_count of previous task
            save_tn = i;                // save the task number of this task
            save_cnt = this_cnt;        // save sch_count of this task
          }
          else{                         // have not been waiting longer than previous task
            __inc_sch_count(i);         // just increase sch_count of this task
          }
        }

        //-------------------------------------------------------------------------------------------
        // this task is a lower priority task, so just increase its sch_count
        //-------------------------------------------------------------------------------------------
        else{                           // is lower priority task
          __inc_sch_count(i);           // increase sch_count of this task
        }
    	} //if

	    //-----------------------------------------------------------------------------------------------
	    // rtr bit was not set
	    //-----------------------------------------------------------------------------------------------
	    else{                                 // is not rtr, but check if waiting for semaphore
	      if(myTasks[i].status & 0x0F){       // if low nibble not 0, then is waiting for a semaphore
	          __inc_sch_count(i);             // increase sch_count of this task
	      }
	    }
	  } //for

	  //---------------------------------------------------------------------------------------------------
	  // check if a task found, if highestPriority is not 8, a task was found
	  //---------------------------------------------------------------------------------------------------
	  if(highestPriority != 8){               // found a task to run
	    currentTaskNum = save_tn;             // save task number
	    currentTaskPtr = &myTasks[save_tn];   // save pointer to task struct
	    currentTaskPtr->sch_count=0;          // clear sch_count of task

	    // (  (void (*)(uint8_t *)) address_to_call  )(uint8_t *);
	    // read address of task stored in program memory
	    // typecast as a function pointer with no return value, and passing a uint8_t pointer
	    // then call that function, with the task goto_label as a parameter
	    (  (void (*)(uint8_t *)) pgm_read_word(&taskAddress[save_tn].addr) ) (currentTaskPtr->goto_label);
	  }

	  else{    // no tasks are ready to run, we could sleep

	  }

  } //for
} //void


//###########################################################################################################
// PUBLIC FUNCTIONS

/*-----------------------------------------------------------------------------------------------------------
 disable task for nTicks ticks, load wait value in myTasks struct
-----------------------------------------------------------------------------------------------------------*/

void __wait(uint8_t nTicks){
    currentTaskPtr->status &= ~RTR_ON;           // turn off task ready-to-run bit
    currentTaskPtr->ticks = nTicks;              // store tick value into task ticks variable
}

/*-----------------------------------------------------------------------------------------------------------
 if semaphore number is in use, suspend task and store the busy semaphore number in task status
 return 1 if sema was available, return 0 if not
 also bump up priority of semaphore owner if another higher priority task also wants that semaphore
-----------------------------------------------------------------------------------------------------------*/

#if (SEMAPHORES_ON)    
uint8_t __getSema(uint8_t n){
  if((n == 0) || (n >= nSemas)){                // make sure a valid sema number is passed
    __osError(ERR__getSema,n);                  // not a valid sema number
    return 0;                                   // keep compiler happy
  }
  else if(semaOwner[n] == currentTaskNum){             // if trying to get the same sema it already owns
		return 1;                                   // just return 1
  }
  else if(semaOwner[n] == 0xFF){                // if available
    semaOwner[n] = currentTaskNum;                   // put task number in semaOwner
    return 1;                                   // available, return 1
  }
  else{                                         // not available
    currentTaskPtr->status &= ~RTR_ON;          // suspend this task
    currentTaskPtr->status |= n;                // set sema number in this task that wanted it
    if(__get_priority(currentTaskNum) < __get_priority(semaOwner[n])){  // if this task has higher priority
        __set_priority(semaOwner[n],__get_priority(currentTaskNum)); // bump up owner priority
    }
    return 0;                                   // not available
  }
}
#endif

//-----------------------------------------------------------------------------------------------------------
// get priority number of a task

uint8_t __get_priority(uint8_t tn){
  if(tn >= nTasks){                              // make sure a valid task number
     __osError(ERR__get_priority,tn);            // error, invalid task number
     return 0;
  }
  else{
     return ((myTasks[tn].status >> 4) & 7);     // lower 3 bits in high nibble
  }
}

//-----------------------------------------------------------------------------------------------------------
// set priority number of a task

void __set_priority(uint8_t tn, uint8_t pri){
  if(tn >= nTasks){                             // make sure a valid task number
		__osError(ERR__set_priority,tn);            // error, invalid task number
  }
  else if(pri > 7){                             // make sure a valid priority
    __osError(ERR__set_priorityP,pri);          // error, invalid priority
  }
  else{
    uint8_t temp_s;                             // temp status byte  storage
    temp_s = myTasks[tn].status & 0x8F;         // clear out priority bits (bits4-6)
    pri <<= 4;                                  // move priority number up into position
    myTasks[tn].status = temp_s | pri;          // save previous status + new priority
  }
}

/*-----------------------------------------------------------------------------------------------------------
 enable task number ready to run status if a valid task number, and not waiting for a semaphore
 also check if the task tn owns a semaphore (only if semaphores enabled)
*/

void __set_rtr(uint8_t tn, uint8_t i){
  if(tn >= nTasks){                             // make sure a valid task number
		__osError(ERR__set_rtr,tn);                 // error, invalid task number
  }
  else{
    #if (SEMAPHORES_ON)
    // check all semaphores (if nSemas passed) to see if task owns any
    // osSET_SEMA_OWNER_RTR(semaowner),osENABLE_SEMA_OWNER(semaowner) will skip this check
    // because they pass 1, which will predecrement to 0,skipping this check
    while(--i){                                 // pre-decrement (check sema 1 to nSemas)
        if(semaOwner[i] == tn){                 // if task owns a semaphore,
            return;                             // return
        }
    }
    // if waiting for a semaphore, do not enable
    if(myTasks[tn].status & 0x0F){              // if waiting for a semaphore, do not enable task
        return;                                 // just return
    }
    #endif
    myTasks[tn].status |= RTR_ON;               // turn on rtr bit
    myTasks[tn].ticks = 0;                      // clear tick counter, so if task was waiting, it will
                                                // not be re-enabled when wait time is done
  }
}

/*-----------------------------------------------------------------------------------------------------------
 clear task number ready to run status
*/

void __clear_rtr(uint8_t tn){
  if(tn >= nTasks){                // make sure a valid task number
    __osError(ERR__clear_rtr,tn);  // error, invalid task number
  }
  else{
    myTasks[tn].status &= ~RTR_ON;
    myTasks[tn].ticks = 0;         // clear tick counter, so if task was waiting, it will
  }                                // not be re-enabled when wait time is done
}


/*-----------------------------------------------------------------------------------------------------------
 clear sema bits from task, then check if any other tasks have that sema number, set highest
 priority task with the sema number ready to run
*/

#if (SEMAPHORES_ON)    
void __releaseSema(uint8_t n){
  uint8_t i;                      // temp counter
  uint8_t this_pri;               // priority of task i
  uint8_t this_cnt;               // sch_count of task i
  uint8_t save_tn=0;              // highest priotiy task number found wanting semaphore
  uint8_t save_cnt=0;             // sch_count of highest priority task number foun wanting semaphore
  uint8_t highestPriority=8;      // highest priority found, any task will be higher priority than 8

  if((n==0) || (n>=nSemas)){                  // make sure a valid sema number is passed
    __osError(ERR__releaseSema,n);            // not a valid sema number
  }
  else if(semaOwner[n] != currentTaskNum){    // only sema owner can release semaphore
    __osError(ERR__releaseSemaTN,n);          // non-owner tried to release semaphore
  }
  else{
    __restore_priority(semaOwner[n]); // restore priority

    semaOwner[n] = 0xFF;                      // clear owner from sema
    for(i = 0; i < nTasks; i++){              // find other tasks wanting this semaphore
      this_pri = myTasks[i].status;           // get status byte
      if((this_pri & SEMA_BITS) == n){        // if sema number matches this sema
        this_pri = (this_pri >> 4) & 0x07;    // get priority
        this_cnt = myTasks[i].sch_count;      // get sch_count of task
        if(this_pri < highestPriority){       // if higher priority task
          highestPriority = this_pri;         // save priority number
          save_tn = i;                        // then save task number
          save_cnt = this_cnt;                // save sch_count
        }
        else if(this_pri == highestPriority){ // if same priority
          if(this_cnt > save_cnt){            // check if was waiting longer
              save_tn = i;                    // yes, then save task number
              save_cnt = this_cnt;            // and save sch_count
          }
        }
      }
    }
    if(highestPriority != 8){                 // found a task to set rtr
      myTasks[save_tn].status |= RTR_ON;      // enable the task
      myTasks[save_tn].status &= ~SEMA_BITS;  // clear sema bits in task
      semaOwner[n] = save_tn;                 // set sema owner to task number
    }
  }
}
#endif

//==========================================================================================
// System tick routine, called from an interrupt handler elsewhere

void SysTick(void){
  uint8_t i;                                  // loop counter
  for (i = 0; i < nTasks; i++){               // go through each task
    if (myTasks[i].ticks){                    // if tick value not 0 for a task
			if (--myTasks[i].ticks==0){             // decrement and check again
				myTasks[i].status |= RTR_ON;        // now is 0, so set it ready-to-run again
			}
    }
  }
}

//==========================================================================================
// Start the Scheduler. This function never returns.

void StartScheduler(void){
	//------------------------------------------------------------------------------------------
	// initialize semaOwner array to 0xFF
	// currentTaskNum is used, and will be 0 when done, semaOwner[0] will not be set (not used)
	#if (SEMAPHORES_ON)
	currentTaskNum=nSemas;
	while(--currentTaskNum){
	    semaOwner[currentTaskNum]=0xFF;
	}
	#endif

	//------------------------------------------------------------------------------------------
	// initialize the task control blocks in myTask[] from setup info compiled in Flash

	currentTaskNum=nTasks;
	do{
		currentTaskNum--;
		myTasks[currentTaskNum].status = pgm_read_byte(&taskAddress[currentTaskNum].init);
	}while(currentTaskNum);

  sei(); // turn on interrupts
  __schedule(); //start scheduler (never returns)
}

#endif