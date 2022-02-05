// SMALL Line-Bloc editor  V0.2.3d (Feb2022)
// (C)2022 ALAIN THÉROUX
// License: You can do what you want with these code but leave these three lines.
 
// This is a quick and compact editor to be used within a simple interpreter or other shell-like program.
// The main loop() in Arduino simply call the editorMain()
// The editorMain() is a loop that wait for any input from the user. You can insert at the end any other code that should
// run concurrently.
// Editor use some ANSI <ESC> code to display text. The option '$' toggle on/off this feature. Slower when <esc> mode is disabled.
// This is a version without the bloc save or load from filesystem. The functions are there but not actives (blocRead, blocSave).
// Pressing <enter> twice move the cursor down without changing the text
// Move the cursor up/down then press <enter> to overwrite the content of that line. Previous text is lost
// I left bloc/save/load there as a guide for documenting.
// It is made for purpose to not erase previous editor content as this can be a way to do a quick edit in/out of a command-line or interpreter.
// 
// if you change these parameters, "case" functions inside the editorMain() have to be updated.
#define NOL   16              // Number of line in the editor
#define LLen  64              // len of each line

static char editorMode=0;     // 0= ansi, 1 = dumb terminal
char bloc[NOL*LLen];    // 1024   // The actual data-editor is saved into bloc[]. The full page is stored here.
short blocID=0;
char dirty='*';
char fileName[12];            // filename if required
short lastBlocID=0;
unsigned char showcase=0;
char line[65];                // Len of a single line+1
unsigned char currentLine=0;

void editorMain();
void displayBloc();
void EraseBloc();
void clearLine();
short blocRead(short b);
short blocWrite(short b);
void ExtendedSerialGets();

char shead[] = "Editor v0.2.3d   ";
char sline[]="  +--------------------------------------------------------------+   ";
char sMenu1[] = "(c)opy (p)aste (i)nsert (x)delete (t)op (l)ast (4-arrows) <Bs> <Del> ! $";
char sMenu2[] = "(N)ext-bloc (P)rev-bloc (B)loc (C)lear (R)efresh (L)oad (S)ave (Q)uit ";

char sBottom[]= "[B" ;
char sRight[]=  "[C" ;
char sLeft[]=  "[D" ;   // ?? test
char sHome[]=   "[1H" ;
char sClr[]=    "[2J" ;

void printS(char*s){ Serial.print(s);}
void printChar(char c){ Serial.print(c); }
void printLn(){ Serial.println();}
void printN(int n){ Serial.print((int)n);}
short charAvailable(){ return(Serial.available());};
char getChar(){ return(Serial.read());}

void moveCursorLeft(){printChar((char)27); printS(sLeft);}
void moveCursorRight(){printChar((char)27); printS(sRight);}
void moveCursorDown() { printChar((char)27); printS(sBottom);}
void moveCursorHome(){printChar((char)27); printS(sHome);}
void clrScreen(){ printChar((char)27); printS(sClr); }

void clearLine(){
  for(short i=0; i<64; i++)
    line[i]=0;
}

void EraseBloc(){
  unsigned short i,j;
  for(i=0;i<NOL; i++){   //16  24
    for(j=0; j<64; j++)
      bloc[i*64 + j]=0;

    bloc[i*64+63]=0;    // null-terminated string instead of <CR>, Your choice   31-01-2022
  }
}

void scrRefresh(){
  if(editorMode==0){
    moveCursorHome(); clrScreen(); 
  }
  else{
    for(short i=0; i < 25; i++)
      printLn();
  }
}
   
void displayBloc(){
  unsigned short c, cc;
  unsigned short l, bid;
  scrRefresh();
  printS(shead); printS(" Bloc:"); printN(blocID);  printChar(' ');
  printS(fileName); printS("    Mode:");printChar((char)dirty); printChar(editorMode); 
        printS("        "); if(showcase) printChar('!');  printLn();
  printS(sline); printLn();
  for(l=0;l<NOL; l++){   //16  24
    printChar('|'); 
    if(currentLine == l) printChar('>'); else printChar(' ');
    for(c=0; c<64; c++){
      cc=bloc[l*64+c];
      if(cc == 0) cc = ' ';
      if(showcase){       // to display not writable char
        if(cc == 0) cc = '_';
        if(cc == 13) cc = '$';
        else if(cc < 32) cc= '%';
      }
      printChar((char)cc);
    }
    printLn();
  }
  printS(sline);printLn();   printS(sMenu1); printLn(); printS(sMenu2); printLn();
}

