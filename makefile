################################
# project configuration
################################

libname = mcutils

# modules -- header-only
modules_h = arithmetic vector_tuple memoizer profiling eigen

# modules -- header-plus-object 
modules_ho = parsing

# programs
programs = arithmetic_test vector_tuple_test memoizer_test
CC := $(CXX)

################################
# common definitions
################################

COMMON_MAKE_DIR ?= .
include $(COMMON_MAKE_DIR)/common.mk

################################
# special dependencies
################################

# program linking
CC := $(CXX)

# external libraries
LDFLAGS += -L../am
LDLIBS += -lam

CXXFLAGS += -std=c++11
