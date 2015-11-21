CFLAGS   = -ansi -Wall
LIBS     = -lm
DEBUGDEF = NUMBERS PRIM CONSTANTS
OPTS     = -march=native -mtune=native -m64 -O3 -funroll-loops

SRC      = src/
OBJ      = obj/
OUT      = treegen

# list of object that will be compiling with optimazion flags
OPT_OBJS = tree.o neutral_rep.o
# and the auto-list of the remaining objects
OBJS = $(filter-out $(OPT_OBJS),$(patsubst %.c,%.o,$(subst $(SRC),,$(wildcard $(SRC)*.c))))


$(OUT): $(addprefix ${OBJ},$(OBJS)) $(addprefix ${OBJ},$(OPT_OBJS))
	gcc $(LIBS) ${OBJ}*.o -o $@


$(addprefix ${OBJ},$(OPT_OBJS)): OPT_OBJS_FLAGS := $(OPTS)
${OBJ}%.o: $(SRC)%.c
	@mkdir -p ${OBJ}
	gcc $(CFLAGS) $(OPT_OBJS_FLAGS) $(addprefix -D,$(DEBUG)) $(addprefix -D,$(DEBUGDEF)) -o $@ -c $<

clean:
	rm -f $(OUT)
	rm -fr ${OBJ}

force: clean ${OUT}

#
#   Templates
#

run: $(OUT)
	@echo
	@/usr/bin/time -f "%U user - %S system - %E elapsed - %P CPU" ./$(OUT)

tofile:
	@rm -fr data
	@mkdir -p data
	@make clean run DEBUG="TOFILE"

