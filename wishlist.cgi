#! /usr/bin/python

import cgitb; cgitb.enable()
import cgi
import glob
import os
import sys
import markdown

BASE = '/var/lib/wishlists'

f = cgi.FieldStorage()

title = 'Wishlists'
content = []

u = f.getfirst('u')
p = f.getfirst('p')

if u:
    if p:
        if p.lower() not in ('dingo', 'jada'):
            content.append("<p>I'm sorry but that is not the right answer.</p>")
        else:
            txt = f.getfirst('txt')
            open(os.path.join(BASE, u), 'w').write(txt)
            content.append('<p>Okay, thanks!</p>')
            content.append('<p><a href="wishlist.cgi">Back to wishlists</a></p>')
    else:
        title = "%s's Wishlist" % cgi.escape(u)
        txt = open(os.path.join(BASE, u)).read()
        content.append('<form action="wishlist.cgi" method="post" accept-charset="utf-8">')
        content.append('<input type="hidden" name="u" value="%s">' % u)
        content.append('<textarea name="txt" rows="15" cols="60">%s</textarea>' % cgi.escape(txt))
        content.append('<br>')
        content.append('What is the name of Amy and Neale\'s dog?')
        content.append('<input name="p">')
        content.append('<input type="submit" value="update">')
        content.append('</form>')
        content.append('<p>Formatting overview:</p>')
        content.append('<pre>* Item')
        content.append('* Second item')
        content.append('* Item with [a link](http://example.com/)')
        content.append('* &lt;del&gt;A spoken-for item&lt;/del&gt;</pre>')
else:
    for fn in sorted(glob.glob(os.path.join(BASE, '*'))):
        u = os.path.basename(fn)
        content.append("<h1>%s</h1>" % cgi.escape(u))
        content.append(markdown.markdown(open(fn).read()))
        content.append('<p><a href="wishlist.cgi?u=%s">edit</a></p>' % u)

print 'Content-type: text/html'
print
sys.stdout.flush()
p = os.popen('m4 -DTITLE="%s" template.html.m4 -' % title, 'w')
p.write('\n'.join(content))
p.close()
