PLAIN += derby
COPY += derby/scrimmage.pdf derby/lineup.pdf

$(DESTDIR)/derby/%.pdf: derby/%.ps
	ps2pdf $< $@
