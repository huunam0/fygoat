#!/bin/bash
#before install check DB setting in 
#	footygoat.conf 
#and run this with root

#CENTOS/REDHAT/FEDERA WEBBASE=/var/www/html APACHEUSER=apache 
WEBBASE=/var/www/
APACHEUSER=www-data
DBUSER=root
DBPASS=usbw

#try install tools
sudo apt-get install flex g++ libmysql++-dev php5 apache2 mysql-server php5-mysql php5-gd php5-cli mono-gmcs libapache2-mod-php5

sudo yum -y update
sudo yum -y install php httpd php-mysql mysql-server php-xml php-gd gcc-c++  mysql-devel php-mbstring glibc-static


#compile and install the core
rm -f gtoday gtable gmatch
make gmatch
make gtable
make gtoday
sudo cp gmatch /usr/bin
sudo cp gtable /usr/bin
sudo cp gtoday /usr/bin

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
sudo /etc/init.d/footygoat start
