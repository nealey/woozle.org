PLAIN += .
COPY += icon.png style.css style-black.css lists.cgi wishlist.cgi set.cgi $(TEMPLATE)
HTML += people.html

$(DESTDIR)/people.html: people.sh
	sh $< | $(MDWNTOHTML) > $@
