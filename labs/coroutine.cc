#include "labs/coroutine.h"


void shell_step_coroutine(shellstate_t& shellstate, coroutine_t& f_coro, f_t& f_locals){

    // insert your code here
    // if(f_locals.enter==0){
    //     f_locals.enter=1;
    //     f_locals.fcoro=f_coro;
    // }
    int& i= f_locals.i;
    int& j= f_locals.j;
    int& a= f_locals.a;
    int& enter=f_locals.enter;
    bool& done= f_locals.done;
    int input =shellstate.input_int;
    bool& isPrime= f_locals.isPrime;

    if(shellstate.execute_function && !shellstate.has_executed && shellstate.selected_option==3){
        h_begin(f_coro);
        for(i=1;i<=shellstate.input_int;i++){
            // hoh_debug("i"<<i);
            isPrime=true;
            for(j=2;j<i;j++){
                if(i%j==0){
                    isPrime=false;
                    break;
                }
                done=false;
                enter++;
                h_yield(f_coro);
            }
            if(isPrime){
            a=i;
            }
        }
        shellstate.answer=a;
        hoh_debug("Coroutine Answer"<<a);
        shellstate.has_executed=true;
        shellstate.display_option=true;
        shellstate.print_ans=true;
        enter=0;
        done=true;
        // h_end(f_locals.fcoro);
        coroutine_reset(f_coro);
        // h_end(f_coro);
        
    }


}


