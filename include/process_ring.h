#include <xinu.h>

/*
 * @author Arun Nekkalapudi
 */ 

 int32 process_polling[65];
 sid32 process_semaphores[65];
 int32 processValue_semaphore; 
 sid32 process_doneSemaphores[65];
 int32 processCount;
 int32 rounds;
 int32 initRounds;
 int32 pollingValueStore[6401]
 int32 liveCounter = 0;
 //struct time t;
 
process decrementValue_polling(volatile int32 processIndex);
process decrementValue_semaphore(volatile int32 processIndex);
void print_usage(void);
