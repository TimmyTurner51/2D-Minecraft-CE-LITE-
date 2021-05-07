NAME ?= MC2DCE
ICON ?= icon.png
DESCRIPTION ?= "2D Minecraft CE (C) DEV_ALPHA v1.0.04"
COMPRESSED ?= Yes
ARCHIVED ?= NO

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk

