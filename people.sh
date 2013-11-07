#! /bin/sh

echo 'Title: People'
echo
echo 'The following people have made public homepages:'
echo

ls /home/*/public_html/index.html | while read fn; do
    a=${fn#/home/}
    u=${a%/public_html/index.html}
    tl=/srv/http/woozle.org/~$u
    l=/srv/http/woozle.org/$u

    echo "* [$u](/~$u/)"
    for link in $tl $l; do
        [ -h $link ] || ln -s /home/$u/public_html $link
    done
done
