#!/bin/bash
service footygoat stop

for i in $*; do
   echo Building  $i
   rm $i
   make $i
   cp $i /usr/bin
 done
service footygoat start
