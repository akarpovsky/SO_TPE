#include "../../include/defs.h"
#include "../../include/structs.h"
#include "../../include/keyboard.h"


struct key_t * key;
extern struct tty_t ttys[];
extern int actualTTY;

char e_lowercase[256] = {0,0x1b,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,'a','s','d','f','g','h','j','k','l',';','\'','\\',0,'\\','z','x','c','v','b','n','m',',','.','/','\\','.',56,' ',58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,'<'};

char e_uppercase[256] =  {0,0x1b,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,'A','S','D','F','G','H','J','K','L',':','"',0xb3,0,'|','Z','X','C','V','B','N','M','<','>','?','\\','.',56,' ',58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,'>'};

char e_capslowercase[256] = {0,0x1b,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','Q','W','E','R','T','Y','U','I','O','P','[',']','\n',0,'A','S','D','F','G','H','J','K','L',';','\'','\\',0,0x60,'Z','X','C','V','B','N','M',',','.','-','\\','.',56,' ',58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,'<'};

char e_capsuppercase[256] =  {0,0x1b,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,'A','S','D','F','G','H','J','K','L',':','"',0xb3,0,'~','Z','X','C','V','B','N','M','<','>','?','\\','.',56,' ',58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,'>'};

char s_lowercase[256] = {0,0xb3,'1','2','3','4','5','6','7','8','9','0','\'',0xa8,'\b','\t','q','w','e','r','t','y','u','i','o','p','´','+','\n',0,'a','s','d','f','g','h','j','k','l',A_ENIE_MIN,'{',0xb3,0,'}','z','x','c','v','b','n','m',',','.','-','\\','.',56,' ',58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,'<'};

char s_uppercase[256] =  {0,0xa7,'!','"','#','$','%','&','/','(',')','=','?',0xad,'\b','\t','Q','W','E','R','T','Y','U','I','O','P','¨','*','\n',0,'A','S','D','F','G','H','J','K','L',A_ENIE_MAY,'[',0xa7,42,']','Z','X','C','V','B','N','M',';',':','_','\\','.',56,' ',58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,'>'};

char s_capslowercase[256] = {0,0xb3,'1','2','3','4','5','6','7','8','9','0','\'',0xa8,'\b','\t','Q','W','E','R','T','Y','U','I','O','P','´','+','\n',0,'A','S','D','F','G','H','J','K','L',A_ENIE_MAY,'{',0xb3,0,'}','Z','X','C','V','B','N','M',',','.','-','\\','.',56,' ',58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,'<'};

char s_capsuppercase[256] =  {0,0xa7,'!','"','#','$','%','&','/','(',')','=','?',0xad,'\b','\t','q','w','e','r','t','y','u','i','o','p','¨','*','\n',0,'a','s','d','f','g','h','j','k','l',A_ENIE_MIN,'[',0xa7,42,']','z','x','c','v','b','n','m',';',':','_','\\','.',56,' ',58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,'>'};


unsigned char insertKey(unsigned char c){

      if(!bufferIsFull()){
// 	  debugger('a');
	  ttys[actualTTY].keyboard->buffer[ttys[actualTTY].keyboard->tail] = c;
// 	  keyBoardBuffer[actualTTY][bufferTail] = c;
//  	  int tail = (ttys[actualTTY].keyboard->tail)+1;
//	  ttys[actualTTY].keyboard->tail = 2;
	  if( ++(ttys[actualTTY].keyboard->tail) == K_BUFFER_SIZE ){
	      ttys[actualTTY].keyboard->tail = 0;
	      return c;
	  }
      }	
	return 0;
}

unsigned char getKey(){
    int head = ttys[actualTTY].keyboard->head;
//     debugger('g');
//     if(bufferIsEmpty==FALSE)
	
      if(!isEmptyBuffer()){
// 	debugger('b');
	ttys[actualTTY].keyboard->buffer[ttys[actualTTY].keyboard->head];
	if( ++(ttys[actualTTY].keyboard->head) == K_BUFFER_SIZE ){
	    ttys[actualTTY].keyboard->head = 0;
	}
	
      return ttys[actualTTY].keyboard->buffer[head];
    }
    

  return EMPTY_CHAR;
    
}

int bufferIsFull(){
    if( (ttys[actualTTY].keyboard->tail+1) % K_BUFFER_SIZE == ttys[actualTTY].keyboard->head ){
	return TRUE;
    }
    return FALSE;
}

int isEmptyBuffer(){
    if( ttys[actualTTY].keyboard->tail == ttys[actualTTY].keyboard->head ){
	return TRUE;
    }
    return FALSE;
}




struct key_t * parseKey(unsigned char c) {
  //   move_cursor_rc(0,0);


