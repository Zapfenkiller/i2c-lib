REMOVE = rm -f
HERE = ./
LIBSRC = ../lib-i2c/source/


all: wipe-lib twi usi wipe1 wipe2 wipe-lib2
	@echo
	@echo No EEPROM content
	$(REMOVE) $(HERE)*.eep


debug: clean twi usi wipe1


twi:
	make -f Makefile_m168


usi:
	make -f Makefile_t2313


clean: wipe-lib wipe1 wipe2
	@echo
	@echo clean:
	$(REMOVE) $(HERE)*.eep
	$(REMOVE) $(HERE)*.elf
	$(REMOVE) $(HERE)*.hex


wipe1:
	@echo
	@echo wipe1:
	$(REMOVE) $(HERE)*.d
	$(REMOVE) $(HERE)*.o
	$(REMOVE) $(HERE)*.s


wipe2:
	@echo
	@echo wipe2:
	$(REMOVE) $(HERE)*.a90
	$(REMOVE) $(HERE)*.cof
	$(REMOVE) $(HERE)*.lnk
	$(REMOVE) $(HERE)*.lss
	$(REMOVE) $(HERE)*.lst
	$(REMOVE) $(HERE)*.map
	$(REMOVE) $(HERE)*.obj
	$(REMOVE) $(HERE)*.sym


wipe-lib wipe-lib2:
	@echo
	@echo wipe-lib:
	$(REMOVE) $(LIBSRC)*.d
	$(REMOVE) $(LIBSRC)*.o
	$(REMOVE) $(LIBSRC)*.s
	$(REMOVE) $(LIBSRC)*.lst


.PHONY: all debug twi usi clean wipe1 wipe2 wipe-lib wipe-lib2