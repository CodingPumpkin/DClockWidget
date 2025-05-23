#!/bin/bash
# This is another launcher for DClockWidget.

#Getting all arguments from yaml config file.

getArgs () {
    arg=$(cat $3 | grep $1)
    arg=${arg:$2:$(expr ${#arg} - $2)}
    echo $arg
}

waitUntilWindowStarts() {
    local window_name="$1"
    local previous_id=""

    while true; do
        current_id=$(wmctrl -l | grep "$window_name" | awk '{print $1}')

        #Checking that the window ID has changed during the creation process
        if [ -n "$current_id" ] && [ "$current_id" != "$previous_id" ]; then
            #If this is not the first iteration, then the ID has changed
            if [ -n "$previous_id" ]; then
                waitUntilWindowStarts "$window_name"
            fi
        else
            #If the ID has stopped changing, the window has been created (finally jeez)
            if [ -n "$current_id" ] && [ "$current_id" == "$previous_id" ]; then
                break
            fi
        fi

        previous_id="$current_id"

        sleep 0.1
    done
}

killWidgetInstance() {
    if ps -o command cp $1 | grep $2 >/dev/null; then
        kill -s 9 $1 && echo "Process $1 successfully killed! ;)"
    fi
}

exec=$(getArgs "^exec " 7 ./Makefile)

if [ "$1" = "kill" ]; then
    PID=$(head -n 1 ./PID)
    rm ./PID
    killWidgetInstance $PID $exec
    exit 0
else

    if [ "$1" = "run" ]; then
        pos_x=$(getArgs 'pos_x:' 7 ./launch-cfg.yml)
        pos_y=$(getArgs 'pos_y:' 7 ./launch-cfg.yml)

        bg_red=$(getArgs 'bg_red:' 8 ./launch-cfg.yml)
        bg_green=$(getArgs 'bg_green:' 10 ./launch-cfg.yml)
        bg_blue=$(getArgs 'bg_blue:' 9 ./launch-cfg.yml)

        fg_red=$(getArgs 'fg_red:' 8 ./launch-cfg.yml)
        fg_green=$(getArgs 'fg_green:' 10 ./launch-cfg.yml)
        fg_blue=$(getArgs 'fg_blue:' 9 ./launch-cfg.yml)

        #Prevent multiple instances from starting
        if [ -f ./PID ]; then
            $0 kill
        fi

        #Starting the DClockWidget in the background.
        ./$exec -x $pos_x -y $pos_y -br $bg_red -bg $bg_green -bb $bg_blue -r $fg_red -g $fg_green -b $fg_blue &
        echo $! > ./PID

        waitUntilWindowStarts "Clock Widget"

        #Pinning DClockWidget to Desktop.
        wmctrl -r 'Clock Widget' -b toggle,below

        #Removing DClockWidget from taskbar (aka Panel)
        wmctrl -r 'Clock Widget' -b add,skip_taskbar
    else
        echo -e "Usage:"
        echo -e "$0 kill\tKills existing instance of widget"
        echo -e "$0 run\t\tRuns new instance of widget"
    fi
fi