	if(ttys[actualTTY].keyboard->escaped_key){
	  c+=256;
	  ttys[actualTTY].keyboard->escaped_key=0;
	}
	switch(c) {
	    case NUMLOCK:
	      ttys[actualTTY].keyboard->num_state = !ttys[actualTTY].keyboard->num_state;
	      key->keyType = HIDDEN_KEY;
	      updateLeds();
	      break;
	    case SCROLLLOCK:
	      ttys[actualTTY].keyboard->scroll_state = !ttys[actualTTY].keyboard->scroll_state;
	      key->keyType = HIDDEN_KEY;
	      updateLeds();
	      break;
	    case LEFT_SHIFT_PRESSED:
	    case RIGHT_SHIFT_PRESSED:
	      ttys[actualTTY].keyboard->shift_state = TRUE;
	      if(ttys[actualTTY].keyboard->ctrl_state == TRUE)
		  changeLang();
	      key->keyType = HIDDEN_KEY;
	      break;
	    case RIGHT_SHIFT_RELEASED: 
	    case LEFT_SHIFT_RELEASED:
	      ttys[actualTTY].keyboard->shift_state = FALSE; 
	      key->keyType = HIDDEN_KEY;
	      break;
	    case CAPSLOCK: 
	      ttys[actualTTY].keyboard->caps_state = !ttys[actualTTY].keyboard->caps_state;
	      updateLeds();
//	      printf("caps_state=%d\n",ttys[actualTTY].keyboard->caps_state);
	      key->keyType = HIDDEN_KEY;
	      break;
	    case ESCAPED_KEY:
	      ttys[actualTTY].keyboard->escaped_key = TRUE;
	      key->keyType = HIDDEN_KEY;
	      break;
	    case ALT_PRESSED:
	      ttys[actualTTY].keyboard->alt_state = TRUE;
	      break;
	    case ALT_RELEASED:
	      ttys[actualTTY].keyboard->alt_state = FALSE;
	      break;
	    case CONTROL_PRESSED:
	      ttys[actualTTY].keyboard->ctrl_state = TRUE;
	      if(ttys[actualTTY].keyboard->shift_state == TRUE)
		  changeLang();
	      break;
	    case CONTROL_RELEASED:
	      ttys[actualTTY].keyboard->ctrl_state = FALSE;
	      break;
	    case ARROW_UP:
	    case ARROW_DOWN:
	    case ARROW_LEFT:
	    case ARROW_RIGHT:
	    case PAGE_UP:
	    case PAGE_DOWN:
	      key->keyType = FUNCTION_KEY;
	      break;
	    case F1: case F2: case F3: case F4:
	      key->keyType = FUNCTION_KEY;
	      changeTTY(c-0x3b);
	      break;
	    case DEAD_KEY:
		if(ttys[actualTTY].keyboard->lang == SPANISH){
		  ttys[actualTTY].keyboard->dead_key = TRUE;
		  break;
		}
	    default:
		if (c & 0x80) {
		    key->keyType = HIDDEN_KEY;
		} else {
		if(isPrintable(c)){
		    key->keyType = ALPHANUM_KEY;
		    if(ttys[actualTTY].keyboard->caps_state){
			if( ttys[actualTTY].keyboard->lang == ENGLISH ){
			    key->ascii = (ttys[actualTTY].keyboard->shift_state?e_capsuppercase:e_capslowercase)[c];
			}
			else if( ttys[actualTTY].keyboard->lang == SPANISH ){
			    if(ttys[actualTTY].keyboard->dead_key && ttys[actualTTY].keyboard->shift_state && isVowel(c)){
			      key->ascii = accVowel(c);
			    }
			    else
			      key->ascii = (ttys[actualTTY].keyboard->shift_state?s_capsuppercase:s_capslowercase)[c];
			}
		    }else{
			if( ttys[actualTTY].keyboard->lang == ENGLISH ){
			    key->ascii = (ttys[actualTTY].keyboard->shift_state?e_uppercase:e_lowercase)[c];
			}
			else if( ttys[actualTTY].keyboard->lang == SPANISH ){
			    if(ttys[actualTTY].keyboard->dead_key && !ttys[actualTTY].keyboard->shift_state && isVowel(c)){
			      key->ascii = accVowel(c);
			    }
			    else
			      key->ascii = (ttys[actualTTY].keyboard->shift_state?s_uppercase:s_lowercase)[c];
			}
		    }
		ttys[actualTTY].keyboard->dead_key = FALSE;
		} else{
		  
		}
		// do something with new_char
	      }
	    break;

      }
      return key;
}

 
int isPrintable(unsigned char c){
    if((c>1 && c<29) || (c>29 && c<42) || (c>42 && c<55 ) || c==86 || c==57){
	return 1;
    }
    return 0;
}
void changeLang(){
    if(ttys[actualTTY].keyboard->lang == ENGLISH)
	ttys[actualTTY].keyboard->lang = SPANISH;
    else
	ttys[actualTTY].keyboard->lang = ENGLISH;
    print_header();
    move_cursor(ttys[actualTTY].screen->wpos/2);
}

int isVowel(char c){
    if(c==0x12 || c==0x1e || c==0x16 || c==0x17 || c==0x18 )
      return TRUE;
    return FALSE;
}

char accVowel(char c){
    char ch;
    switch(c){
      case 0x12:
	ch='é';
	break;
      case 0x1e:
	ch='á';
	break;
      case 0x16:
	ch='ú';
	break;
      case 0x17:
	ch='í';
	break;
      case 0x18:
	ch='ó';
	break;
    }
    return ch;
}

void viewBuffer(){
//    ttys[actualTTY].screen->address[ttys[actualTTY].screen->wpos+=2]= getc();
	putc(getc());
//     int i = 0;
//     ttys[actualTTY].screen->address[ttys[actualTTY].screen->wpos+=2]= '[';  
//     for(i = 0 ; i < K_BUFFER_SIZE; i++){
//         ttys[actualTTY].screen->address[ttys[actualTTY].screen->wpos+=2]= ttys[actualTTY].keyboard->buffer[i];  
//     }
//        ttys[actualTTY].screen->address[ttys[actualTTY].screen->wpos+=2]= ']';  
}

void updateLeds(){

	unsigned char leds = ttys[actualTTY].keyboard->scroll_state | (ttys[actualTTY].keyboard->num_state * 2) | (ttys[actualTTY].keyboard->caps_state * 4);
	_out(0x60, 0xed);
	while (_in(0x60) != 0xfa);
	_out(0x60, leds);
}
