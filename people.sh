#! /bin/sh

echo 'Title: People'
echo
echo 'The following people have made public homepages:'
echo

ls /home/*/public_html/index.html | while read fn; do
    a=${fn#/home/}
    u=${a%/public_html/index.html}
    l=/srv/www/woozle.org/~$u

    echo "* [$u](/~$u/)"
    [ -h $l ] || ln -s /home/$u/public_html $l
done
