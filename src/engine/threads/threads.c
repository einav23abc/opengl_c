#include "threads.h"

int32_t thread_create(  thread_t* thread, const thread_attr_t* attr,
                        void* (*start_routine)(void*), void* arg) {
    uint32_t creation_flags = 0;                        
    uint32_t ss_size = 0;
    
    if (attr != NULL) {
        if (attr->ss_size != 0) {
            ss_size = attr->ss_size;
            creation_flags |= STACK_SIZE_PARAM_IS_A_RESERVATION;
        }
        if (attr->suspended) {
            creation_flags |= CREATE_SUSPENDED;
        }

    }
    
    HANDLE thread_handle = CreateThread(
        NULL, // windows security options - unnessecery
        (DWORD)                  ss_size,
        (LPTHREAD_START_ROUTINE) start_routine,
        (LPVOID)                 arg,
        (DWORD)                  creation_flags,
        (PDWORD)                 &(thread->thread_id)
    );
    
    // eroor
    if (thread_handle == NULL) {
        return GetLastError();
    }

    thread->thread_handle = thread_handle;

    return 0;
}

// returns the thread's previous suspend count
int32_t thread_suspend(thread_t thread) {
    return SuspendThread(thread.thread_handle);
}

// returns the thread's previous suspend count.
int32_t thread_resume(thread_t thread) {
    return ResumeThread(thread.thread_handle);
}

void exit_thread(int32_t exit_code) {
    ExitThread(exit_code);
}

// returns the thread's exit code
int32_t thread_join(thread_t thread) {
    // wait
    WaitForSingleObject(
        thread.thread_handle,
        INFINITE // The time-out interval [...] If dwMilliseconds is INFINITE, the function will return only when the object is signaled.
    );
    
    // get exit code
    int32_t exit_code = thread_get_exit_code(thread);

    // close the handle
    CloseHandle(thread.thread_handle);

    return exit_code;
}

void thread_cancel(thread_t thread) {
    TerminateThread(thread.thread_handle, 0);
    
    // close the handle
    CloseHandle(thread.thread_handle);
}

// returns the thread's exit code or STILL_ACTIVE if thread did not exit
int32_t thread_get_exit_code(thread_t thread) {
    int32_t exit_code;
    GetExitCodeThread(
        thread.thread_handle,
        (PDWORD)&exit_code
    );
    return exit_code;
}

// returns the current thread's id
int32_t thread_self() {
    return GetCurrentThreadId();
}

void sleep(uint32_t ms) {
    Sleep(ms);
}