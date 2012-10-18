#ifndef RTOS_H
#define RTOS_H

/*-----------------------------------------------------------------------------------------------------------
 set up your tasks here, up to 16 (__Task0 to __Task15) 
 more can be added if needed - change rtos.c (taskAddress variable initialization) and defines below
-------------------------------------------------------------------------------------------------------------

// task definition template
#define __Taskn                 NameOfTaskFunction
#define __Taskn_priority        priority (0-7), 7 is highest priority
#define __Taskn_init            RTR_ON or RTR_OFF (ready to run)

-----------------------------------------------------------------------------------------------------------*/

#define __Task0                 TaskMain
#define __Task0_priority        2
#define __Task0_init            RTR_ON

#define __Task1                 TaskAcq
#define __Task1_priority        2
#define __Task1_init            RTR_OFF	// we don't want this running until TaskMain has made sure everything's okay

#define __Task2                 TaskUSB
#define __Task2_priority        2
#define __Task2_init            RTR_ON

#define __Task3                 TaskFlash
#define __Task3_priority        2
#define __Task3_init            RTR_ON

#define __Task4                 TaskWatchDog
#define __Task4_priority        2
#define __Task4_init            RTR_ON

#if BLUETOOTH
#define __Task5                 TaskBT
#define __Task5_priority        2
#define __Task5_init            RTR_ON
#endif

/*-----------------------------------------------------------------------------------------------------------
 create semaphores here, (up to 15)- there is no semaphore 0 (only 1-15)
 enum {do_not_remove,firstSema,nSemas};  do_not_remove used to prevent using first one (0)
-----------------------------------------------------------------------------------------------------------*/

#define SEMAPHORES_ON  0 // 0=off, 1=on

enum {do_not_remove,semaTX,nSemas};


// Function prototypes

void SysTick(void);

/*-----------------------------------------------------------------------------------------------------------
 setup function prototype, enumerate any defined tasks
 goto_label (gl) is passed to task, used for goto statement
 need 2 macros, so 'name' is expanded first
-----------------------------------------------------------------------------------------------------------*/

#define SETUP_TASK2(name,num)	void name(uint8_t *gl); enum{name ## NUMBER = num};
#define SETUP_TASK(name,num)	SETUP_TASK2(name,num)

/*-----------------------------------------------------------------------------------------------------------
 SETUP_TASK(taskname,num) create a function prototype and define the task number of each defined task
 this currenty supports up to 16 tasks
 nTasks will automatically be set to the number of tasks
-----------------------------------------------------------------------------------------------------------*/

#define nTasks 0						// default is no tasks, used for error checking

#ifdef __Task0              // if this task exists
SETUP_TASK(__Task0,nTasks)  // setup the task, using current nTasks as the task number
#undef nTasks               // increment nTasks
#define nTasks 1
#endif

#ifdef __Task1
SETUP_TASK(__Task1,nTasks)
#undef nTasks
#define nTasks 2
#endif

#ifdef __Task2
SETUP_TASK(__Task2,nTasks)
#undef nTasks
#define nTasks 3
#endif

#ifdef __Task3
SETUP_TASK(__Task3,nTasks)
#undef nTasks
#define nTasks 4
#endif

#ifdef __Task4
SETUP_TASK(__Task4,nTasks)
#undef nTasks
#define nTasks 5
#endif

#ifdef __Task5
SETUP_TASK(__Task5,nTasks)
#undef nTasks
#define nTasks 6
#endif

#ifdef __Task6
SETUP_TASK(__Task6,nTasks)
#undef nTasks
#define nTasks 7
#endif

#ifdef __Task7
SETUP_TASK(__Task7,nTasks)
#undef nTasks
#define nTasks 8
#endif

#ifdef __Task8
SETUP_TASK(__Task8,nTasks)
#undef nTasks
#define nTasks 9
#endif

#ifdef __Task9
SETUP_TASK(__Task9,nTasks)
#undef nTasks
#define nTasks 10
#endif

