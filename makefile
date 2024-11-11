all:
	$(MAKE) -C cpu
	$(MAKE) -C assembler
	$(MAKE) -C screen
	