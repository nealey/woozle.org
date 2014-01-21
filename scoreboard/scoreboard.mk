PLAIN += scoreboard
COPY += scoreboard/screenshot.png
COPY += scoreboard/logos.html
COPY += scoreboard/others.html

TARGETS += $(DESTDIR)/scoreboard/live/index.html

$(DESTDIR)/scoreboard/live/.git:
	cd $(DESTDIR)/scoreboard; git clone /home/neale/projects/scoreboard live

$(DESTDIR)/scoreboard/live/index.html: $(DESTDIR)/scoreboard/live/.git
$(DESTDIR)/scoreboard/live/index.html: /home/neale/projects/scoreboard
	cd $(DESTDIR)/scoreboard/live; git pull