#ifdef __Task10
SETUP_TASK(__Task10,nTasks)
#undef nTasks
#define nTasks 11
#endif

#ifdef __Task11
SETUP_TASK(__Task11,nTasks)
#undef nTasks
#define nTasks 12
#endif

#ifdef __Task12
SETUP_TASK(__Task12,nTasks)
#undef nTasks
#define nTasks 13
#endif

#ifdef __Task13
SETUP_TASK(__Task13,nTasks)
#undef nTasks
#define nTasks 14
#endif

#ifdef __Task14
SETUP_TASK(__Task14,nTasks)
#undef nTasks
#define nTasks 15
#endif

#ifdef __Task15
SETUP_TASK(__Task15,nTasks)
#undef nTasks
#define nTasks 16
#endif

// add more here if needed

#if (!(nTasks))
#error No Tasks Defined in Tasks.h
#endif



/*-----------------------------------------------------------------------------------------------------------
 semaOwner[] keeps track of semaphore task owner
*/

#if (SEMAPHORES_ON)
extern uint8_t semaOwner[];
#endif

extern uint8_t thisTaskN; // the current task number

void StartScheduler(void);

//-----------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION PROTOTYPES for all os calls available to tasks

void    __save_goto     (uint8_t *gl);
uint8_t __get_priority  (uint8_t tn);
void    __set_priority  (uint8_t tn, uint8_t pri);
void    __set_rtr       (uint8_t tn, uint8_t i);
void    __clear_rtr     (uint8_t tn);
void    __wait          (uint8_t nTicks);
#if (SEMAPHORES_ON)
uint8_t __getSema       (uint8_t n);
void    __releaseSema   (uint8_t n);
#endif



//-----------------------------------------------------------------------------------------------------------
// DEFINES

/*-----------------------------------------------------------------------------------------------------------
 generate a goto label, we need 2 'layers' because __LINE__ needs expanding first to line number
-----------------------------------------------------------------------------------------------------------*/

#define MY_CONCAT2(s,L)		__CONCAT(s,L)
#define MY_CONCAT(s,L)		MY_CONCAT2(s,L)

/*-----------------------------------------------------------------------------------------------------------
 macros for running tasks, put at beginning and end of tasks (functions)
 sets up function name and task loop, goto will cause a jump to appropriate resume position in task
-----------------------------------------------------------------------------------------------------------*/

#define BEGIN_TASK(name)	void (name)(uint8_t *gl){\
                          	if (gl) goto *gl;\
                            while(1){
#define END_TASK            } }


//-----------------------------------------------------------------------------------------------------------
// OS CALL DEFINES

/*-----------------------------------------------------------------------------------------------------------
 suspend the task for "ticks" ticks
 ready to run bit will be cleared, rtos system interrupt will set it ready to run when ticks gets to 0
-----------------------------------------------------------------------------------------------------------*/

#define osWAIT(ticks)                   { __save_goto(&&MY_CONCAT(L,__LINE__));\
                                        __wait(ticks);\
                                        return; }\
                                        MY_CONCAT(L,__LINE__):

/*-----------------------------------------------------------------------------------------------------------
 wait while 'statement' is true, if true, wait a tick, then try again
-----------------------------------------------------------------------------------------------------------*/

#define osWAIT_WHILE(statement)         { __save_goto(&&MY_CONCAT(L,__LINE__));\
                                        MY_CONCAT(L,__LINE__):\
                                        if(statement) { __wait(1); return; } }

/*-----------------------------------------------------------------------------------------------------------
 wait until 'statement' is true, if not true, wait a tick, then try again
-----------------------------------------------------------------------------------------------------------*/

#define osWAIT_UNTIL(statement)         { __save_goto(&&MY_CONCAT(L,__LINE__));\
                                        MY_CONCAT(L,__LINE__):\
                                        if(!(statement)) { __wait(1); return; } }

/*-----------------------------------------------------------------------------------------------------------
 clear rtr of task, do statement, then yield, waiting for irq to set task rtr again
-----------------------------------------------------------------------------------------------------------*/

