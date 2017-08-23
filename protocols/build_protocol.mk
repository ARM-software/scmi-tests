#
# Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0
#

C_FILES := $(wildcard *.c)
H_FILES := $(wildcard $(HEADER_DIR)/*.h)
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(C_FILES))

all: all_protocols

all_protocols: $(OBJS)
	@echo "### Finished building with makefile at `pwd` ###"

$(OBJ_DIR)/%.o: %.c $(H_FILES)
	@echo "$(CC) $(D_NAMES) $(CFLAGS) $(I_DIRS) -c $< -o $@"
	$(CC) $(D_NAMES) $(CFLAGS) $(I_DIRS) -c $< -o $@

clean: clean_protocols

clean_protocols:

.PHONY: clean