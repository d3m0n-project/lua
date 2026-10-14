CC				= d3c
INCLUDE			= -I../../
PWD_BASENAME	= $(shell basename $(shell pwd))
OUT				= lib_$(PWD_BASENAME).o
C_FILE			= $(PWD_BASENAME).c
DEFINES			= -DLUA_32BITS

all: $(OUT)

$(OUT): $(C_FILE)
	@$(CC) $(INCLUDE) $(DEFINES) -c $^ -o $@
	@echo "$(shell pwd)"

clean:
	rm -rf $(OUT)

re: clean all

.PHONY: all clean re