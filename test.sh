#!/bin/bash

if [ "$1" = "1" -o "$1" = "0" ]
then
    ./generate ../ext_calib/images/C1.jpg ../ext_calib/ext_calib1.xml ../int_calib/calib_gopro3.xml 0 1 ./constant.txt > trash
fi

if [ "$1" = "2" -o "$1" = "0" ]
then
    ./generate ../ext_calib/images/C2.jpg ../ext_calib/ext_calib2.xml ../int_calib/calib_gopro3.xml 1 1 ./constant.txt > trash
fi

if [ "$1" = "3" -o "$1" = "0" ]
then
    ./generate ../ext_calib/images/C3.jpg ../ext_calib/ext_calib3.xml ../int_calib/calib_gopro3.xml 2 1 ./constant.txt > trash
fi

if [ "$1" = "4" -o "$1" = "0" ]
then
    ./generate ../ext_calib/images/C4.jpg ../ext_calib/ext_calib4.xml ../int_calib/calib_gopro3.xml 3 1 ./constant.txt > trash
fi

if [ "$1" = "5" -o "$1" = "0" ]
then
    ./generate ../ext_calib/images/C5.jpg ../ext_calib/ext_calib5.xml ../int_calib/calib_gopro4.xml 4 1 ./constant.txt > trash
fi

if [ "$1" = "6" -o "$1" = "0" ]
then
    ./generate ../ext_calib/images/C6.jpg ../ext_calib/ext_calib6.xml ../int_calib/calib_gopro4.xml 5 1 ./constant.txt > trash
fi

if [ "$1" = "7" -o "$1" = "0" ]
then
    ./generate ../ext_calib/images/C7.jpg ../ext_calib/ext_calib7.xml ../int_calib/calib_gopro4.xml 6 1 ./constant.txt > trash
fi