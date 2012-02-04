PLAIN += .
COPY += icon.png style.css style-black.css lists.cgi wishlist.cgi set.cgi $(TEMPLATE)
COPY += mail.cgi
HTML += people.html

$(DESTDIR)/people.html: people.sh template.html.m4
	sh $< | $(MDWNTOHTML) > $@

$(DESTDIR)/mail.cgi: mail.cgi.c
	$(CC) -Wall -Werror -o $@ $<
