#include "labs/shell.h"
#include "labs/vgatext.h"

//
// initialize shellstate
//
void shell_init(shellstate_t& state){
  const char* p="Shell Initialised";
  int off=80;
  for(int loc=0;*p;loc++,p++){
    vgatext::writechar(loc+off,*p,0xa,0x0,(addr_t)0xb8000);
  }
  off=160;
  const char* p1="Number of Keys Pressed: ";
  for(int loc=0;*p1;loc++,p1++){
  vgatext::writechar(loc+off,*p1,0xe,0x0,(addr_t)0xb8000);
  }
  state.offset=240;
  // state.offset=0; 25 x 80 // 0-960
  state.option_offset=state.offset;
  state.cursor_at=state.offset;

  state.total_key_pressed=0;
  state.is_int=false;

  state.is_option_selected=false;
  state.selected_option=0;
  state.total_options=7;
  state.get_parameter=false;
  state.execute_function=false;
  state.has_executed=false;//////////////////////////////////
  state.input_len=0;
  state.input_int=0;
  state.answer=0;

  state.total_task=0;
  state.task1=0;
  state.task2=0;

  state.prev_fiber=0;
  state.display_option=true;
  state.print_ans=false;
  state.run_task=0;
  //10 chars
  state.char_to_int="0123456789";
  //53 chars
  state.keys="~1234567890-=BTqwertyuiop[]ECasdfghjkl;' S\\zxcvbnm,./";
  // char keys[]= {'esc','1','2','3','4','5','6','7','8','9','0','-','=','back','tab','q','w','e','r','t','y','u','i','o','p','[',']','entr','ctrl','a','s','d','f','g','h','j','k','l',';','\'',' ','shft','\\','z','x','c','v','b','n','m',',','.','/'};
  // char *options[state.total_options]={"Function_1","Function_2","Function_3","Clear Screen"};
  // state.options = options;
}

