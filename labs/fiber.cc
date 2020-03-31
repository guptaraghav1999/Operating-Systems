#include "labs/fiber.h"
#include "labs/preempt.h"
#include "devices/lapic.h"

void f_fiber(addr_t& f_stack,addr_t& main_stack ,shellstate_t& shellstate, preempt_t& preempt){
    int i;
    int j;
    bool isPrime=false;
    int a=0;
    for(i=1;i<=shellstate.input_int;i++){
      // hoh_debug("In fiber1 "<<i);
      isPrime=true;
      // hoh_debug("In fiber2 "<<i);
      for(j=2;j<i;j++){
        // hoh_debug("In fiber3 "<<j);
        if(i%j==0){
          isPrime=false;
          break;
        }
        // preempt.is_interrupt=0;
        // stack_saverestore(f_stack,main_stack);
        // hoh_debug("In fiber4 "<<i);
      }
      if(isPrime){
        a=i;
      }
    }
    // hoh_debug("In fiber5");
    shellstate.answer=a;
    hoh_debug("Fiber Answer "<<a);
    shellstate.has_executed=true;
    shellstate.display_option=true;
    shellstate.f_enter=0;
    shellstate.print_ans=true;
    stack_saverestore(f_stack,main_stack);
}

void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, preempt_t& preempt, addr_t& f_stack, addr_t f_array, uint32_t f_arraysize, dev_lapic_t& lapic){
  //insert your code here
    if(shellstate.execute_function && !shellstate.has_executed && shellstate.selected_option==4){
      if(shellstate.f_enter==0){
        stack_init4(f_stack, f_array, f_arraysize, &f_fiber, &f_stack, &main_stack, &shellstate, &preempt);
        lapic.reset_timer_count(10000000);
        shellstate.f_enter=1;
      }
      // hoh_debug("Heyyy111");
      
      stack_saverestore(main_stack, f_stack);
      f_stack=preempt.saved_stack;
      // hoh_debug("shell step fiber"<< preempt.is_interrupt);
      if(preempt.is_interrupt==0){
        // hoh_debug("Help Me");
        lapic.reset_timer_count(10000000);
        preempt.is_interrupt=1;
      }
    }
        

}


