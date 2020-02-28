#pragma once

#include "labs/shell.h"
#include "util/coroutine.h"

// state for your coroutine implementation:
struct f_t{
    //insert your code here
    int i;
    int j;
    int a;
    int enter;
    bool done;
    bool isPrime;
    int input;
    coroutine_t fcoro;

};


void shell_step_coroutine(shellstate_t& shellstate, coroutine_t& f_coro, f_t& f_locals);