//
// handle keyboard event.
// key is in scancode format.
// For ex:
// scancode for following keys are:
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | esc |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |  0 |  - |  = |back|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 01  | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 0a | 0b | 0c | 0d | 0e |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | tab |  q |  w |  e |  r |  t |  y |  u |  i |  o |  p |  [ |  ] |entr|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 0f  | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 1a | 1b | 1c |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     |ctrl |  a |  s |  d |  f |  g |  h |  j |  k |  l |  ; |  ' |    |shft|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 1d  | 1e | 1f | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 2a |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
// so and so..
//
// - restrict yourself to: 0-9, a-z, esc, enter, arrows
// - ignore other keys like shift, control keys
// - only handle the keys which you're interested in
// - for example, you may want to handle up(0x48),down(0x50) arrow keys for menu.
//
void shell_update(uint8_t scankey, shellstate_t& stateinout){
// char char_to_int[10]={'0','1','2','3','4','5','6','7','8','9'};
// //esc='~', enter='E', 
// char keys[53]= {'~','1','2','3','4','5','6','7','8','9','0','-','=','B','T','q','w','e','r','t','y','u','i','o','p','[',']','E','C','a','s','d','f','g','h','j','k','l',';','\'',' ','S','\\','z','x','c','v','b','n','m',',','.','/'};
stateinout.total_key_pressed++;
stateinout.key_scancode=(int)scankey;
// int temp=stateinout.total_key_pressed;
// int i=1;

// while(temp!=0){
//   temp=temp/10;
//   i++;
// }
// temp=stateinout.total_key_pressed;
// char no_of_keys[i];

// for(int j=0; j<i; j++ ){
//   no_of_keys[i-j-1]=stateinout.char_to_int[temp%10];
//   temp=temp/10;
// }
// for(int loc=0;loc<i;loc++){
//   vgatext::writechar(loc+184,no_of_keys[loc],0xe,0x0,(addr_t)0xb8000);
// }
  // hoh_debug("Got: "<<unsigned(scankey));
  uint8_t key=unsigned(scankey);
  stateinout.is_int=false;
if(stateinout.print_ans)stateinout.print_ans=false;
  if(stateinout.total_task>0){
    if(key==0x4b){//left Arrow
        if(stateinout.selected_option==5)stateinout.selected_option=6;
        else stateinout.selected_option--;
        return;
    }
    else if(key==0x4d){//Right Arrow
        if(stateinout.selected_option==6)stateinout.selected_option=5;
        else stateinout.selected_option++;
        return;
    }
    else if(key<=53){
    if(stateinout.keys[(int)key-1]=='B'){//Backspace
    
      if(stateinout.get_parameter){

        if(stateinout.cursor_at%80<=20){
        }
        else{
          stateinout.key_pressed=stateinout.keys[(int)key-1];
          stateinout.cursor_at--;
          stateinout.input_len--;
          stateinout.get_input[stateinout.input_len+1]='\0';
        }
      }
    }
    else if(stateinout.keys[(int)key-1]=='E'){//Enter
      if(stateinout.get_parameter){
        stateinout.get_parameter=false;
        stateinout.get_input[stateinout.input_len+1]='\0';
        int len=0;
          int temp=0;
          int dec=1;
          while(stateinout.get_input[len]!='\0'){
            len++;
          }
          for(int i=len;i>=1;i-- ){
            temp=temp+(stateinout.get_input[i-1]-'0')*(dec);
            dec=dec*10;
          }
          stateinout.input_int=temp;
          stateinout.ans_cursor=stateinout.option_offset+80*2+20;
          stateinout.task_ans_cursor[stateinout.run_task]=stateinout.ans_cursor;
          stateinout.task_input[stateinout.run_task]=stateinout.input_int;
          hoh_debug("ans cursor "<<stateinout.ans_cursor/80);
          hoh_debug("run task "<<stateinout.run_task);
          hoh_debug("task Input "<< stateinout.task_input[stateinout.run_task]);
          stateinout.f_enter=0;
          stateinout.option_offset=stateinout.option_offset+80*3;
          stateinout.display_option=true;
          stateinout.input_len=0;
      }
      else{
          if(stateinout.task1<3 && stateinout.selected_option==5){
            stateinout.cursor_at=stateinout.option_offset+80+20;
            if(stateinout.total_task<5){
              stateinout.run_task=(stateinout.run_task+1)%5;
              stateinout.task1++;
              stateinout.total_task++;
              stateinout.get_parameter=true;
              stateinout.cursor_at=stateinout.option_offset+80+20;
              stateinout.display_option=false;
              // stateinout.task_ans_cursor[stateinout.total_task-1]=stateinout.cursor_at+80;
            }
          }
          else if(stateinout.task2<3 && stateinout.selected_option==6){
            stateinout.cursor_at=stateinout.option_offset+80+20;
            if(stateinout.total_task<5){
              stateinout.run_task=(stateinout.run_task+1)%5;
              stateinout.task2++;
              stateinout.total_task++;
              stateinout.get_parameter=true;
              stateinout.cursor_at=stateinout.option_offset+80+20;
              stateinout.display_option=false;
              // stateinout.task_ans_cursor[stateinout.total_task-1]=stateinout.cursor_at+80;
            }
          }
        // stateinout.get_parameter=true;
        // stateinout.cursor_at=stateinout.option_offset+80+20;
        // stateinout.display_option=false;
      }
    }

    else{
      if(stateinout.get_parameter){
        if(scankey>=0x02 && scankey<= 0xb){
          stateinout.input_len=(stateinout.cursor_at)%80 - 20;
          stateinout.key_scancode=(int)scankey;
          stateinout.key_pressed=stateinout.keys[(int)key-1];
          stateinout.get_input[(stateinout.cursor_at)%80-20]=stateinout.key_pressed;
          stateinout.cursor_at++;
          stateinout.is_int=true;
        }

      }
    }
    
  }
  return;
 }


  if(stateinout.execute_function && stateinout.has_executed){
    stateinout.display_option=true;
    stateinout.is_option_selected=false;
    stateinout.execute_function=false;
    stateinout.has_executed=false;
    stateinout.answer=0;
    stateinout.input_int=0;
  }

  if(key==0x39){//Space
    stateinout.key_scancode=(int)scankey;
    stateinout.key_pressed=' ';
  }
  else if(key==0x36){//Other Shift Button
  }
  else if(key==0x4b){
    if(!stateinout.is_option_selected){
      if(stateinout.selected_option==0)stateinout.selected_option=stateinout.total_options-1;
      else stateinout.selected_option=(stateinout.selected_option-1);
    }
  }
  else if(key==0x4d){
      if(!stateinout.is_option_selected){
      stateinout.selected_option=(stateinout.selected_option+1)%stateinout.total_options;
    }
  }
  else if(key==0x48){//Up Arrow
    // }

  }
  else if(key==0x50){//Down Arrow
    
  }
  else if(key<=53){
    if(stateinout.keys[(int)key-1]=='B'){//Backspace
      if(stateinout.get_parameter){
        if(stateinout.cursor_at%80<=20){

        }
        else{
          stateinout.key_scancode=(int)scankey;
          stateinout.key_pressed=stateinout.keys[(int)key-1];
          stateinout.cursor_at--;
          stateinout.input_len--;
          stateinout.get_input[stateinout.input_len+1]='\0';
        }
      }
    }
    else if(stateinout.keys[(int)key-1]=='~'){//Escape
    }
    else if(stateinout.keys[(int)key-1]=='E'){//Enter
      if(!stateinout.is_option_selected){
        stateinout.display_option=false;
        if(stateinout.selected_option<5){
          stateinout.is_option_selected=true;
          stateinout.get_parameter=true;
          stateinout.execute_function=false;
          stateinout.cursor_at=stateinout.option_offset+80+20;
        }
        else {
          if(stateinout.task1<3 && stateinout.selected_option==5){
            if(stateinout.total_task<5){
              if(stateinout.run_task==5)stateinout.run_task=1;
              else stateinout.run_task++;
              stateinout.task1++;
              stateinout.total_task++;
              stateinout.get_parameter=true;
              stateinout.cursor_at=stateinout.option_offset+80+20;
              // stateinout.task_ans_cursor[stateinout.total_task-1]=stateinout.cursor_at+80;
            }
          }
          else if(stateinout.task2<3 && stateinout.selected_option==6){
            if(stateinout.total_task<5){
              if(stateinout.run_task==5)stateinout.run_task=1;
              else stateinout.run_task++;
              stateinout.task2++;
              stateinout.total_task++;
              stateinout.get_parameter=true;
              stateinout.cursor_at=stateinout.option_offset+80+20;
              // stateinout.task_ans_cursor[stateinout.total_task-1]=stateinout.cursor_at+80;
            }
          }
        }
        
      }
      else if(stateinout.is_option_selected && !stateinout.execute_function){
        if(stateinout.selected_option<5){
          stateinout.get_parameter=false;
          stateinout.get_input[stateinout.input_len+1]='\0';
          hoh_debug("input "<<stateinout.get_input);
          int len=0;
          int temp=0;
          int dec=1;
          while(stateinout.get_input[len]!='\0'){
            len++;
          }
          for(int i=len;i>=1;i-- ){
            temp=temp+(stateinout.get_input[i-1]-'0')*(dec);
            dec=dec*10;
          }
          stateinout.input_int=temp;
          stateinout.execute_function=true;
          stateinout.has_executed=false;
          stateinout.cursor_at=stateinout.option_offset+80*2+20;
          stateinout.ans_cursor=stateinout.cursor_at;
          hoh_debug("ans cursor non task "<<stateinout.ans_cursor/80);
          stateinout.option_offset=stateinout.option_offset+80*3;
          stateinout.input_len=0;
          
        }
        // else{
        //   stateinout.get_parameter=false;
        //   stateinout.get_input[stateinout.input_len+1]='\0';
        //   hoh_debug("input "<<stateinout.get_input);
        //   int len=0;
        //   int temp=0;
        //   int dec=1;
        //   while(stateinout.get_input[len]!='\0'){
        //     len++;
        //   }
        //   for(int i=len;i>=1;i-- ){
        //     temp=temp+(stateinout.get_input[i-1]-'0')*(dec);
        //     dec=dec*10;
        //   }
        //   stateinout.input_int=temp;
        //   stateinout.execute_function=true;
        //   stateinout.cursor_at=stateinout.option_offset+80*2+20;
        //   stateinout.option_offset=stateinout.option_offset+80*3;
        //   stateinout.display_option=true;
        //   stateinout.input_len=0;
        // }

      }
      // else if(stateinout.is_option_selected && !stateinout.execute_function){
        // stateinout.get_input[stateinout.input_len+1]='\0';
        // hoh_debug("input "<<stateinout.get_input);
        // int len=0;
        // int temp=0;
        // int dec=1;
        // while(stateinout.get_input[len]!='\0'){
        //   len++;
        // }
        // for(int i=len;i>=1;i-- ){
        //   temp=temp+(stateinout.get_input[i-1]-'0')*(dec);
        //   dec=dec*10;
        // }
        // stateinout.input_int=temp;
        // stateinout.execute_function=true;
        // stateinout.cursor_at=stateinout.option_offset+80*2+20;
        // stateinout.option_offset=stateinout.option_offset+80*3;
        // stateinout.input_len=0;
        

      // } 
    }
    else if(stateinout.keys[(int)key-1]=='T'){//Tab
    
    }
    else{
      if(stateinout.get_parameter){
        if(scankey>=0x02 && scankey<= 0xb){
          stateinout.input_len=(stateinout.cursor_at)%80 - 20;
          stateinout.key_scancode=(int)scankey;
          stateinout.key_pressed=stateinout.keys[(int)key-1];
          stateinout.get_input[(stateinout.cursor_at)%80-20]=stateinout.key_pressed;
          stateinout.cursor_at++;
          stateinout.is_int=true;
        }
        stateinout.key_scancode=(int)scankey;
        stateinout.key_pressed=stateinout.keys[(int)key-1];

      }
      }
    //   if(stateinout.is_option_selected && !stateinout.execute_function){
      //   if(scankey>=0x02 && scankey<= 0xb){
      //     stateinout.input_len=(stateinout.cursor_at)%80 - 20;
      //     stateinout.key_scancode=(int)scankey;
      //     stateinout.key_pressed=stateinout.keys[(int)key-1];
      //     stateinout.get_input[(stateinout.cursor_at)%80-20]=stateinout.key_pressed;
      //     stateinout.cursor_at++;
      //     stateinout.is_int=true;
      //   }
      //   stateinout.key_scancode=(int)scankey;
      //   stateinout.key_pressed=stateinout.keys[(int)key-1];

      // }

    // }
    
  }
  else{

  }
    

}


