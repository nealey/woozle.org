PLAIN += scoreboard
COPY += scoreboard/screenshot.png
COPY += scoreboard/logos.html
COPY += scoreboard/others.html
COPY += scoreboard/other-browsers.png
COPY += scoreboard/chrome.png

TARGETS += $(DESTDIR)/scoreboard/live/index.html

$(DESTDIR)/scoreboard/live/.git:
	cd $(DESTDIR)/scoreboard; git clone /home/neale/projects/derby/scoreboard live

$(DESTDIR)/scoreboard/live/index.html: $(DESTDIR)/scoreboard/live/.git
$(DESTDIR)/scoreboard/live/index.html: /home/neale/projects/derby/scoreboard
	cd $(DESTDIR)/scoreboard/live; git pull
