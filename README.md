# cmus-youtube
A patch that allows you to download and play youtube songs using cmus

# How to use ?
First , you have to download cmus code source , and build them .

    $ git clone https://github.com/cmus/cmus.git
    
then go to cmus directory . Open the file <b>command_mode.c</b> , and add the content of patch-r1.c .

add this

    { "youtube",		cmd_youtube,	1, 1, NULL,		  0, 0 },
    
after 

    { "wq",			cmd_quit,	0, 1, NULL,		  0, 0 },
    
save the command_mode.c file

and configure cmus .

List available optional features

    $ ./configure --help

Auto-detect everything

    $ ./configure

To disable some feature, arts for example, and install to $HOME run

    $ ./configure prefix=$HOME CONFIG_ARTS=n
    
Now open the Makefile and go to line <b>19</b>

    CFLAGS += -D_FILE_OFFSET_BITS=64
    
add this 
    
    -D_YOUTUBE_DL=1
    
then build the source using 

    $ make 
    
open cmus using 

    $ ./cmus
  
#Command Line
You can use cmus-youtube by using the command line : 

    :youtube <url>
    


