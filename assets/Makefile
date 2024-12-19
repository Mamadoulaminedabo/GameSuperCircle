images = background.png difficulty.png easy.png hard.png medium.png play.png quit.png title.png
minified_images = $(patsubst %.png,%-small.png,$(images))

.PHONY: digits minify

digits:
	convert digits.png -scale 12.5% digits-small.png

minify: $(minified_images)

%-small.png: %.png
	convert $< -scale 50% $@
