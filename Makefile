#
# Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0
#

include Makefile.inc
export TOP=$(shell pwd)
# Library and executable names
LIB=scmi_test
PROGRAM=scmi_test_agent
# Obtain PLAT command line argument and set platform directory
# However PLAT is an optional varible hence conditionally set these
ifneq ($(PLAT),)
	PLAT_DIR=platform/$(PLAT) platform/$(PLAT)/transport
	EXE=$(PROGRAM)
endif

# Obtain PROTOCOLS command line argument
comma := ,
ALL_PROTOCOLS=base $(subst $(comma), ,$(PROTOCOLS))
ALL_PROTOCOLS_UPPER=$(shell echo $(ALL_PROTOCOLS) | tr a-z A-Z)
# List of subdirs to be built
DIRS=test_agent $(PLAT_DIR) $(ALL_PROTOCOLS:%=protocols/%)
BUILD_ALL=$(DIRS)
# Name for directory within each subdir that contains its header files
export HEADER_DIR=include
# List of directories to be searched for header files
export I_DIRS=$(DIRS:%=-I$(TOP)/%/${HEADER_DIR}) -I$(TOP)/protocols/${HEADER_DIR} -I$(TOP)/platform/${HEADER_DIR}
# Output objects directory
export OBJ_DIR=$(TOP)/output
# Platform output objects directory
export PLATFORM_OBJ_DIR=$(TOP)/platform_output
# list of names to be converted as macros
export D_NAMES=$(ALL_PROTOCOLS_UPPER:%=-D%_PROTOCOL)
# Location of external library directories
LIB_DIR=$(TOP)

all: all_makefiles # to avoid overriding all target

all_makefiles: check_requirements $(BUILD_ALL) $(LIB) $(EXE)
	@echo "### Built project succesfully!!!###"

check_requirements:
	mkdir -p ${OBJ_DIR}
	mkdir -p ${PLATFORM_OBJ_DIR}

$(BUILD_ALL):
	@echo "Building sub-component at $(@)"
	$(MAKE) -C $(@)

$(PROGRAM):
	echo "Building executable '$@' at `pwd`"
	echo "$(CC) $(D_NAMES) $(CFLAGS) $(PLATFORM_OBJ_DIR)/*.o $(LDFLAGS) -L$(LIB_DIR) -l$(LIB) -o $@"
	$(CC) $(D_NAMES) $(CFLAGS) $(PLATFORM_OBJ_DIR)/*.o $(LDFLAGS) -L$(LIB_DIR) -l$(LIB) -o $@

$(LIB):
	echo "Building library at `pwd`"
	$(AR) -cvq lib$@.a $(OBJ_DIR)/*.o

clean: clean_all # to avoid overriding clean target

clean_all:
	echo "Cleaning at $(TOP)"
	rm -f lib*.a
	rm -f $(PROGRAM)
	rm -rf $(OBJ_DIR)
	rm -rf $(PLATFORM_OBJ_DIR)

.PHONY: check_requirements
.PHONY: all
.PHONY: $(DIRS)
.PHONY: clean
