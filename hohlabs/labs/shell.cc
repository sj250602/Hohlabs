#include "labs/shell.h"
#include "labs/vgatext.h"

//
// initialize shellstate
//

static char getCharFromScanKey(uint8_t scankey){
    char* all = (char*)" 1234567890    qwertyuiop    asdfghjkl     zxcvbnm";
    if(int(scankey-0x01)>=50){
        return ' ';
    }
    return all[int(scankey-0x01)];
}

void factorial(int n,int& cnt){
    if(n==0){
        return ;
    }
    for(int i=0;i<n;i++){
        cnt++;
        factorial(n-1,cnt);
    }
}

void shell_init(shellstate_t& state){
    state.num_keys = 0;
    state.cur_line = 0;
    state.cur_coln = 0;
    state.ltask = false;
    state.exec_f = false;
    state.fibo = false;
    state.fact = false;
    state.echo = false;
    state.ctask = false;
    state.cstart = 'd';
    state.carg = 0;
    state.cdone = false;
    state.ftask = false;
    state.fstart = 'd';
    state.farg = 0;
    state.fdone = false;
    state.fret = 0;
    
    state.buffer_pos = 0;
    state.GArg = 0;
    state.HArg = 0;
    state.G = false;
    state.H = false;
    state.G_invoke = 0;
    state.H_invoke = 0;
    
    for(int i=0;i<5;i++){
    	state.g_h[i] = ' ';
    	state.array_arg[i] = 0;
    	state.runnable[i] = false;
    	state.sret[i] = 0;
    	state.sdone[i] = false;
    }
  //  state.max_line = 0;
    for(int i=0; i<50; i++){
        state.commands[i] = ' ';
    }
    for(int i=0; i<50; i++){
        state.result[i] = ' ';
    }

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

void cals(shellstate_t &stateinout);

void shell_update(uint8_t scankey, shellstate_t& stateinout){

    hoh_debug("Got: "<<unsigned(scankey));
    stateinout.num_keys++;
    char charPressed = getCharFromScanKey(scankey);
    if(charPressed!=' '){
        if(stateinout.exec_f==true){
            stateinout.cur_line++;
            stateinout.exec_f = false;
            for(int i=0; i<50; i++){
                stateinout.result[i] = ' ';
            }
        }
        stateinout.commands[stateinout.cur_coln] = charPressed;
        stateinout.cur_coln++;
    } else {
        if(scankey==0x1c){
            stateinout.exec_f = true;
            stateinout.cur_line += 1;
            stateinout.cur_coln = 0;
            if (stateinout.commands[0] == 'f' && stateinout.commands[1] == 'a' && stateinout.commands[2] == 'c' && stateinout.commands[3]=='t' && stateinout.commands[4]==' '){
                stateinout.fact = true;
            }
            else if (stateinout.commands[0] == 'f' && stateinout.commands[1] == 'i' && stateinout.commands[2] == 'b' && stateinout.commands[3]=='o' && stateinout.commands[4]==' '){
                stateinout.fibo= true;
            }
            else if (stateinout.commands[0] == 'e' && stateinout.commands[1] == 'c' && stateinout.commands[2] == 'h' && stateinout.commands[3]=='o' && stateinout.commands[4]==' '){
                stateinout.echo= true;
            }else if (stateinout.commands[0] == 'l' && stateinout.commands[1] == 't' && stateinout.commands[2] == 'a' && stateinout.commands[3]=='s' && stateinout.commands[4]=='k'&& stateinout.commands[5]==' '){
                stateinout.ltask= true;
            }else if(stateinout.commands[0] == 'c' && stateinout.commands[1] == 't' && stateinout.commands[2] == 'a' && stateinout.commands[3]=='s' && stateinout.commands[4]=='k'){
            	stateinout.ctask = true;
            }else if(stateinout.commands[0] == 'f' && stateinout.commands[1] == 't' && stateinout.commands[2] == 'a' && stateinout.commands[3]=='s' && stateinout.commands[4]=='k'){
            	stateinout.ftask = true;
            }else if(stateinout.commands[0]=='g' && stateinout.commands[1] == 't' && stateinout.commands[2] == 'a' && stateinout.commands[3]=='s' && stateinout.commands[4]=='k'){
            	stateinout.G = true;
            }else if(stateinout.commands[0]=='h' && stateinout.commands[1] == 't' && stateinout.commands[2] == 'a' && stateinout.commands[3]=='s' && stateinout.commands[4]=='k'){
            	stateinout.H = true;
            }
      //      stateinout.command[0] = '$';
      	    cals(stateinout);
        }
        if(scankey==0x4b){
            if(stateinout.cur_coln!=0){
                stateinout.cur_coln--;
            }
        }
        if(scankey==0x39){
          stateinout.cur_coln++;    
        }
        if(scankey==0x0e){
            if(stateinout.cur_coln!=0){
                stateinout.cur_coln--;
                stateinout.commands[stateinout.cur_coln] = ' ';
            }
        }
        
    }
}



void cals(shellstate_t& stateinout){
  if(stateinout.fact == true){
   // int j = 0;
    int j = 5;
    int num = 0;
    while(j<50 && stateinout.commands[j]!=' '){
      int cur = int(stateinout.commands[j]-'0');
      if(cur>=0 && cur<=9){
        num = 10*num+cur;
      } else {
        char* err = (char*)"Can't find factorial number for symbols";
        for(int m=0; m<39; m++){
            stateinout.result[m] = err[m];
        }
        stateinout.fact = false;
        break;
      }
      j++;
    }
    if (stateinout.fact == true){
      int factans = 1;
      for(int i =1; i<=num; i++){
        factans *=i;
      }
      int extra = factans;
      int len = 0;
      while(factans!=0){
        len++;
        factans/=10;
      }
        for(int i=0;i<50;i++){
            stateinout.result[i] =' ';
        }
      
      while(extra!=0){
        stateinout.result[--len]=char(extra%10 + 48);
        extra/=10;
      }
      stateinout.fact = false;
    }
  }
  else if (stateinout.fibo == true){
    int j = 5;
    int num = 0;
    while(j<50 && stateinout.commands[j]!=' '){
      int cur = int(stateinout.commands[j]-'0');
      if(cur>=0 && cur<=9){
        num = 10*num+cur;
      } else {
        char* err = (char*)"Can't find fibonacci number for symbols";
        for(int m=0; m<39; m++){
            stateinout.result[m] = err[m];
        }
        stateinout.fibo = false;
        break;
      }
      j++;
    }
    if (stateinout.fibo == true){
      int f1 = 0;
      if(num==1){
          stateinout.result[0] = '0';
      }else{
        int f2 = 1;
        int ans = 1;
        for(int i =2; i<num; i++){
            int temp = f1;
            f1 = f2;
            f2 = temp +f2;
        }
        for(int i=0;i<50;i++){
            stateinout.result[i] =' ';
        }
        int extra = f2;
        int len = 0;
        while(f2!=0){
          len++;
          f2/=10;
        }
        while(extra!=0){
          stateinout.result[--len]=char(extra%10 + 48);
          extra/=10;
        }
      }
      stateinout.fibo = false;
    }
  }
  else if (stateinout.echo == true){
    int j = 5;
    char* ars;
   // int num = 0;
    while(j<50){
     // int cur = int(stateinout.command[j]-'0');
        stateinout.result[j-5] = stateinout.commands[j];
        j++;
    }
    stateinout.echo = false;
  }else if(stateinout.ltask==true){
     int j = 6;
     int num = 0;
     while(j<50 && stateinout.commands[j]!=' '){
       int cur = int(stateinout.commands[j]-'0');
       if(cur>=0 && cur<=9){
         num = 10*num+cur;
       } else {
         char* err = (char*)"Can't find sequence number for symbols";
        for(int m=0; m<38; m++){
            stateinout.result[m] = err[m];
        }
         stateinout.ltask = false;
         break;
       }
       j++;
     }
     if (stateinout.ltask == true){
       int factans = 0;
       factorial(num,factans);
       int extra = factans;
       int len = 0;
       while(factans!=0){
         len++;
         factans/=10;
       }
       for(int i=0;i<50;i++){
            stateinout.result[i] =' ';
        }
       while(extra!=0){
         stateinout.result[--len]=char(extra%10 + 48);
         extra/=10;
       }
       stateinout.ltask = false;
     }
  }else if(stateinout.ctask==true){
  	int j = 6;
     int num = 0;
     while(j<50 && stateinout.commands[j]!=' '){
       int cur = int(stateinout.commands[j]-'0');
       if(cur>=0 && cur<=9){
         num = 10*num+cur;
       } else {
         char* err = (char*)"Can't find sequence number for symbols";
        for(int m=0; m<38; m++){
            stateinout.result[m] = err[m];
        }
         stateinout.ctask = false;
         break;
       }
       j++;
     }
       for(int i=0;i<50;i++){
            stateinout.result[i] =' ';
        }
        stateinout.carg = num;
  }else if(stateinout.ftask==true){
  
  	int j = 6;
     int num = 0;
     while(j<50 && stateinout.commands[j]!=' '){
       int cur = int(stateinout.commands[j]-'0');
       if(cur>=0 && cur<=9){
         num = 10*num+cur;
       } else {
         char* err = (char*)"Can't find sequence number for symbols";
        for(int m=0; m<38; m++){
            stateinout.result[m] = err[m];
        }
         stateinout.ftask = false;
         break;
       }
       j++;
     }
     
       for(int i=0;i<50;i++){
            stateinout.result[i] =' ';
        }
        stateinout.farg = num;
  }else if(stateinout.G==true){
     int j = 6;
     int num = 0;
     while(j<50 && stateinout.commands[j]!=' '){
       int cur = int(stateinout.commands[j]-'0');
       if(cur>=0 && cur<=9){
         num = 10*num+cur;
       } else {
         char* err = (char*)"Can't find sequence number for symbols";
        for(int m=0; m<38; m++){
            stateinout.result[m] = err[m];
        }
         stateinout.G = false;
         break;
       }
       j++;
     }
     
     for(int i=0;i<50;i++){
            stateinout.result[i] =' ';
        }
        
        stateinout.GArg = num;
        
  }else if(stateinout.H==true){
     int j = 6;
     int num = 0;
     while(j<50 && stateinout.commands[j]!=' '){
       int cur = int(stateinout.commands[j]-'0');
       if(cur>=0 && cur<=9){
         num = 10*num+cur;
       } else {
         char* err = (char*)"Can't find sequence number for symbols";
        for(int m=0; m<38; m++){
            stateinout.result[m] = err[m];
        }
         stateinout.H = false;
         break;
       }
       j++;
     }
     
     for(int i=0;i<50;i++){
            stateinout.result[i] =' ';
        }
        
        stateinout.HArg = num;
        
  }

  //
  //one way:
  // if a function is enabled in stateinout
  //   call that function( with arguments stored in stateinout) ;
//stateinout.args[0] = 5;
//stateinout.args[1] = 5;
  //


}


//
// do computation
//
void shell_step(shellstate_t& stateinout){
	// kuch nahi karunga 
}


//
// shellstate --> renderstate
//
void shell_render(const shellstate_t& shell, renderstate_t& render){
    render.num_keys = shell.num_keys;
    render.cur_line= shell.cur_line;
    render.cur_coln  = shell.cur_coln;
    render.commands = shell.commands;
    render.result =shell.result;
    render.exec_f = shell.exec_f;
    render.fibo =shell.fibo;
    render.fact =shell.fact;
    render.echo = shell.echo;
    render.ltask = shell.ltask;
    render.ctask = shell.ctask;
    render.carg = shell.carg;
    render.cstart = shell.cstart;
    render.cdone = shell.cdone;
    render.ftask = shell.ftask;
    render.fstart = shell.fstart;
    render.fdone = shell.fdone;
    render.farg = shell.farg;
    render.fret = shell.fret;
    
    render.buffer_pos = shell.buffer_pos;
    render.GArg = shell.GArg;
    render.HArg = shell.HArg;
    render.G = shell.G;
    render.H = shell.H;
    render.G_invoke = shell.G_invoke;
    render.H_invoke = shell.H_invoke;
    render.array_arg[5] = shell.array_arg[5];
    render.runnable[5] = shell.runnable[5];
    render.g_h = shell.g_h;
    render.sret[5]  =shell.sret[5];
    render.sdone[5] = shell.sdone[5];
    
    
  //
  // renderstate. number of keys pressed = shellstate. number of keys pressed
  //
  // renderstate. menu highlighted = shellstate. menu highlighted
  //
  // renderstate. function result = shellstate. output argument
  //
  // etc.
  //
}


//
// compare a and b
//
bool render_eq(const renderstate_t& a, const renderstate_t& b){
  if ((a.cur_line== b.cur_line) && (a.cur_coln == b.cur_coln) && (a.num_keys == b.num_keys) 
)    {
      bool aa = false;
      for(int i=0;i<50;i++){
        if ((a.commands[i]==b.commands[i]) && (a.result[i]== b.result[i]))
         // return true;
         continue;
        else
          aa = true;
          break;
      }
      if(aa==true){
        return false;
      }
      else{
        return true;
      }

     // return true;
    }
    return false;
}


static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawtext(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumberindec(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);

//
// Given a render state, we need to write it into vgatext buffer
//
void render(const renderstate_t& state, int w, int h, addr_t vgatext_base){


  // this is just an example:
  //
  // Please create your own user interface
  //
  // You may also have simple command line user interface
  // or menu based interface or a combination of both.
  //
  drawtext(0,state.cur_line,state.result,50,0,7,w,h,vgatext_base);
  //drawtext(50,1,state.result,50,0,7,w,h,vgatext_base);
  const char* key_prss = "No. of Key Pressed: ";
  drawtext(50,1, key_prss, 40, 0, 7,w,h,vgatext_base);
  drawnumberindec(70, 1, state.num_keys, 10, 0, 7, w, h, vgatext_base);
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

static char dec2char(int n){
    return char(n + '0');
}

static void drawnumberindec(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  enum {max=sizeof(uint32_t)*2+1};
  char a[max];
  for(int i=0;i<max-1;i++){
    a[max-1-i-1]=dec2char(number%10);
    number=number/10;
  }
  a[max-1]='\0';

  drawtext(x,y,a,maxw,bg,fg,w,h,vgatext_base);
}

