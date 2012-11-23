#!/bin/bash

for i in $*; do
   cat /var/log/footygoat/$i.logs
 done

