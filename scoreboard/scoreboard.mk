TARGETS += $(DESTDIR)/scoreboard/index.html

$(DESTDIR)/scoreboard/.git:
	cd $(DESTDIR); git clone /home/neale/projects/scoreboard

$(DESTDIR)/scoreboard/index.html: $(DESTDIR)/scoreboard/.git
$(DESTDIR)/scoreboard/index.html: /home/neale/projects/scoreboard
	cd $(DESTDIR)/scoreboard; git pull
