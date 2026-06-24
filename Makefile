TARGET = OIB

DEV_CFLAGS = -g -fsanitize=address,undefined -fno-omit-frame-pointer
DEV_LDFLAGS = -fsanitize=address,undefined

TSAN_CFLAGS = -g -O1 -fsanitize=thread -fno-omit-frame-pointer
TSAN_LDFLAGS = -fsanitize=thread

PROD_CFLAGS = -O2
PROD_LDFLAGS =

CFLAGS = -MMD -MP
LDFLAGS =

dev: CFLAGS += $(DEV_CFLAGS)
dev: LDFLAGS += $(DEV_LDFLAGS)
dev: $(TARGET)

tsan: CFLAGS += $(TSAN_CFLAGS)
tsan: LDFLAGS += $(TSAN_LDFLAGS)
tsan: $(TARGET)

prod: CFLAGS += $(PROD_CFLAGS)
prod: LDFLAGS += $(PROD_LDFLAGS)
prod: $(TARGET)

MOLTNDIR = ../MOLTN/

# Linking
$(TARGET): OIB.h libOIB.a libMoltnCore.a libHelper.a  main.o  
	gcc main.o -o $@ $(LDFLAGS) libOIB.a libMoltnCore.a libHelper.a -lm

libHelper.a:
	$(MAKE) -C $(MOLTNDIR)
	cp $(MOLTNDIR)libHelper.a .

libMoltnCore.a:
	$(MAKE) -C $(MOLTNDIR)
	cp $(MOLTNDIR)libMoltnCore.a .

MoltnCore.h:
	$(MAKE) -C $(MOLTNDIR)
	cp $(MOLTNDIR)MoltnCore.h .

OIB.h: MoltnCore.h inpEvent.h
	@echo "Generating inputouput header"
	@echo "#pragma once" > OIB.h
	@cat MoltnCore.h renderFrame.h inpEvent.h >> OIB.h

# Static lib
libOIB.a: renderFrame.o
	ar rs $@ $^

# Compiling
main.o: main.c
	gcc $(CFLAGS) -c main.c -o $@

renderFrame.o: renderFrame.c renderFrame.h
	gcc $(CFLAGS) -c renderFrame.c -o $@

# tools
clean:
	rm -f *.o *.a *.d

fclean:
	rm -f $(TARGET) *.o *.a *.d MoltnCore.h OIB.h

fixTerminal:
	stty sane

# merges .d files into dependency graph
-include *.d
