PLAIN += .
COPY += icon.png style.css style-black.css lists.cgi wishlist.cgi set.cgi $(TEMPLATE)
COPY += mail.cgi ilohamail.cgi l.cgi
COPY += google7f698b9893809122.html
HTML += people.html
COPY += robots.txt
COPY += derbygirl.png

$(DESTDIR)/people.html: people.sh template.html.m4
	sh $< | $(MDWNTOHTML) > $@

$(DESTDIR)/%.cgi: %.cgi.c
	$(CC) -Wall -Werror -o $@ $<
