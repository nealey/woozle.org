TARGETS += $(DESTDIR)/track/index.html

$(DESTDIR)/track/.git:
	cd $(DESTDIR); git clone /home/neale/projects/track

$(DESTDIR)/track/index.html: $(DESTDIR)/track/.git
	cd $(DESTDIR)/track; git pull
