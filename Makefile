TARGET      := $(word 1, $(MAKECMDGOALS))
COMMON_DIRS := $(addprefix ./modules/,common_utils parsers types nn_solvers)

LSH_DIRS  := ./src/lsh ./modules/nn_solvers/lsh $(COMMON_DIRS)
LSH_FILES := $(wildcard $(addsuffix /*,$(LSH_DIRS)))
LSH_INCS  := $(filter %.h,$(LSH_FILES))
LSH_SRCS  := $(filter %.cc,$(LSH_FILES))
LSH_OBJS  := $(subst .cc,.o,$(LSH_SRCS))

CUBE_DIRS  := ./src/cube ./modules/nn_solvers/cube $(COMMON_DIRS)
CUBE_FILES := $(wildcard $(addsuffix /*,$(CUBE_DIRS)))
CUBE_INCS  := $(filter %.h,$(CUBE_FILES))
CUBE_SRCS  := $(filter %.cc,$(CUBE_FILES))
CUBE_OBJS  := $(subst .cc,.o,$(CUBE_SRCS))

CLUSTER_DIRS  := ./src/cluster $(shell find ./modules -type d)
CLUSTER_FILES := $(wildcard $(addsuffix /*,$(CLUSTER_DIRS)))
CLUSTER_INCS  := $(filter %.h,$(CLUSTER_FILES))
CLUSTER_SRCS  := $(filter %.cc,$(CLUSTER_FILES))
CLUSTER_OBJS  := $(subst .cc,.o,$(CLUSTER_SRCS))

CXX = g++

ifeq ($(TARGET),lsh)
	CXXFLAGS := -std=c++11 -O2 -Wall $(addprefix -I,$(LSH_DIRS))
else ifeq ($(TARGET),cube)
	CXXFLAGS := -std=c++11 -O2 -Wall $(addprefix -I,$(CUBE_DIRS))
else
	CXXFLAGS := -std=c++11 -O2 -Wall $(addprefix -I,$(CLUSTER_DIRS))
endif

lsh: $(LSH_OBJS) $(LSH_INCS)
	@$(CXX) $(CXXFLAGS) $(LSH_OBJS) -o ./bin/lsh

cube: $(CUBE_OBJS) $(CUBE_INCS)
	@$(CXX) $(CXXFLAGS) $(CUBE_OBJS) -o ./bin/cube

cluster: $(CLUSTER_OBJS) $(CLUSTER_INCS)
	@$(CXX) $(CXXFLAGS) $(CLUSTER_OBJS) -o ./bin/cluster

.SILENT: $(LSH_OBJS) $(CUBE_OBJS) $(CLUSTER_OBJS)
%.o: %.cc
	@$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@echo "Cleaning up ..."
	@rm -f $(LSH_OBJS) $(CUBE_OBJS) $(CLUSTER_OBJS) ./bin/*
	@touch bin/.keep
