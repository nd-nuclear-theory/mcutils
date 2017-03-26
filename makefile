################################
# project configuration
################################

libname = mcutils

# modules -- header-only
modules_h = arithmetic vector_tuple memoizer profiling eigen gsl

# modules -- header-plus-object 
modules_ho = parsing

# programs
programs = arithmetic_test eigen_test vector_tuple_test memoizer_test

# requires gsl...
programs += gsl_test

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
LDLIBS += -lam -lgsl

CXXFLAGS += -std=c++11
