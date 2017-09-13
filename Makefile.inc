export CC      := $(CROSS_COMPILE)gcc
CFLAGS  += -static

LD      := $(CROSS_COMPILE)ld
LDFLAGS +=

export AR      := $(CROSS_COMPILE)ar
ARFLAGS +=

.SILENT :
