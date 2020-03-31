#include "labs/fiber_scheduler.h"
#include "labs/preempt.h"
#include "devices/lapic.h"
//
// stackptrs:      Type: addr_t[stackptrs_size].  array of stack pointers (generalizing: main_stack and f_stack)
// stackptrs_size: number of elements in 'stacks'.
//
// arrays:      Type: uint8_t [arrays_size]. array of memory region for stacks (generalizing: f_array)
// arrays_size: size of 'arrays'. equal to stackptrs_size*STACK_SIZE.
//
// Tip: divide arrays into stackptrs_size parts.
// Tip: you may implement a circular buffer using arrays inside shellstate_t
//      if you choose linked lists, create linked linked using arrays in
//      shellstate_t. (use array indexes as next pointers)
// Note: malloc/new is not available at this point.
//

void f_fiber1(addr_t& main_stack,addr_t& f_stack ,shellstate_t& shellstate, preempt_t &preempt){

      int ans_cursor= shellstate.ans_cursor;
      int i;
      int j;
      bool isPrime=false;
      int a=0;
      int input = shellstate.input_int;

      // hoh_debug("input1---" << input);
      for(i=1;i<=input;i++){
          hoh_debug("i1 "<<shellstate.prev_fiber);
          isPrime=true;
          for(j=2;j<i;j++){
            if(i%j==0){
              isPrime=false;
              break;
            }
              // hoh_debug("yo "<< input);
            // preempt.is_interrupt=1;
            // stack_saverestore(f_stack,main_stack);
          }
         
          if(isPrime){
            a=i; 
          }
      }
      // if(a==12586316){
      //   hoh_debug("shittttttttttttt");
      // }
      // asm volatile ("cli");
      // hoh_debug("answer--- "<<a);  
      // asm volatile ("sti");    
      shellstate.answer=a;
      shellstate.ans_cursor=ans_cursor;
      shellstate.print_ans=true;

      shellstate.has_executed=true;
      shellstate.fiber_scheduler_arr[shellstate.prev_fiber]=false;
      shellstate.total_task-=1;
      shellstate.task1--;
      if(shellstate.total_task==0){
        shellstate.f_enter=0;
      }
      stack_saverestore(f_stack,main_stack);

}

void f_fiber2(addr_t& main_stack,addr_t& f_stack ,shellstate_t& shellstate, preempt_t &preempt){

      int ans_cursor= shellstate.ans_cursor;
      int i;
      int j;
      bool isPrime=false;
      int a=0;
      int input = shellstate.input_int;
      // hoh_debug("prev fib "<<shellstate.prev_fiber);
      // hoh_debug("input2  " << input);
      for(i=1;i<=input;i++){
          hoh_debug("i2 "<<shellstate.prev_fiber);
          isPrime=true;
          for(j=2;j<i;j++){
              if(i%j==0){
                  isPrime=false;
                  break;
              }
            //   hoh_debug("main_stack"<< main_stack);
              // preempt.is_interrupt=1;
              preempt.is_yield=1;
              stack_saverestore(f_stack,main_stack);
          }
          if(isPrime){
            a=i;
          }
      }
      
      // hoh_debug("answer--- "<<a);      
      shellstate.answer=a;
      shellstate.ans_cursor=ans_cursor;
      shellstate.print_ans=true;
      shellstate.has_executed=true;
      shellstate.fiber_scheduler_arr[shellstate.prev_fiber]=false;
      shellstate.total_task-=1;
      shellstate.task2--;
      if(shellstate.total_task==0){
        shellstate.f_enter=0;
      }
      stack_saverestore(f_stack,main_stack);

}


void shell_step_fiber_scheduler(shellstate_t& shellstate, addr_t& main_stack,  preempt_t& preempt, addr_t stackptrs[], size_t stackptrs_size, addr_t arrays, size_t arrays_size, dev_lapic_t& lapic){
    
     
    //insert your code here
    
    if(shellstate.total_task>0 && !shellstate.get_parameter){
        int index=shellstate.prev_fiber;
        if(shellstate.f_enter==0 && shellstate.selected_option==5){
            index++;
            index=index%5;
            while(shellstate.fiber_scheduler_arr[index]){
                index++;
                index=index%5;
            }
            hoh_debug("init5" << shellstate.total_task);
            
            shellstate.prev_fiber=index;
            preempt.index=shellstate.prev_fiber;

            shellstate.fiber_scheduler_arr[shellstate.prev_fiber]=true;
            
            stack_init4(stackptrs[index], arrays+(arrays_size/stackptrs_size)*index, arrays_size/stackptrs_size , &f_fiber1, &main_stack, &stackptrs[index], &shellstate, &preempt);
            shellstate.f_enter=1;
            lapic.reset_timer_count(100000);
            

        }
        else if(shellstate.f_enter==0 && shellstate.selected_option==6){
            
            index++;
            index=index%5;

            while(shellstate.fiber_scheduler_arr[index]){
                index++;
                index=index%5;
            }
            hoh_debug("init6" << shellstate.total_task);
        
            shellstate.prev_fiber=index;
            preempt.index=shellstate.prev_fiber;
            shellstate.fiber_scheduler_arr[shellstate.prev_fiber]=true;

            stack_init4(stackptrs[index], arrays+(arrays_size/stackptrs_size)*index, arrays_size/stackptrs_size , &f_fiber2, &main_stack, &stackptrs[index], &shellstate, &preempt);
            shellstate.f_enter=1; 
            lapic.reset_timer_count(100000);
                         
        }
        else if(shellstate.f_enter==1){
            index++;
            index=index%5;
            while(!shellstate.fiber_scheduler_arr[index]){
              index++;
              index=index%5;
            }
            
            shellstate.prev_fiber=index;
            preempt.index=shellstate.prev_fiber;          
        }
        lapic.reset_timer_count(100000000);
        stack_saverestore(main_stack, stackptrs[index]);
        
        if(preempt.is_yield==1){
          // hoh_debug("Ohhhhhhhhh");
          preempt.is_yield=0;
        }
        else if(preempt.is_interrupt==0){
          // hoh_debug("Help Me");
          // lapic.reset_timer_count(0);
          stackptrs[index]=preempt.stacks[preempt.index];
          preempt.is_interrupt=1;

        }
            
    }

}