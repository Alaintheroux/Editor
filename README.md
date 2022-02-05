# Editor v0.23d
# (C)2022 Alain Theroux
#License: You can do what you want with this code but leave these three lines.

A simple VTTY editor that works with a simple terminal program such as Putty.
It is useful to add editor facility with your own program such as a shell or interpreter.
The section for load/save is left uncompleted as this is dependant of your system.
Serial default speed is 115Kbps. You should change the default in Putty fo backspace ^H instead of del.
The main loop() simply call the editor to show its fonctionnality.
The data bloc is not reinitialize all the time as this permit to edit last lines without saving.
I personnaly use this editor for my own tiny interpreter. This is another project.
It is not complete by itself as the purpose is to integrate it within another application.

The arrow-keys are code returned by a PC keyboard under Windows OS.
There is still a small bug when using the "del" key. Will be fix shortly.
Select a line to edit by pressing <CR> or -> right-arrow. Press <CR> again to exit the line-editing.


