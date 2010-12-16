#! /bin/sh

echo 'Content-type: text/plain'
echo
if [ "$PATH_INFO" = "/moo" ]; then
    echo $REMOTE_ADDR > /usr/local/etc/nsd/home
fi
set
