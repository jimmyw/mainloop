
// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include "mainloop.h"


// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

enum mainloop_task_type_e {
    MAINLOOP_PERIODIC_TASKS,
    MAINLOOP_ONE_SHOT_TASKS,
    MAINLOOP_IDLE_TASKS,
    MAINLOOP_INSTANC_TASK,
    MAINLOOP_ESP_EVENT,
};



typedef enum mainloop_task_type_e mainloop_task_type_t;

struct task_s {
    mainloop_task_type_t task_type;
    const char *name;
    mainloop_task_cb_t task;
    void *task_ctx;
    priority_t priority;
    union {
        uint32_t period_ms;
        uint32_t delay_ms;
    } task_data;
};

struct mainloop_s {
    const char *name;
    // Tasks in this array, will always be sorted and executed in priority order
    struct task_s *tasks;
};

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//                                Global Variables
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//                                Static Variables
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------
