TARGETS += $(DESTDIR)/track/index.html

$(DESTDIR)/track/.git:
	cd $(DESTDIR); git clone /home/neale/projects/derby/track

$(DESTDIR)/track/index.html: $(DESTDIR)/track/.git
	cd $(DESTDIR)/track; git pull
