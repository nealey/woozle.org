#! /bin/sh

echo 'Title: People'
echo

ls /home/*/public_html/index.html | while read fn; do
    a=${fn#/home/}
    u=${a%/public_html/index.html}

    echo "* [$u](/~$u)"
done
