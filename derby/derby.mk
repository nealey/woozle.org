PLAIN += derby
COPY += derby/scrimmage.pdf

$(DESTDIR)/derby/scrimmage.pdf: derby/scrimmage.ps
	ps2pdf $< $@
