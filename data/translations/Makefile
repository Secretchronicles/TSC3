# Gather list of all source files for xgettext to process.
SRCFILES=$(shell find ../../src -iname '*.hpp' -or -iname '*.cpp')

TSC3.pot: $(SRCFILES)
	xgettext --output=$@ \
	--default-domain=TSC3 \
	--c++ \
	--from-code=UTF-8 \
	--add-comments='TRANS:' \
	--width=65 \
	--keyword=_ \
	--keyword=S_ \
	--keyword=PL_ \
	--keyword=C_ \
	--package-name=TSC3 \
	--msgid-bugs=tsc-devel@lists.secretchronicles.org $^

checkpo:
	msgfmt -o /dev/null -c --statistics *.po

%.po: TSC3.pot
	msgmerge -U $@ $<
