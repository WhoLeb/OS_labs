#!/bin/sh
rm output_file
gnome-terminal -- ./scr1.sh
gnome-terminal -- ./scr2.sh
gnome-terminal -- "tail -f output_file"
