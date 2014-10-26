#!/bin/bash
#ps -ef|grep -c gtoday
c=`ps -ef | grep gtoday|grep -cv grep `
#echo $c
if  [ $c -lt 1 ]
then
        #tail /var/log/footygoat/today.log | mail -s "Footygoat stop alert" huunam0@gmail.com
        date >> /var/log/theodoi.log
        sudo kill `ps -ef | grep gmatch | grep -v grep | awk '{print $2}'`
        service footygoat start
        /usr/bin/theodoi2.sh
#else
#       ps -ef | grep gtoday | grep -v grep >> /var/log/theodoi.log
fi
