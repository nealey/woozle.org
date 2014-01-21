TARGETS += $(DESTDIR)/scoreboard/live/index.html

$(DESTDIR)/scoreboard/live/.git:
	cd $(DESTDIR); git clone /home/neale/projects/scoreboard

$(DESTDIR)/scoreboard/live/index.html: $(DESTDIR)/scoreboard/live/.git
$(DESTDIR)/scoreboard/live/index.html: /home/neale/projects/scoreboard
	cd $(DESTDIR)/scoreboard/live; git pull
