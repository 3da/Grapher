#
# Makefile for `bsgui' - it just calls bmake
#

bsguimaker:
	bmake

.PHONY: clean
clean:
	make -f Makefile.bm clean

.PHONY: distclean
distclean:
	rm -fr bsgui
	rm -fr Makefile.win
	make -f Makefile.lib distclean
	rm -fr Makefile.lib
	make -f Makefile.bm distclean
	rm -fr Makefile.bm
.PHONY: install
install:
	make -f Makefile.bm install
.PHONY: uninstall
uninstall:
	make -f Makefile.bm uninstall
	rm -f Makefile.bm

