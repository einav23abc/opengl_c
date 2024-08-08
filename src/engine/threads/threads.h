#ifndef THREADS_H
#define THREADS_H

#include <windows.h>
#include <stdint.h>

// fix wierd defines in <windows.h>
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif


typedef struct {
    int32_t thread_id;
    HANDLE thread_handle;
} thread_t;

typedef struct {
    uint8_t suspended; // start in suspended state
    uint32_t ss_size; // size of dynamically allocated stack
} thread_attr_t;


int32_t thread_create(  thread_t* thread, const thread_attr_t* attr,
                        void* (*start_routine)(void*), void* arg);

// \returns The thread's previous suspend count
int32_t thread_suspend(thread_t thread);

// \returns The thread's previous suspend count.
int32_t thread_resume(thread_t thread);

void exit_thread(int32_t exit_code);

// \returns The thread's exit code
int32_t thread_join(thread_t thread);

void thread_cancel(thread_t thread);

// \returns The thread's exit code or STILL_ACTIVE if thread did not exit
int32_t thread_get_exit_code(thread_t thread);

// \returns The current thread's id
int32_t thread_self();

void sleep(uint32_t ms);

#endif