//
// do computation
//
void shell_step(shellstate_t& stateinout){

//
//one way:
//if a function is enabled in stateinout
//call that function( with arguments stored in stateinout) ;
//stateinout.args[0] = 5;
//stateinout.args[1] = 5;
//
  int temp=stateinout.input_int;
  int ans=1;
  int i=0;
  int count = 1;
  int flag = 0;
  bool isPrime=false;
  int a = 0;
  int input=stateinout.input_int;
  int previousFib = 0, currentFib = 1;
  int newFib = previousFib + currentFib;
  if(stateinout.execute_function && !stateinout.has_executed){
    hoh_debug("non task execution");
    switch (stateinout.selected_option){
      case 0:
            while(temp!=0){
              ans=ans*temp;
              temp=temp-1;
            }
            stateinout.answer= ans;
            hoh_debug("answer"<<stateinout.answer);
            stateinout.has_executed=true;
            stateinout.display_option=true;
            stateinout.print_ans=true;
            break;
      case 1:
            for(int i=1;i<=input;i++){
              isPrime=true;
              for(int j=2;j<i;j++){
                if(i%j==0){
                  isPrime=false;
                  break;
                }
              }
              if(isPrime){
                a=i;
              }
            }
            // hoh_debug("prime no"<<input);
            stateinout.answer= a;
            hoh_debug("answer"<<stateinout.answer);
            stateinout.has_executed=true;
            stateinout.display_option=true;
            stateinout.print_ans=true;
            break;

      case 2:
            if (input <= 1)	{
              stateinout.answer=input;
              stateinout.has_executed=true;
              break;
            }
            
            for (int i = 0; i < input - 1; i++)
            {
              newFib = previousFib + currentFib;
              previousFib = currentFib;
              currentFib  = newFib;
              hoh_debug("answer"<<currentFib);
            }
            stateinout.answer=currentFib;
            hoh_debug("answer"<<stateinout.answer);
            stateinout.has_executed=true;
            stateinout.display_option=true;
            stateinout.print_ans=true;
            break;


            

    }
    // hoh_debug("answer"<<stateinout.answer);
    // stateinout.has_executed=true;
  }
  

}


