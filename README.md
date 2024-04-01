
# DClockWidget
## Motivation

I made this app because I wanted to make a clock widget for personal use. It should come with a script to run it with my settings after log in. I'm working on the script but for now this is just a borderless digital clock window. Obviously there's a lot left to be done.

## Compiling
The code is written using SDL2 library. So you have to install it in order to compile it. After you've made sure you have SDL2 and can link it you can use Makefile of compile yourself. You can check which flags to use  with the following command:
>$ pkg-config --libs sdl2 

It should output something like 
> -lSDL2

After you've done it simply run
> $ make

or this command:
> $ gcc -lSDL2 -std=c99 ./src/*.c -o ./app

from DClockWidget directory. (You can change ./app to whatever name you like.)

## Usage
Run the program by running either
> ./app

or whatever name you gave the program at the previous step.
The program supports several flags. (Keep in ind that '-h' stands for height parameter and there for requires a numeric value to follow it). To get help you'll have to write '-\-help' or enter any unknown flag. 
| flag | long flag | description | value |
|------|-----------|---------------------------------------|-|
| -x | -\-pos_x| Window position coordinate X| integer |
| -y | -\-pos_y | Window position coordinate Y| integer|
| -w | -\-width | Window width | integer|
|-h| -\-height| Window height | integer
| \- | \-\-help | Show help | \-|
|\-|--img-src|Select a folder with digits and separator (must include 10 ".bmp" files named s.bmp and 0.bmp to 9.bmp | a valid folder path|
|-r| \-| Set value for red color of digits| integer from 0 to 255 |
|-g| \-| Set value for green color of digits| integer from 0 to 255 |
|-b| \-| Set value for blue color of digits| integer from 0 to 255 |
|-br| \-| Set value for background red color| integer from 0 to 255 |
|-bg| \-| Set value for background green color| integer from 0 to 255 |
|-bb| \-| Set value for background blue color| integer from 0 to 255 |


Tip: It is generally a good idea to set x:y ratio to about 7:1. Otherwise the window looks hella funny =-]

## Launcher
To run this widget you can use launcher:
| Command                                     | Result                          |
|:-------------------------------------------:|:-------------------------------:|
|```./DClockWidget.sh kill```                 |Kills existing instance of widget|
|```./DClockWidget.sh run```                  |Runs new instance of widget      |
|``` ./DClockWidget.sh <any_other_argument>```| Prints its usage                |

Launcher works with ```launch-cfg.yml```, so you can modify some parameters described [above](#Usage).