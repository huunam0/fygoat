#!/bin/bash
#Share link of wp's post onto fb - Tran Huu Nam - huunam0@gmail.com
user=root
password=G0at123
pageid=308602152532791
db=footygoat
token=`mysql $db -u$user -p$password -s -N -e "SELECT option_value FROM footygoat.wp_options WHERE option_name='fb-page-act' limit 1"`
while read -a pid
do
    #echo $pid
    ketqua=`curl -ss -X POST -F link=http://www.footygoat.com/?p=$pid -F access_token=$token https://graph.facebook.com/$pageid/feed`
    if [[ $ketqua == *id* ]]
    then
      mysql $db -u$user -p$password -s -N -e "INSERT INTO wp_postmeta (post_id,meta_key,meta_value) VALUE ($pid,'posted2fb',NOW())"
    fi
    echo $pid, $ketqua, `date` >> /var/log/footygoat/wp2fb.log
    sleep 1
done < <(echo "SELECT ID FROM footygoat.wp_posts where (ID>803) and (post_type='post') and (post_status='publish') and (ID not in (select post_id from wp_postmeta where meta_key='posted2fb'))" | mysql $db  -u$user -p$password -N)