//
// shellstate --> renderstate
//
void shell_render(const shellstate_t& shell, renderstate_t& render){

  //
  // renderstate. number of keys pressed = shellstate. number of keys pressed
  //
  // renderstate. menu highlighted = shellstate. menu highlighted
  //
  // renderstate. function result = shellstate. output argument
  //
  // etc.
  //
  render.char_to_int=shell.char_to_int;
  // render.get_input=shell.get_input;
  // render.options=shell.options;

  render.offset=shell.offset;
  render.cursor_at=shell.cursor_at;
  render.option_offset=shell.option_offset;
  // render.get_input=shell.get_input;

  render.key_scancode=shell.key_scancode;
  render.key_pressed=shell.key_pressed;
  render.total_key_pressed=shell.total_key_pressed;
  render.is_int=shell.is_int;

  render.is_option_selected=shell.is_option_selected;
  render.selected_option=shell.selected_option;
  render.total_options=shell.total_options;
  render.get_parameter=shell.get_parameter;
  render.execute_function=shell.execute_function;
  render.has_executed=shell.has_executed;
  render.input_len=render.input_len;
  render.input_int=shell.input_int;
  render.answer=shell.answer;

  render.display_option=shell.display_option;
  render.prev_fiber=shell.prev_fiber;
  render.total_task=shell.total_task;

  render.fiber_scheduler_arr[0]=shell.fiber_scheduler_arr[0];
  render.fiber_scheduler_arr[1]=shell.fiber_scheduler_arr[1];
  render.fiber_scheduler_arr[2]=shell.fiber_scheduler_arr[2];
  render.fiber_scheduler_arr[3]=shell.fiber_scheduler_arr[3];
  render.fiber_scheduler_arr[4]=shell.fiber_scheduler_arr[4];

  render.task_ans[0]=shell.task_ans[0];
  render.task_ans[1]=shell.task_ans[1];
  render.task_ans[2]=shell.task_ans[2];
  render.task_ans[3]=shell.task_ans[3];
  render.task_ans[4]=shell.task_ans[4];
  
  render.task_ans_cursor[0]=shell.task_ans_cursor[0];
  render.task_ans_cursor[1]=shell.task_ans_cursor[1];
  render.task_ans_cursor[2]=shell.task_ans_cursor[2];
  render.task_ans_cursor[3]=shell.task_ans_cursor[3];
  render.task_ans_cursor[4]=shell.task_ans_cursor[4];

  render.task_ans_cursor[0]=shell.task_ans_cursor[0];
  render.ans_cursor=shell.ans_cursor;
  render.print_ans=shell.print_ans;
  render.run_task=shell.run_task;


}
//
// compare a and b
//
bool render_eq(const renderstate_t& a, const renderstate_t& b){
  if(a.total_key_pressed==b.total_key_pressed &&  a.offset==b.offset && a.option_offset==b.option_offset && a.cursor_at==b.cursor_at && a.key_scancode==b.key_scancode && a.selected_option==b.selected_option && a.answer==b.answer && a.total_task==b.total_task && a.ans_cursor==b.ans_cursor && a.print_ans==b.print_ans) return true;
  else return false;
}

