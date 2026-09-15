TARGET = OIB

LIBDIR = lib/
INCDIR = include/
SRCDIR = src/

HELPERDIR = ../HelperFuncs/
HELPERINC = $(HELPERDIR)include/
HELPERLIB = $(HELPERDIR)lib/

MOLTNDIR = ../MOLTN/
MOLTNINC = $(MOLTNDIR)include/
MOLTNLIB = $(MOLTNDIR)lib/

DEV_CFLAGS = -g -fsanitize=address,undefined -fno-omit-frame-pointer
DEV_LDFLAGS = -fsanitize=address,undefined

TSAN_CFLAGS = -g -O1 -fsanitize=thread -fno-omit-frame-pointer
TSAN_LDFLAGS = -fsanitize=thread

PROD_CFLAGS = -O2
PROD_LDFLAGS =

CFLAGS = -MMD -MP -I$(HELPERINC) -I$(MOLTNINC) -I$(INCDIR) -I$(SRCDIR)
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


# Linking
$(TARGET): $(INCDIR)OIB.h $(LIBDIR)libOIB.a $(MOLTNLIB)libMoltnCore.a $(MOLTNINC)MoltnCore.h $(HELPERLIB)libHelper.a  $(HELPERINC)helper.h main.o  
	gcc main.o -o $@ $(LDFLAGS) $(LIBDIR)libOIB.a -L$(MOLTNLIB) -lMoltnCore -L$(HELPERLIB) -lHelper -lm

$(HELPERLIB)libHelper.a:
	$(MAKE) -C $(HELPERDIR)

$(MOLTNLIB)libMoltnCore.a:
	$(MAKE) -C $(MOLTNDIR)

# Static lib
$(LIBDIR)libOIB.a: renderFrame.o color.o text.o | $(LIBDIR)
	ar rs $@ $^

# Compiling
main.o: main.c
	gcc $(CFLAGS) -c main.c -o $@

renderFrame.o: $(SRCDIR)renderFrame.c $(INCDIR)renderFrame.h
	gcc $(CFLAGS) -c $(SRCDIR)renderFrame.c -o $@

color.o: $(SRCDIR)color.c $(INCDIR)color.h
	gcc $(CFLAGS) -c $(SRCDIR)color.c -o $@

text.o: $(SRCDIR)text.c $(INCDIR)text.h
	gcc $(CFLAGS) -c $(SRCDIR)text.c -o $@

$(LIBDIR):
	mkdir -p $(LIBDIR)

# tools
clean:
	rm -f *.o *.d

fclean:
	rm -f $(TARGET) *.o *.d $(LIBDIR)libOIB.a 

# merges .d files into dependency graph
-include *.d
