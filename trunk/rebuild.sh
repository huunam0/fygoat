#!/bin/bash
service footygoat status
service footygoat stop
ps -ef | grep gmatch
kill `ps -ef | grep gmatch | grep -v grep | awk '{print $2}'`
for i in $*; do
   echo Building  $i
   rm $i
   make $i
   cp $i /usr/bin
 done
service footygoat start