void editorLine(){
    clearLine();
    moveCursorHome();  // HOME
    for(int up = 0; up < currentLine+2; up++){
      moveCursorDown();   // BOTTOM
    }
    //moveCursorRight();
    moveCursorRight();
    printChar('$');
    ExtendedSerialGets();

    if(line[0] == 0) { // empty line    13
      currentLine++;
      if(currentLine==NOL) {
        currentLine --;   // --- 32 31 24
      }
      displayBloc(); 
    }
    else {
        line[63]=13;
        for(int i=0;i<64;i++){
           bloc[currentLine*64 + i]=line[i];
        }
        currentLine++;
        if(currentLine==NOL)  currentLine --;
        clearLine();
        displayBloc();
    }
}

short blocRead(short b){      // 0 = success, 1 = error
  // Put here your code to actually read a bloc from  FILE
  return(0);
}

short blocWrite(short b){      // 0 = success, 1 = error
  // Put here your code to actually save to a FILE
  return(0);
}

// this editor is a stream and with little modification can work with other task simultanously
char  sQuit[]=" Quit";
short col=0;

void editorMain(){
  char c;
  char tmp[8];
  char copyline[64];                                       // for copy-paste line
  short i,bid,z;
  //scrRefresh();
  blocID=0;
  char dirty=' ';
  displayBloc();
  
  for(i=0; i < 64; i++)  copyline[i]=' ';                  // clear copy-paste line is case of paste before copy
  copyline[63]=0;
  
  while(1){
    while(charAvailable()){
        c =getChar();
        if(c == 27) { 
            //Serial.print(" ESC>");  
            while(!charAvailable()); c=getChar();//Serial.print((int)c); 
            if(c != 91) break;
            while(!charAvailable()); c=getChar();printN((int)c); 
            switch(c){
              case 'A':  c='u';    break; // up     convert to 'up'
              case 'B':  c='d';    break; // down   convert to 'bottom'
              case 'C':  c= 13;    break; // right  convert to CR
              default : c=0;
            }
         }
        switch(c){ 
                case('u'): if(currentLine>0) currentLine--; displayBloc();col=0; break;    // up one line
                case('d'): dirty='*'; if(currentLine<(NOL-1)) currentLine++; col=0;;displayBloc(); break;   // down one line -- 15
                case('!'): if(showcase) showcase=0; else showcase=1; displayBloc(); break;
                case('$'): if(editorMode==0) editorMode=1; else editorMode=0;break;        // Enable or disable <ESC> sequence in editor
                case('l'): currentLine=NOL-1; displayBloc(); break;                      // last line  --15
                case('t'): currentLine=0; displayBloc(); break;                       // top of page, first line
                case('x'): dirty='*';
                        for(i=currentLine*64; i < ((NOL*64)-64); i++) { bloc[i]=bloc[i+64];} 
                        for(i=0; i <64;i++) bloc[(NOL*64)-64 +i]=0;   
                        bloc[(NOL*64)-1]=13; 
                        displayBloc(); 
                      break;
                case('i'):
                        dirty='*';
                        z=currentLine*64 -1;                        
                        if(currentLine==0){z=0;}
                          for(i=1024-64; i > z; i--) {
                             bloc[i+64-1]=bloc[i-1];
                           } 
                        for( i=0; i<63; i++)
                          bloc[currentLine*64+i]=0;
                        bloc[currentLine*64+63]=13;
                        displayBloc(); 
                      break;
                case('p'): dirty = '*';
                       for(i=0;i<64; i++) {                                      // paste line
                        bloc[currentLine*64+i]= copyline[i];  
                        printChar(copyline[i]);
                      }
                      if (currentLine <15) currentLine++;
                      displayBloc(); 
                      break;
                case('c'): printChar('C');                                         // copy line
                      for(i=0;i<64; i++) {
                        copyline[i] = bloc[currentLine*64+i];
                        printChar(copyline[i]);
                      }
                      break;
                
                case ('C'): EraseBloc(); dirty = '*';
                      currentLine=0;
                      displayBloc();
                      break;
                case('R'):  // TOP OF SCREEN - Refresh
                      currentLine=0;
                      displayBloc();
                      break;   
                case('L'): dirty = ' ';
                      if(blocRead(blocID) == -1){ printS("File error loading bloc");}
                      displayBloc();
                      break;
                case('S'): dirty = ' ';
                      if(blocWrite(blocID) == -1){ printS("File error saving bloc");}
                      displayBloc();
                      break;
                case('B'):
                      printS("Bloc ? "); ExtendedSerialGets();  
                      strncpy(tmp, line, 6);
                      z = atoi(tmp);
                      if(z<0  || z>511) blocID=0; else blocID=z;
                      if(dirty=='*') {
                        if(blocWrite(blocID) == -1){ printS("File error current saving bloc");}
                      }
                      if(blocRead(blocID) == -1){ printS("File error loading bloc");}
                      displayBloc();
                      break;
                case('N'): 
                       if(dirty=='*'){
                          if(blocWrite(blocID)==-1){
                             printS("File save error!"); return;
                          }
                       }
                       blocID ++;
                       currentLine=0; 
                       if (blocID== 512){ blocID--;}     // limit size to 512 K
                       if(blocRead(blocID) !=0){
                         EraseBloc();
                         blocWrite(blocID);
                         dirty=' ';
                       }
                       displayBloc();
                       break;
                case('P'):
                      if(dirty=='*'){
                        if(blocWrite(blocID)==-1){
                          printS("File save error!"); return;
                        }
                      }
                      if(blocID>0) blocID--;
                      blocRead(blocID);
                      displayBloc();
                      break;
                case('Q'):
                      // Update file when file save/load is implemented
                      return;   //exit main edit
                      break;
                
                case(13): dirty = '*';            // CR or cursor-right
                    editorLine(); break;
        } 
  } //while ser.avail
 } // do-while1
}

