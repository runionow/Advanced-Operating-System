#include <xinu.h>

/*
 * @author Arun Nekkalapudi
 */ 

 int32 process_polling[64];
 int32 process_semaphores[64];
 int32 processCount;
 int32 rounds;
 int32 initRounds;

process decrementValue_polling(volatile int32 processIndex);