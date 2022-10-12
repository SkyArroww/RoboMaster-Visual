#!/bin/ba sh 
time find Test/ -name ".c" -o -name ".h" -type f | xargs grep hello -n