void ExtendedSerialGets(){           // until CR then end by null(0). No handling for timeout for the moment... Return -1 on timeout or string len??
char c;
int t ,index=0;
    strncpy(&line[0], &bloc[currentLine*64], 64);      // copy previous content of line
    line[64]=0;
    do{
        if(charAvailable()){
            t=0;
            c=getChar();
            if(c == 27) { 
                while(!charAvailable()); 
                c=getChar();
                if(c == 91) {
                  while(!charAvailable()); c= getChar();
                  switch(c){
                    case('C'): if(index < 63){ index++; moveCursorRight();} 
                                break;  //right
                    case('D'): if(index >0) {index--; moveCursorLeft(); }
                               break;  //left      
                    case('3'):
                              line[63]=' ';
                              for(t=index; t<64; t++){
                                line[t]=line[t+1];
                              }
                              line[63]=' ';line[64]=0;
                              printChar(' ');
                              printChar((char)13);printS("|$"); printS(&line[0]);
                              printChar((char)13);
                                for(t=0; t<index+1 ; t++)
                                   moveCursorRight();
                              moveCursorLeft(); index--;
                              break;
                              defaut: ;
                  }
                }
                else{         // ESC key and not cursor arrows. Esc keep the remaining of line. Enter cut the line
                  return;  
                }
            }  
            else{
              switch(c){
                case 8: if(index>0){                                                        // Back Space
                          printChar(8); printChar(' ');printChar(8);
                          line[--index]=' '; line[63]=' ';
                          for(t=index; t<64; t++)
                            line[t]=line[t+1];
                          line[63]=' ';line[64]=0;
                          printChar((char)13); printS("|$"); printS(&line[0]); printChar(' ');
                          printChar((char)13); printS("|$");
                          for(t=0; t<index; t++)
                            moveCursorRight();
                        }
                        break;
                case 13: return;
                case 127:   break;
                default:
                  line[index] = c;
                  if(index < 63) { index++; printChar(c); }
              }
            }
          }
        } while(1) ; 
}
