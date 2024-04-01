#!/bin/bash
# This is another launcher for DClockWidget.

#Getting all arguments from yaml config file.

getArgs () {
    arg=$(cat launch-cfg.yml | grep $1)
    arg=${arg:$2:$(expr ${#arg} - $2)}
    echo $arg
}

if [ "$1" = "kill" ]; then
    kill -s 9 $(pidof app)
    exit 0
else

pos_x=$(getArgs 'pos_x:' 7)
pos_y=$(getArgs 'pos_y:' 7)

bg_red=$(getArgs 'bg_red:' 8)
bg_green=$(getArgs 'bg_green:' 10)
bg_blue=$(getArgs 'bg_blue:' 9)

fg_red=$(getArgs 'fg_red:' 8)
fg_green=$(getArgs 'fg_green:' 10)
fg_blue=$(getArgs 'fg_blue:' 9)

#Starting the DClockWidget in the background.
./app -x $pos_x -y $pos_y -br $bg_red -bg $bg_green -bb $bg_blue -r $fg_red -g $fg_green -b $fg_blue &

#Waiting until SDL app starts.
sleep 1

#Pinning DClockWidget to Desktop.
wmctrl -r 'Clock Widget' -b toggle,below

#Removing DClockWidget from taskbar (aka Panel)
wmctrl -r 'Clock Widget' -b add,skip_taskbar

fi