static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawtext(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);

//
// Given a render state, we need to write it into vgatext buffer
//
void render(const renderstate_t& state, int w, int h, addr_t vgatext_base){
  hoh_debug("render");
  char *options[state.total_options]={"Calculate Factorial","Calculator Largest Prime Number Less than a Number","Calculate Nth Fibbonacci Number","Coroutine", "Fiber", "Fiber 1", "fiber 2"};


  // this is just an example:
  //
  // Please create your own user interface
  //
  // You may also have simple command line user interface
  // or menu based interface or a combination of both.
  //

  int temp=state.total_key_pressed;
  int i=1;
  while(temp!=0){
    temp=temp/10;
    i++;
  }
  temp=state.total_key_pressed;
  char no_of_keys[i];
  for(int j=0; j<i; j++ ){
    no_of_keys[i-j-1]=state.char_to_int[temp%10];
    temp=temp/10;
  }
  for(int loc=0;loc<i;loc++){
    vgatext::writechar(loc+184,no_of_keys[loc],0xe,0x0,vgatext_base);
  }

if(state.print_ans){
      hoh_debug("Render at ans cursor  "<< state.ans_cursor/80 );
      const char* p="Answer: ";
      for(int loc=0;*p;loc++,p++){
        vgatext::writechar(state.ans_cursor-20+loc,*p,0,7,vgatext_base);
      }
      int temp=state.answer;
      int i=1;
      if(temp==0)vgatext::writechar(state.ans_cursor,'0',0x0,0x7,vgatext_base);
      else{
          while(temp!=0){
          temp=temp/10;
          i++;
        }
        temp=state.answer;
        char no_of_keys[i];
        for(int j=0; j<i; j++ ){
          no_of_keys[i-j-1]=state.char_to_int[temp%10];
          temp=temp/10;
        }
        for(int loc=0;loc<i;loc++){
          vgatext::writechar(state.ans_cursor+loc,no_of_keys[loc],0x0,0x7,vgatext_base);
        }
     }
}

  if(state.total_task>0){
    // hoh_debug("total task "<< state.total_task);
    if(state.display_option){
    char *option=options[state.selected_option];
      for(int j=0; j<80; j++){
        vgatext::writechar(state.option_offset+j,' ',0x0,0x7,vgatext_base);
      }
      for(int j=0; *option; option++, j++){
        vgatext::writechar(state.option_offset+j,*option,0xf,0x1,vgatext_base);
      }
  }
    if(state.get_parameter){
      const char* p="Enter Number: ";
      // const char* p="Enter Parametre for function: ";
      for(int loc=0;*p;loc++,p++){
        vgatext::writechar(state.option_offset+80+loc,*p,0,7,vgatext_base);
      }
      if(state.is_int){
        vgatext::writechar(state.cursor_at-1,state.key_pressed,0,7,vgatext_base);

      }
      if(state.key_pressed=='B'){
      // int off=state.option_offset%2000;
      // vgatext::writechar(off,' ',0,7,vgatext_base);
      vgatext::writechar(state.cursor_at,' ',0,7,vgatext_base);
    }
    }
      return;
    }
      // const char* p="Answer: ";
      // for(int loc=0;*p;loc++,p++){
      //   vgatext::writechar(state.task_ans_cursor[state.prev_fiber]-20+loc,*p,0,7,vgatext_base);
      // }
      // int temp=state.task_ans[state.prev_fiber];
      // int i=1;
      // if(temp==0)vgatext::writechar(state.task_ans_cursor[state.prev_fiber],'0',0x0,0x7,vgatext_base);
      // else{
      //     while(temp!=0){
      //     temp=temp/10;
      //     i++;
      //   }
      //   temp=state.task_ans[state.prev_fiber];
      //   char no_of_keys[i];
      //   for(int j=0; j<i; j++ ){
      //     no_of_keys[i-j-1]=state.char_to_int[temp%10];
      //     temp=temp/10;
      //   }
      //   for(int loc=0;loc<i;loc++){
      //     vgatext::writechar(state.task_ans_cursor[state.prev_fiber]+loc,no_of_keys[loc],0x0,0x7,vgatext_base);
      //   }
    
    

  
  if(state.get_parameter){
    const char* p="Enter Number: ";
    // const char* p="Enter Parametre for function: ";
    for(int loc=0;*p;loc++,p++){
    //   int off =state.option_offset%2000;
    //   vgatext::writechar(off+80*(state.total_options)+loc,*p,0,7,vgatext_base);
      vgatext::writechar(state.option_offset+80+loc,*p,0,7,vgatext_base);
    }
    if(state.is_int){
      // int off=state.option_offset%2000;
      // vgatext::writechar(off-1,state.key_pressed,0,7,vgatext_base);
      vgatext::writechar(state.cursor_at-1,state.key_pressed,0,7,vgatext_base);

    }
        if(state.key_pressed=='B'){
      // int off=state.option_offset%2000;
      // vgatext::writechar(off,' ',0,7,vgatext_base);
      vgatext::writechar(state.cursor_at,' ',0,7,vgatext_base);
    }

  }

  // if(state.is_option_selected && !state.execute_function){
    // const char* p="Enter Number: ";
    // // const char* p="Enter Parametre for function: ";
    // for(int loc=0;*p;loc++,p++){
    // //   int off =state.option_offset%2000;
    // //   vgatext::writechar(off+80*(state.total_options)+loc,*p,0,7,vgatext_base);
    //   vgatext::writechar(state.option_offset+80+loc,*p,0,7,vgatext_base);
    // }
    // if(state.is_int){
    //   // int off=state.option_offset%2000;
    //   // vgatext::writechar(off-1,state.key_pressed,0,7,vgatext_base);
    //   vgatext::writechar(state.cursor_at-1,state.key_pressed,0,7,vgatext_base);

    // }
  // }
                // if(state.is_longcomp_running && state.is_option_selected && !state.get_parameter){
                //   const char* p="Answer: ";
                //   for(int loc=0;*p;loc++,p++){
                //     vgatext::writechar((state.option_offset)+80*(-1)+loc,*p,0,7,vgatext_base);
                //   }
                // }
                                              // if(state.execute_function && state.has_executed){
                                              //   const char* p="Answer: ";
                                              //   for(int loc=0;*p;loc++,p++){
                                              //     // int off=state.option_offset%2000;
                                              //     // vgatext::writechar(off+80*(-2)+loc,*p,0,7,vgatext_base);
                                              //     vgatext::writechar((state.option_offset)+80*(-1)+loc,*p,0,7,vgatext_base);
                                              //   }
                                              //   int temp=state.answer;
                                              //   int i=1;
                                              //   // int off=state.option_offset%2000;
                                              //   // if(temp==0)vgatext::writechar(off,'0',0x0,0x7,vgatext_base);
                                              //   if(temp==0)vgatext::writechar(state.cursor_at,'0',0x0,0x7,vgatext_base);
                                              //   else{
                                              //       while(temp!=0){
                                              //       temp=temp/10;
                                              //       i++;
                                              //     }
                                              //     temp=state.answer;
                                              //     char no_of_keys[i];
                                              //     for(int j=0; j<i; j++ ){
                                              //       no_of_keys[i-j-1]=state.char_to_int[temp%10];
                                              //       temp=temp/10;
                                              //     }
                                              //     for(int loc=0;loc<i;loc++){
                                              //       // int off=state.option_offset%2000;
                                              //       // vgatext::writechar(off+loc,no_of_keys[loc],0x0,0x7,vgatext_base);
                                              //       vgatext::writechar(state.cursor_at+loc,no_of_keys[loc],0x0,0x7,vgatext_base);
                                              //     }
                                              //   }
                                              // }

  // if(!state.is_option_selected || state.execute_function && state.has_executed ){
  //   for(int i=0;i<state.total_options;i++){
  //     char *option=options[i];
  //     for(int j=0; *option; option++, j++){
  //       if(i==state.selected_option){
  //         // int off=state.option_offset%2000;
  //         // vgatext::writechar(off+80*i+j,*option,0xf,0x1,vgatext_base);
  //         vgatext::writechar(state.option_offset+80*i+j,*option,0xf,0x1,vgatext_base);
  //       }
  //       else{
  //         // int off=state.option_offset%2000;
  //         // vgatext::writechar(off+80*i+j,*option,0x0,0x7,vgatext_base);
  //         vgatext::writechar(state.option_offset+80*i+j,*option,0x0,0x7,vgatext_base);
  //       }
  //     }
  //   }
  // }

  // if(!state.is_option_selected|| state.has_executed){
  //   char *option=options[state.selected_option];
  //     for(int j=0; j<80; j++){
  //       vgatext::writechar(state.option_offset+j,' ',0x0,0x7,vgatext_base);
  //     }
  //     for(int j=0; *option; option++, j++){
  //       vgatext::writechar(state.option_offset+j,*option,0xf,0x1,vgatext_base);
  //     }
  // }
  if(state.display_option){
    char *option=options[state.selected_option];
      for(int j=0; j<80; j++){
        vgatext::writechar(state.option_offset+j,' ',0x0,0x7,vgatext_base);
      }
      for(int j=0; *option; option++, j++){
        vgatext::writechar(state.option_offset+j,*option,0xf,0x1,vgatext_base);
      }
  }


  // else if(state.is_option_selected && !state.execute_function){
  //   if(state.key_pressed=='B'){
  //     // int off=state.option_offset%2000;
  //     // vgatext::writechar(off,' ',0,7,vgatext_base);
  //     vgatext::writechar(state.cursor_at,' ',0,7,vgatext_base);
  //   }
  // }

}
//
//
// helper functions
//
//

