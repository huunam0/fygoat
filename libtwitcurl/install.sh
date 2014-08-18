#!/bin/bash
#install twicurl library


sudo apt-get install libcurl4-gnutls-dev
sudo make
sudo cp libtwitcurl.so.1.0 /usr/lib/libtwitcurl.so.1
sudo ln -s /usr/lib/libtwitcurl.so.1 /usr/lib/libtwitcurl.so
sudo export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib
