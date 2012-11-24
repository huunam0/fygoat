#!/bin/bash

for i in $*; do
   cat /var/log/footygoat/$i.log
 done

