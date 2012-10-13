#!/bin/bash
sudo kill `ps -ef | grep gmatch | grep -v grep | awk '{print $2}'`
