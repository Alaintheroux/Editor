Editor v0.23d
(C)2022 Alain Theroux
License: You can do what you want with this code but leave these three lines.

A simple VTTY editor that works with a simple terminal program such as Putty.
It is useful to add editor facility with your own program such as a shell or interpreter.
The section for load/save is left uncompleted as this is dependant of your system.
Serial default speed is 115Kbps. You should change the default in Putty(or other terminal emulator) fo backspace ^H instead of del.
The main loop() simply call the editor to show its fonctionnality.
The data bloc is not reinitialize all the time as this permit to edit last lines without saving.
I personnaly use this editor for my own tiny interpreter. This is another project.
It is not complete by itself as the purpose is to integrate it within another application.
It was based on the absumption that the file is handle by bloc of date of 1 KB. (16 lines x 64 chars).

The arrow-keys are code returned by a PC keyboard under Windows OS.
Select a line to edit by pressing <CR> or -> right-arrow. Press <CR> again to exit the line-editing.

This is the main screen of the editor:
Editor v0.2.3d    Bloc:0     Mode:*
  +--------------------------------------------------------------+
|>
        (x 16 lines)
  
  +--------------------------------------------------------------+
  
(c)opy (p)aste (i)nsert (x)delete (t)op (l)ast (4-arrows) <Bs> <Del> !
(N)ext-bloc (P)rev-bloc (B)loc (C)lear (R)efresh (L)oad (S)ave (Q)uit
  
The current selected line is show by ">". 
  c - copy the current line and save it in a internal buffer.
  p - paste the last copied line to the current cursor position (line).
  i - insert a line before the current line.
  x - delete the current line and move up the remaining line by one row.
  t - move the cursor to line 0
  l - move the cursor to the last line
  Up-arrow - move the cursor by one line
  Dn-arrow - move the cursor to the next line
  Right-arrow - select the current line. <CR> has the same effect.
  
  While editing a line, 
  Right-arrow - move the cursor to the next row.
  Left-arros  - move the cursor to the previous row.
  <Back Space> - erase the current character in line and move left the remaining of the line.
  <Del>       - Delete from current position to the end of line. Exit line edit
  <CR>        - Exit line edit. Move cursor to the next line.
    
