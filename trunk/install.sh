#!/bin/bash
#before install check DB setting in 
#	footygoat.conf 
#and run this with root

#CENTOS/REDHAT/FEDERA WEBBASE=/var/www/html APACHEUSER=apache 
WEBBASE=/var/www/
APACHEUSER=www-data
DBUSER=root
DBPASS=sql#2012

#try install tools
sudo apt-get update
sudo apt-get install flex g++ libmysql++-dev php5 apache2 mysql-server php5-mysql php5-gd php5-cli mono-gmcs libapache2-mod-php5 curl libcurl3 libcurl3-dev php5-curl

#sudo yum -y update
#sudo yum -y install php httpd php-mysql mysql-server php-xml php-gd gcc-c++  mysql-devel php-mbstring glibc-static


#compile and install the core
rm -f gtoday gtable gmatch ftrigger
make gmatch
make gtable
make gtoday
make ftrigger
sudo cp gmatch /usr/bin
sudo cp gtable /usr/bin
sudo cp gtoday /usr/bin
sudo cp ftrigger /usr/bin

#create work dir set default conf
sudo    mkdir /etc/footygoat
sudo cp footygoat.conf /etc/footygoat
sudo    mkdir /var/log/footygoat
sudo chmod -R 777 /var/log/footygoat


#boot up footygoat
sudo cp footygoat /etc/init.d/footygoat
sudo chmod +x  /etc/init.d/footygoat
sudo ln -s /etc/init.d/footygoat /etc/rc3.d/S93footygoat
sudo ln -s /etc/init.d/footygoat /etc/rc2.d/S93footygoat

sudo cp ftriggers /etc/init.d/ftriggers
sudo chmod +x  /etc/init.d/ftriggers
sudo ln -s /etc/init.d/ftriggers /etc/rc3.d/S93ftriggers
sudo ln -s /etc/init.d/ftriggers /etc/rc2.d/S93ftriggers
sudo /etc/init.d/footygoat start
