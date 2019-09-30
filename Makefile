#/** @file
# * Copyright (c) 2019, Arm Limited or its affiliates. All rights reserved.
# * SPDX-License-Identifier : Apache-2.0
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *  http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
#**/

include Makefile.inc
export TOP=$(shell pwd)

# Library and executable names
LIB=scmi_test
PROGRAM=scmi_test_agent

# Default version and verbose setting
VERBOSE := 3
VERSION := 1

# Obtain PLAT command line argument and set platform directory
# However PLAT is an optional variable hence conditionally set these
ifneq ($(PLAT),)
	PLAT_DIR=platform/$(PLAT)
	EXE=$(PROGRAM)
endif

APP_DIR=linux_app

# Obtain PROTOCOLS command line argument
comma := ,
ALL_PROTOCOLS=base common $(subst $(comma), ,$(PROTOCOLS))
ALL_PROTOCOLS_UPPER=$(shell echo $(ALL_PROTOCOLS) | tr a-z A-Z)

# List of subdirs to be built
DIRS=val $(PLAT_DIR) $(ALL_PROTOCOLS:%=test_pool/%) $(APP_DIR)
BUILD_ALL=$(DIRS)

# Name for directory within each subdir that contains its header files
export HEADER_DIR=include

# List of directories to be searched for header files
export I_DIRS=$(DIRS:%=-I$(TOP)/%/${HEADER_DIR}) -I$(TOP)/test_pool/${HEADER_DIR} -I$(TOP)/platform/${HEADER_DIR}

# Output objects directory
export APP_OBJ_DIR=$(TOP)/app_output

# Test output objects directory
export TEST_OBJ_DIR=$(TOP)/test_output

# Platform output objects directory
export PLATFORM_OBJ_DIR=$(TOP)/platform_output

# Val output objects directory
export VAL_OBJ_DIR=$(TOP)/val_output

# list of names to be converted as macros
export D_NAMES=$(ALL_PROTOCOLS_UPPER:%=-D%_PROTOCOL)
export CFLAGS=-DVERBOSE_LEVEL=$(VERBOSE) -Wall -Werror
export CFLAGS+=-DSCMI_VERSION_$(VERSION)

# Location of external library directories
LIB_DIR=$(TOP)

all: all_makefiles # to avoid overriding all target

all_makefiles: check_requirements $(BUILD_ALL) $(LIB) $(EXE)
	@echo "### Built project successfully!!!###"

check_requirements:
	mkdir -p ${APP_OBJ_DIR}
	mkdir -p ${TEST_OBJ_DIR}
	mkdir -p ${PLATFORM_OBJ_DIR}
	mkdir -p ${VAL_OBJ_DIR}

$(BUILD_ALL):
	@echo "Building sub-component at $(@)"
	$(MAKE) -C $(@)

$(PROGRAM):
	echo "Building executable '$@' at `pwd`"
	echo "$(CC) $(D_NAMES) $(CFLAGS) $(PLATFORM_OBJ_DIR)/*.o $(LDFLAGS) -L$(LIB_DIR) -l$(LIB) -o $@"
	$(CC) $(D_NAMES) $(CFLAGS) $(PLATFORM_OBJ_DIR)/*.o $(LDFLAGS) -L$(LIB_DIR) -l$(LIB) -o $@

$(LIB):
	echo "Building library at `pwd`"
	$(AR) -cvq lib$@.a $(VAL_OBJ_DIR)/*.o $(TEST_OBJ_DIR)/*.o  $(APP_OBJ_DIR)/*.o

help:
	@echo "### SUPPORTED PROTOCOLS : base power_domain system_domain performance clock sensor  ###"
	@echo "### SUPPORTED VERSION : 1 (SCMI v1.0 test) , 2 (SCMI v2.0 test)  ###"
	@echo "### SUPPORTED VERBOSE : 1 (ERR) 2 (WARN) 3 (TEST) 4 (DEBUG) 5 (INFO)   ###"

clean: clean_all # to avoid overriding clean target

clean_all:
	echo "Cleaning at $(TOP)"
	rm -f lib*.a
	rm -f $(PROGRAM)
	rm -rf $(APP_OBJ_DIR)
	rm -rf $(TEST_OBJ_DIR)
	rm -rf $(VAL_OBJ_DIR)
	rm -rf $(PLATFORM_OBJ_DIR)

.PHONY: check_requirements
.PHONY: all
.PHONY: $(DIRS)
.PHONY: clean
