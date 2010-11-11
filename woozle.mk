PLAIN += .
COPY += icon.png style.css lists.cgi wishlist.cgi $(TEMPLATE)
HTML += people.html

$(DESTDIR)/people.html: people.sh
	sh $< | $(MDWNTOHTML) > $@
