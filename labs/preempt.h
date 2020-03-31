#pragma once

#include "util/config.h"


//
// preempt_t : State for your timer/preemption handler
//
// Note:
//  We've one instance of core_t per each core.
//  To access this instance, you need to use %gs:0
//  (The entire kernel doesn't have any global/static variables)
//
// %gs:core_offset_preempt will point to start of preempt_t instance
//
// for example: 
// %gs:0 will return pointer to core_t
// %gs:core_offset_mainstack will return core_t::main_stack
// %gs:core_offset_preempt+0 will return core_t::saved_stack
//
// etc.
//
// See Definition of core_t in x86/main.h
//

struct preempt_t{
  int is_interrupt=1;
  // your data structure, if any
  addr_t saved_stack; //feel free to change it - provided as an example
  addr_t stacks[5];
  int index;
  int is_yield=0;
  

};


//
// 
// This macro is being called from x86/except.cc
//
//
// _name: label name
// _f   : C function to be called 
//        ex: we may have to do send EOI to LAPIC or PIC etc.
// 

#  define  _ring0_preempt(_name,_f)            \
  __asm(                                       \
      "  .text                            \n\t"\
      " " STR(_name) ":                   \n\t"\
      "  pushl %edx                       \n\t"\
      "  pushl %ecx                       \n\t"\
      "  pushl %eax                       \n\t"\
      "  call " STR(_f) "                 \n\t"\
      "  popl  %eax                       \n\t"\
      "  popl  %ecx                       \n\t"\
      "  popl  %edx                       \n\t"\
      "                                   \n\t"\
      "  # insert your code here          \n\t"\
      "  movl $0,%gs:" STR(core_offset_preempt) "+0                                 \n\t"\
      "  movl %gs:" STR(core_offset_preempt) "+32, %eax                                \n\t"\
      "  cmpl $0, %eax                                                                   \n\t"\
      "  jne done                                                                          \n\t"\
      "  #FXSAVE (%esp)                                                                        \n\t"\
      "  pushl %eax                                                                     \n\t"\
      "  pushl %ebx                                                                           \n\t"   \
      "  pushl %ecx                                                                           \n\t"   \
      "  pushl %edx                                                                           \n\t"   \
      "  pushl %esi                                                                           \n\t"   \      
      "  pushl %edi                                                                           \n\t"   \      
      "  pushl %ebp                                                                       \n\t" \
      "  #movl %esp, %ebp                                                                       \n\t" \
      "  #subl $512, %esp                                                                       \n\t" \
      "  #andl $0xfffffff0 , %esp                                                                              \n\t"\  
      "  #fxsave (%esp)                                                                              \n\t"\  
      "  #pushl %ebp                                                                              \n\t"\  
      "  pushl $1f                                                                                       \n\t"\
      "                                                                                \n\t"\                          
      "  movl %esp, %gs:" STR(core_offset_preempt) "+4                                 \n\t"\
      "                                                                                    \n\t"\
      "  movl %gs:" STR(core_offset_preempt) "+28, %eax                                                                                \n\t"\
      "  cmpl $0, %eax                                                                               \n\t"\
      "  je index0                                                                               \n\t"\    
      "  cmpl $1, %eax                                                                               \n\t"\
      "  je index1                                                                                \n\t"\ 
      "  cmpl $2, %eax                                                                               \n\t"\      
      "  je index2                                                                               \n\t"\ 
      "  cmpl $3, %eax                                                                               \n\t"\         
      "  je index3                                                                                \n\t"\  
      "  cmpl $4, %eax                                                                               \n\t"\      
      "  je index4                                                                               \n\t"\    
      "                                                                                \n\t"\               
      "                                                                                  \n\t"\  
      "                                                                                  \n\t"\  
      "  index0:                                                                                \n\t"\                          
      "  movl %esp, %gs:" STR(core_offset_preempt) "+8                                 \n\t"\
      "  movl %gs:" STR(core_offset_mainstack) "+0, %esp                                 \n\t"\
      "  ret                                                                                     \n\t"\
      "  index1:                                                                                \n\t"\                          
      "  movl %esp, %gs:" STR(core_offset_preempt) "+12                                 \n\t"\
      "  movl %gs:" STR(core_offset_mainstack) "+0, %esp                                 \n\t"\
      "  ret                                                                                     \n\t"\
      "  index2:                                                                                \n\t"\                          
      "  movl %esp, %gs:" STR(core_offset_preempt) "+16                                 \n\t"\
      "  movl %gs:" STR(core_offset_mainstack) "+0, %esp                                 \n\t"\
      "  ret                                                                                     \n\t"\
      "  index3:                                                                                \n\t"\                          
      "  movl %esp, %gs:" STR(core_offset_preempt) "+20                                 \n\t"\
      "  movl %gs:" STR(core_offset_mainstack) "+0, %esp                                 \n\t"\
      "  ret                                                                                     \n\t"\
      "  index4:                                                                                \n\t"\                          
      "  movl %esp, %gs:" STR(core_offset_preempt) "+24                                 \n\t"\
      "  movl %gs:" STR(core_offset_mainstack) "+0, %esp                                 \n\t"\ 
      "  ret                                                                                     \n\t"\                             
      "                                                                                  \n\t"\
      "  1:                                                                                       \n\t"\
      "  #sti                                                                                \n\t"\
      "  #popl %ebp                                                                              \n\t"                                         \
      "  #fxrstor (%esp)                                                                              \n\t"\  
      "  #movl %ebp , %esp                                                                              \n\t"\  
      "  popl %ebp                                                                              \n\t"\        
      "  popl %edi                                                                           \n\t"   \
      "  popl %esi                                                                           \n\t"   \
      "  popl %edx                                                                           \n\t"   \
      "  popl %ecx                                                                        \n\t"                                         \
      "  popl %ebx                                                                \n\t"                                         \
      "  popl %eax                                                                \n\t"                                         \
      "  #FXRSTOR (%esp)                                                                           \n\t"\
      "  done:                                                                                   \n\t"\
      "  jmp iret_toring0                                                                                      \n\t"\
      "                                                                             \n\t"\
      "                                                                                              \n\t"\
      )                                                                      \


