/*! @file
  @brief
  Realtime multitask monitor for mruby/c

  <pre>
  Copyright (C) 2015-2018 Kyushu Institute of Technology.
  Copyright (C) 2015-2018 Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.
  </pre>
*/

#ifndef MRBC_SRC_RRT0_H_
#define MRBC_SRC_RRT0_H_

/***** Feature test switches ************************************************/
/***** System headers *******************************************************/
//@cond
#include <stdint.h>
//@endcond

/***** Local headers ********************************************************/
#include "vm.h"

#ifdef __cplusplus
extern "C" {
#endif
/***** Constant values ******************************************************/

//================================================
/*!@brief
  Task state
*/
enum MrbcTaskState {
  TASKSTATE_DORMANT   = 0x00,	//!< Domant
  TASKSTATE_READY     = 0x01,	//!< Ready
  TASKSTATE_RUNNING   = 0x03,	//!< Running
  TASKSTATE_WAITING   = 0x04,	//!< Waiting
  TASKSTATE_SUSPENDED = 0x08,	//!< Suspended
};

enum MrbcTaskReason {
  TASKREASON_SLEEP = 0x00,
  TASKREASON_MUTEX = 0x01,
};

static const int MRBC_TASK_DEFAULT_PRIORITY = 128;
static const int MRBC_TASK_DEFAULT_STATE = TASKSTATE_READY;

#if !defined(MRBC_TASK_NAME_LEN)
#define MRBC_TASK_NAME_LEN 15
#endif


/***** Macros ***************************************************************/
/***** Typedefs *************************************************************/

struct RMutex;

//================================================
/*!@brief
  Task control block
*/
typedef struct RTcb {
#if defined(MRBC_DEBUG)
  uint8_t type[4];		//!< set "TCB\0" for debug.
#endif
  struct RTcb *next;		//!< daisy chain in task queue.
  char name[MRBC_TASK_NAME_LEN+1]; //!< task name (optional)
  uint8_t priority;		//!< task priority. initial value.
  uint8_t priority_preemption;	//!< task priority. effective value.
  volatile uint8_t timeslice;	//!< time slice counter.
  enum MrbcTaskState state : 8;	//!< task state.
  enum MrbcTaskReason reason : 8; //!< sub state.

  union {
    uint32_t wakeup_tick;	//!< wakeup time for sleep state.
    struct RMutex *mutex;
  };

  struct VM vm;

} mrbc_tcb;


//================================================
/*!@brief
  Mutex
*/
typedef struct RMutex {
  volatile int lock;
  struct RTcb *tcb;
} mrbc_mutex;

#define MRBC_MUTEX_INITIALIZER { 0 }


/***** Global variables *****************************************************/
/***** Function prototypes **************************************************/
void mrbc_tick(void);
mrbc_tcb *mrbc_tcb_new(int regs_size, enum MrbcTaskState task_state, int priority);
mrbc_tcb *mrbc_create_task(const void *byte_code, mrbc_tcb *tcb);
int mrbc_start_task(mrbc_tcb *tcb);
int mrbc_run(void);
void mrbc_sleep_ms(mrbc_tcb *tcb, uint32_t ms);
void mrbc_relinquish(mrbc_tcb *tcb);
void mrbc_change_priority(mrbc_tcb *tcb, int priority);
void mrbc_suspend_task(mrbc_tcb *tcb);
void mrbc_resume_task(mrbc_tcb *tcb);
mrbc_mutex *mrbc_mutex_init(mrbc_mutex *mutex);
int mrbc_mutex_lock(mrbc_mutex *mutex, mrbc_tcb *tcb);
int mrbc_mutex_unlock(mrbc_mutex *mutex, mrbc_tcb *tcb);
int mrbc_mutex_trylock(mrbc_mutex *mutex, mrbc_tcb *tcb);
void mrbc_cleanup(void);
void mrbc_init(void *heap_ptr, unsigned int size);
void pq(const mrbc_tcb *p_tcb);
void pqall(void);


/***** Inline functions *****************************************************/

#ifdef __cplusplus
}
#endif
#endif // ifndef MRBC_SRC_RRT0_H_