static void writecharxy(int x, int y, uint8_t c, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  vgatext::writechar(y*w+x,c,bg,fg,vgatext_base);
}

static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int y=y0;y<y1;y++){
    for(int x=x0;x<x1;x++){
      writecharxy(x,y,0,bg,fg,w,h,vgatext_base);
    }
  }
}

static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){

  writecharxy(x0,  y0,  0xc9, bg,fg, w,h,vgatext_base);
  writecharxy(x1-1,y0,  0xbb, bg,fg, w,h,vgatext_base);
  writecharxy(x0,  y1-1,0xc8, bg,fg, w,h,vgatext_base);
  writecharxy(x1-1,y1-1,0xbc, bg,fg, w,h,vgatext_base);

  for(int x=x0+1; x+1 < x1; x++){
    writecharxy(x,y0, 0xcd, bg,fg, w,h,vgatext_base);
  }

  for(int x=x0+1; x+1 < x1; x++){
    writecharxy(x,y1-1, 0xcd, bg,fg, w,h,vgatext_base);
  }

  for(int y=y0+1; y+1 < y1; y++){
    writecharxy(x0,y, 0xba, bg,fg, w,h,vgatext_base);
  }

  for(int y=y0+1; y+1 < y1; y++){
    writecharxy(x1-1,y, 0xba, bg,fg, w,h,vgatext_base);
  }
}

static void drawtext(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int i=0;i<maxw;i++){
    writecharxy(x+i,y,str[i],bg,fg,w,h,vgatext_base);
    if(!str[i]){
      break;
    }
  }
}

static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  enum {max=sizeof(uint32_t)*2+1};
  char a[max];
  for(int i=0;i<max-1;i++){
    a[max-1-i-1]=hex2char(number%16);
    number=number/16;
  }
  a[max-1]='\0';

  drawtext(x,y,a,maxw,bg,fg,w,h,vgatext_base);
}

