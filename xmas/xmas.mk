PLAIN += $(wildcard xmas/*)

XMAS_IMAGES = $(wildcard xmas/*/*.jpg)
XMAS_IMAGES_SC = $(patsubst %.jpg, %-sm.jpg, $(XMAS_IMAGES))
IMAGES += $(XMAS_IMAGES) $(XMAS_IMAGES_SC)