#define osWAIT_IRQ(statement)           { __clear_rtr(thisTaskN);\
                                        {statement}\
                                        __save_goto(&&MY_CONCAT(L,__LINE__));\
                                        return; }\
                                        MY_CONCAT(L,__LINE__):

/*-----------------------------------------------------------------------------------------------------------
 yield to any other higher priority tasks that are ready to run
 does not clear the ready to run bit, returns immediately if it is the highest priority task
-----------------------------------------------------------------------------------------------------------*/

#define osYIELD                         { __save_goto(&&MY_CONCAT(L,__LINE__));\
                                        return; }\
                                        MY_CONCAT(L,__LINE__):

/*-----------------------------------------------------------------------------------------------------------
 suspend the task, will only start again when another task or irq sets this task ready to run
-----------------------------------------------------------------------------------------------------------*/

#define osSUSPEND                       { __save_goto(&&MY_CONCAT(L,__LINE__));\
                                        __wait(0);\
                                        return; }\
                                        MY_CONCAT(L,__LINE__):

/*-----------------------------------------------------------------------------------------------------------
 get priority number of a task
-----------------------------------------------------------------------------------------------------------*/

#define osGET_PRIORITY(taskname)        __get_priority(taskname ## NUMBER)

/*-----------------------------------------------------------------------------------------------------------
 set priority of a task
-----------------------------------------------------------------------------------------------------------*/

#define osSET_PRIORITY(taskname,pri)    __set_priority(taskname ## NUMBER,pri)

/*-----------------------------------------------------------------------------------------------------------
 clear the ready to run status of a task, scheduler does not run (ticks is also cleared)
 taskname is the name of the task, the macro will expand to get the number
-----------------------------------------------------------------------------------------------------------*/

#define osCLEAR_RTR(taskname)           __clear_rtr(taskname ## NUMBER)
#define osDISABLE_TASK(taskname)        __clear_rtr(taskname ## NUMBER)

/*-----------------------------------------------------------------------------------------------------------
 get ownership of a semaphore, if available, otherwise suspend until it becomes available
-----------------------------------------------------------------------------------------------------------*/

#if (SEMAPHORES_ON)
#define osGET_SEMA(semaNum)             { __save_goto(&&MY_CONCAT(L,__LINE__));\
                                        if(!(__getSema(semaNum))) return;}\
                                        MY_CONCAT(L,__LINE__):
#endif

/*-----------------------------------------------------------------------------------------------------------
 release ownership of a semaphore, check if other tasks want it
-----------------------------------------------------------------------------------------------------------*/

#if (SEMAPHORES_ON)
#define osRELEASE_SEMA(semaNum)         __releaseSema(semaNum);
#endif

/*-----------------------------------------------------------------------------------------------------------
 if a task owns a semaphore, only the these two commands will enable the task
-----------------------------------------------------------------------------------------------------------*/

#if (SEMAPHORES_ON)
#define osSET_SEMA_OWNER_RTR(semaNum)   __set_rtr(semaOwner[semaNum],1)
#define osENABLE_SEMA_OWNER(semaNum)    __set_rtr(semaOwner[semaNum],1)
#endif

/*-----------------------------------------------------------------------------------------------------------
 set the ready to run status of a task, scheduler does not run (ticks is also cleared)
 taskname is the name of the task, the macro will expand to get the number
 if task owns a semaphore, it will not be enabled (use osENABLE_SEMA_OWNER instead)
-----------------------------------------------------------------------------------------------------------*/

#if (SEMAPHORES_ON)
#define osSET_RTR(taskname)             __set_rtr(taskname ## NUMBER,nSemas)
#define osENABLE_TASK(taskname)         __set_rtr(taskname ## NUMBER,nSemas)
#else
#define osSET_RTR(taskname)             __set_rtr(taskname ## NUMBER,1)
#define osENABLE_TASK(taskname)         __set_rtr(taskname ## NUMBER,1)
#endif

/*---------------------------------------------------------------------------------------------------------*/

#endif // RTOS_H

