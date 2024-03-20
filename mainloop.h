#pragma once
// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#include "esp_event.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

#define MAINLOOP_MAX_TASKS 32

enum mainloop_task_type_e {
    MAINLOOP_PERIODIC_TASKS,
    MAINLOOP_ONE_SHOT_TASKS,
    MAINLOOP_IDLE_TASKS,
    MAINLOOP_INSTANC_TASK,
    MAINLOOP_ESP_EVENT,
};

typedef enum mainloop_task_type_e mainloop_task_type_t;
typedef void (*mainloop_task_cb_t)(void *ctx);
typedef uint8_t priority_t;

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

typedef struct mainloop_s mainloop_t;

// -----------------------------------------------------------------------------
//                                Global Variables
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                          Public Function Declarations
// -----------------------------------------------------------------------------

/**
 * @brief Initialize the mainloop
 * This function is not thread safe, and should be called from the same thread that will call mainloop_run
 * @param[in] mainloop: the mainloop to initialize
 */
mainloop_t *mainloop_create(const char *mainloop_name, uint32_t max_tasks_count, uint32_t idle_interval_ms);

/**
 * @brief Run the mainloop
 * This function is not thread safe, and should be called from the same thread that called mainloop_create
 * This function will block the calling thread until there is no tasks left to execute or mainloop_stop is called
 * @param[in] mainloop: the mainloop to run
 * @param[in] auto_start: if true, the mainloop will start automatically without calling mainloop_start externally
 */
void mainloop_run(mainloop_t *mainloop, bool auto_start);


/**
 * @brief Start the mainloop
 * This function is safe to call from any thread
 * @param[in] mainloop: the mainloop to start
 */
void mainloop_start(mainloop_t *mainloop);

/**
 * @brief Stop the mainloop
 * This function is safe to call from any thread
 * @param[in] mainloop: the mainloop to start
 */
void mainloop_stop(mainloop_t *mainloop);


/**
 * @brief Add a periodic task to the mainloop
 * This function is safe to call from any thread
 * @param[in] mainloop: the mainloop to add the task to
 * @param[in] name: the name of the task
 * @param[in] task: the task to execute
 * @param[in] ctx: the context to pass to the task
 * @param[in] priority: the priority of the task
 * @param[in] period_ms: the period in milliseconds to execute the task
 * @param[in] initial_delay: the delay in milliseconds before the task is executed for the first time
 */
void mainloop_add_periodic_task(mainloop_t *mainloop, const char *name, mainloop_task_cb_t task, void *ctx, priority_t priority, uint32_t period_ms, uint32_t initial_delay);

/**
 * @brief Remove a periodic task from the mainloop
 * This function is safe to call from any thread
 * @param[in] mainloop: the mainloop to remove the task from
 * @param[in] identifier: This could be the task name, task function pointer or the task context
 */
void mainloop_remove_periodic_task(mainloop_t *mainloop, void *identifier);
t
 * @param[in] mainloop: the mainloop to add the task to
 * @param[in] name: the name of the task
 * @param[in] task: the task to execute
 * @param[in] ctx: the context to pass to the task
 * @param[in] priority: the priority of the task
 * @param[in] delay_ms: the delay in milliseconds before the task is executed
 */
void mainloop_add_one_shot_task(mainloop_t *mainloop, const char *name, mainloop_task_cb_t task, void *ctx, priority_t priority, uint32_t delay_ms);

/**
 * @brief Remove a one-shot task from the mainloop
 * This function is safe to call from any thread
 * @param[in] mainloop: the mainloop to remove the task from
 * @param[in] identifier: This could be the task name, task function pointer or the task context
 */
void mainloop_remove_one_shot_task(mainloop_t *mainloop, void *identifier);

/**
 * @brief Execute a task immediately on main thread
 * This function is safe to call from any thread
 * @param[in] mainloop: the mainloop to remove the task from
 * @param[in] name: the name of the task
 * @param[in] task: the task to remove
 * @param[in] priority: the priority of the task
 * @param[in] ctx: the context to pass to the task
 */
void mainloop_execute_task(mainloop_t *mainloop, const char *name, mainloop_task_cb_t task, priority_t priority, void *ctx);

/**
 * @brief Add an idle task to the mainloop that will run on timeout when no other tasks are running
 * This function is safe to call from any thread
 * @param[in] mainloop: the mainloop to add the task to
 * @param[in] name: the name of the task
 * @param[in] task: the task to execute
 * @param[in] priority: the priority of the task
 * @param[in] ctx: the context to pass to the task
 */
void mainloop_add_idle_task(mainloop_t *mainloop, const char *name, mainloop_task_cb_t task, priority_t priority, void *ctx);

/**
 * @brief Remove a idle task from the mainloop
 * This function is safe to call from any thread
 * @param[in] mainloop: the mainloop to remove the task from
 * @param[in] identifier: This could be the task name, task function pointer or the task context
 */
void mainloop_remove_idle_task(mainloop_t *mainloop, void *identifier);


/**
 * @brief Register an event handler to the system event loop
 *
 * This function is the equivalent of esp_event_handler_register, but will deliver the
 * event to the mainloop instead of the system event loop.
 *
 * @param[in] mainloop: the mainloop to add the event to
 * @param[in] name: the name of the event
 * @param[in] event_base the base ID of the event to register the handler for
 * @param[in] event_id the ID of the event to register the handler for
 * @param[in] event_handler the handler function which gets called when the event is dispatched
 * @param[in] event_handler_arg data, aside from event data, that is passed to the handler when it is called
 * @param[in] priority
 */
void mainloop_esp_event_handler_register(mainloop_t *mainloop, const char *name, esp_event_base_t event_base, int32_t event_id, esp_event_handler_t event_handler,
                                         void *event_handler_arg, priority_t priority);

/**
 * @brief Unregister an event handler from the system event loop
 *
 * This function is the equivalent of esp_event_handler_unregister, but will remove the
 * event from the mainloop instead of the system event loop.
 *
 * @param[in] mainloop: the mainloop to remove the event from
 * @param[in] event_base the base ID of the event to unregister the handler for
 * @param[in] event_id the ID of the event to unregister the handler for
 * @param[in] event_handler the handler function which gets called when the event is dispatched
 */
void mainloop_esp_event_handler_unregister(mainloop_t *mainloop, esp_event_base_t event_base, int32_t event_id, esp_event_handler_t event_handler);
