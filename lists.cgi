#! /usr/bin/python

import cgitb; cgitb.enable()
import cgi
import os
import sys
import glob
import re
import smtplib

f = cgi.FieldStorage()

l = f.getfirst('l')
if not l:
    l = os.environ.get('PATH_INFO', '/')[1:]

desc_re = re.compile(r'List-Id: "?([^<]*)"? <')
def getdesc(d):
    fn = '%s/control/customheaders' % d
    try:
        hdrs = file(fn).read()
    except IOError:
        return '(none)'
    ret = desc_re.search(hdrs)
    if ret:
        return cgi.escape(ret.group(1)).replace('#', '')
    else:
        return '(none)'

listdir = '/var/spool/mlmmj/%s' % l
if l and os.path.isdir(os.path.join(listdir, 'control')):
    title = '%s membership' % l
    a = f.getfirst('a')
    addr = f.getfirst('addr')

    if ((a == 'subscribe') or (a == 'unsubscribe')) and addr:
        server = smtplib.SMTP('localhost')
        faddr = addr
        taddr = '%s-%s@woozle.org' % (l, a)
        try:
            server.sendmail(faddr, 
                            [taddr],
                            'From: %s\nTo: %s\n\n' % (faddr, taddr))
            content = '<p>I have sent a confirmation message to <tt>%s</tt>. ' % addr
            content += 'It should be in your mailbox shortly.</p>'
        except Exception, err:
            content = "<p>Uh oh.  That didn't work.</p>"
            content += "<pre>%s</pre>" % cgi.escape(str(err))
    else:
        content = '<h2>%s@woozle.org' % l
        content += ": %s" % getdesc(listdir)
        content += '</h2>'
        content += '<p>To subscribe to or unsubscribe from the %s list,' % l
        content += '   just enter your email address in this handy dandy form!</p>'
        content += '<form method="post" action="/lists.cgi">'
        content += '  <input type="hidden" name="l" value="%s" />' % l
        content += '  Email address: <input name="addr" />'
        content += '  <input type="submit" name="a" value="subscribe" />'
        content += '  <input type="submit" name="a" value="unsubscribe" />'
        content += '</form>'
        if os.path.exists(os.path.join(listdir, 'control', 'archive')):
            content += "  <p><a href='http://woozle.org/list-archives/%s/threads.html'>Message archive</a></p>" % l
else:
    title = 'Email lists'

    content = '<h2>Public email lists on this host</h2>'
    content += '<table>'
    content += '  <thead>'
    content += '    <tr>'
    content += '      <th>list</th>'
    content += '      <th>description</th>'
    content += '      <th>actions</th>'
    content += '    </tr>'
    content += '  </thead>'
    content += '  <tbody>'

    for d in sorted(glob.glob('/var/spool/mlmmj/*')):
        if os.path.islink(d):
            continue
        if os.path.exists('%s/control/private' % d):
            continue

        l = os.path.basename(d)
        content += '<tr><td><a href="/lists.cgi/%s">%s</a></td>' % (l, l)
        content += '<td>%s</td>' % getdesc(d)
        content += '<td><form action="/lists.cgi">'
        content += '  <input name="addr" />'
        content += '  <input type="hidden" name="l" value="%s" /><br />' % l
        content += '  <input type="submit" name="a" value="subscribe" />'
        content += '  <input type="submit" name="a" value="unsubscribe" />'
        content += '</form>'
        if os.path.exists('%s/control/archive' % d):
            content +=  '<a href="http://woozle.org/list-archives/%s/threads.html">view&nbsp;archive</a>' % l
        content += '</td></tr>\n'

    content += '  </tbody>'
    content += '</table>'

print 'Content-type: text/html'
print
sys.stdout.flush()
p = os.popen('m4 -DTITLE="%s" template.html.m4 -' % title, 'w')
p.write(content)
p.close()
