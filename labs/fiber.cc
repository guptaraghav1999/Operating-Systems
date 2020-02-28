#include "labs/fiber.h"
void f_fiber(addr_t* pmain_stack,addr_t* pf_stack ,shellstate_t* pshellstate){
      addr_t& main_stack = *pmain_stack; 
      addr_t& f_stack = *pf_stack;
      shellstate_t& shellstate=*pshellstate;
      int i;
      int j;
      bool isPrime=false;
      int a=0;
      for(i=1;i<=shellstate.input_int;i++){
          hoh_debug("i"<<i);
          isPrime=true;
          for(j=2;j<i;j++){
              if(i%j==0){
                  isPrime=false;
                  break;
              }
              hoh_debug("j"<<j);
              stack_saverestore(f_stack,main_stack);
          }
          if(isPrime){
          a=i;
          }
      }
      shellstate.answer=a;
      hoh_debug("answer"<<a);
      shellstate.has_executed=true;
      shellstate.f_enter=0;
      stack_saverestore(f_stack,main_stack);
      

}

void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, addr_t& f_stack, addr_t f_array, uint32_t f_arraysize){
  //insert your code here
    if(shellstate.execute_function && !shellstate.has_executed && shellstate.selected_option==4){
      if(shellstate.f_enter==0){
        hoh_debug("init");
        stack_init3(f_stack, f_array, f_arraysize, &f_fiber, &main_stack, &f_stack, &shellstate);
        shellstate.f_enter=1;
      }
      stack_saverestore(main_stack, f_stack);     

    }

}


