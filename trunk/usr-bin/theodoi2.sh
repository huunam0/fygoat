#!/bin/bash
#ps -ef|grep -c gtoday
sleep 9
c=`ps -ef | grep gtoday|grep -cv grep `
#echo $c
if  [ $c -lt 1 ]
then
        tail -20 /var/log/footygoat/today.log | mail -s "Footygoat stop alert" huunam0@gmail.com
        date >> /var/log/theodoi.log
        sudo kill `ps -ef | grep gmatch | grep -v grep | awk '{print $2}'`
        service footygoat start

#else
#       ps -ef | grep gtoday | grep -v grep >> /var/log/theodoi.log
fi
