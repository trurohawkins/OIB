TARGET = OIB

LIBDIR = lib/
INCDIR = include/

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

CFLAGS = -MMD -MP -I$(HELPERINC) -I$(MOLTNINC) -I$(INCDIR)
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
$(TARGET): $(INCDIR)OIB.h $(LIBDIR)libOIB.a $(MOLTNDIR)libMoltnCore.a $(HELPERDIR)libHelper.a  main.o  
	gcc main.o -o $@ $(LDFLAGS) $(LIBDIR)libOIB.a -L$(MOLTNLIB) -lMoltnCore -L$(HELPERLIB) -lHelper -lm

$(HELPERDIR)libHelper.a:
	$(MAKE) -C $(HELPERDIR)

$(MOLTNDIR)libMoltnCore.a:
	$(MAKE) -C $(MOLTNDIR)

# Static lib
$(LIBDIR)libOIB.a: renderFrame.o | $(LIBDIR)
	ar rs $@ $^

# Compiling
main.o: main.c
	gcc $(CFLAGS) -c main.c -o $@

renderFrame.o: renderFrame.c $(INCDIR)renderFrame.h
	gcc $(CFLAGS) -c renderFrame.c -o $@

$(LIBDIR):
	mkdir -p $(LIBDIR)

# tools
clean:
	rm -f *.o *.d

fclean:
	rm -f $(TARGET) *.o *.d $(LIBDIR)libOIB.a 

# merges .d files into dependency graph
-include *.d
