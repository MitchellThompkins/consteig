# get the normalized current directory
THIS_DIR := $(shell pwd)

UID=$(shell id -u)
GID=$(shell id -g)

# Default to a Debug build. If you want to enable debugging flags, run
# "make BUILD_TYPE=Release"
BUILD_TYPE ?= Debug
ifneq "$(BUILD_TYPE)" "Debug"
    ifneq "$(BUILD_TYPE)" "Release"
        $(error Bad BUILD_TYPE value "$(BUILD_TYPE)" please use "Debug" or "Release")
    endif
endif
BUILD_TYPE_LOWER := $(shell echo $(BUILD_TYPE) | tr A-Z a-z)


# Figure out where to build the software. Use BUILD_PREFIX if it was passed in.
BUILD_PREFIX ?= $(THIS_DIR)/build/$(BUILD_TYPE_LOWER)

BUILD_TOOL ?= make

BUILD_FILE = Makefile
CMAKE_GENERATOR = "Unix Makefiles"

# Get number of jobs Make is being called with. This only works with '-j' and not --jobs'
BUILD_TOOL_PID := $(shell echo $$PPID)
JOB_FLAG := $(filter -j%, $(subst -j ,-j,$(shell ps T | grep "^\s*$(BUILD_TOOL_PID).*$(BUILD_TOOL)")))

INSTALL_PREFIX ?= $(shell echo $(THIS_DIR)/build/$(BUILD_TYPE_LOWER)/install )

ifeq "$(CMAKE_OPTIONS)" ""
    CMAKE_OPTIONS := -G $(CMAKE_GENERATOR) -DCLANG_TIDY_FIX=$(CLANG_TIDY_FIX) -DCMAKE_INSTALL_PREFIX=$(INSTALL_PREFIX) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
else
   # force cmake to be re-run if we change the cmake options
   $(shell rm $(BUILD_PREFIX)/$(BUILD_FILE))
endif

.PHONY: build
build: $(BUILD_PREFIX)/$(BUILD_FILE)
	@set -o xtrace; \
	export CTEST_OUTPUT_ON_FAILURE=1; \
	cmake --build $(BUILD_PREFIX) --target all -- $(JOB_FLAG) ${a}; \

.PHONY: h
h:
	@echo 'make [OPTIONS...] [TARGETS...]'
	@echo
	@echo 'TARGETS:'
	@echo
	@echo '<any>'
	@echo '    Any targets generated by CMake, e.g. all, test'
	@echo
	@echo 'remove'
	@echo '    delete the build directory'
	@echo
	@echo 'format'
	@echo '    runs clang-format on .h/.hpp and .c/.cpp files'
	@echo
	@echo 'test'
	@echo '    runs unit tests with gtest'
	@echo
	@echo 'OPTIONS:'
	@echo
	@echo 'BUILD_TYPE=<Debug|Release>'
	@echo '    specifies the CMake build type, and the build subdirectory'
	@echo '    default: Debug'
	@echo
	@echo 'INSTALL_PREFIX=<path>'
	@echo '    Specifies the CMAKE_INSTALL_PREFIX CMake variable value'
	@echo '    default when BUILD_TYPE is Debug: ./build/debug/install'
	@echo '    default when BUILD_TYPE is Release: ./build/release/install'
	@echo
	@echo '-j <jobs>'
	@echo '    <jobs> pass -j flag to underlying BUILD_TOOL to set the job number'

.PHONY: format
format:
	find . -iname *.hpp -o -iname *.cpp -o -iname *.h -o -iname *.c | xargs clang-format -i --style=WebKit

.PHONY: remove
remove:
	rm -rf build/

$(BUILD_PREFIX)/$(BUILD_FILE):
	# create the temporary build directory if needed
	mkdir -p $(BUILD_PREFIX)
	touch -c $@
	# run CMake to generate and configure the build scripts
	ln -sf $(BUILD_PREFIX)/compile_commands.json compile_commands.json && \
	git config core.hooksPath .githooks && \
	cd $(BUILD_PREFIX) && \
	cmake ../.. $(CMAKE_OPTIONS); \

# Other (custom) targets are passed through to the cmake-generated $(BUILD_FILE)
# Note: when no targets are passed from the commanding the special variable $@
# is "Makefile" so we need to angle this odd case differently.
%: $(BUILD_PREFIX)/$(BUILD_FILE)
	@set -o xtrace; \
	export CTEST_OUTPUT_ON_FAILURE=1; \
	cmake --build $(BUILD_PREFIX) --target $@ -- $(JOB_FLAG) ${a}; \

# All the Makefiles read themselves get matched if a target exists for them, so
# they will get matched by a Match anything target %:. This target is here
# to prevent the %: Match-anything target from matching, and do nothing.
Makefile:
	;

.PHONY: cmd
cmd: $(BUILD_PREFIX)/$(BUILD_FILE)
	export CTEST_OUTPUT_ON_FAILURE=1; \
	cd $(BUILD_PREFIX); \
	${a};

.PHONY: container.build
container.build:
	docker build --file Dockerfile --tag consteig_dev_image .

.PHONY: container.pull
container.pull:
	docker pull ghcr.io/mitchellthompkins/consteig_dev_image:latest

.PHONY: container.start
container.start:
	docker-compose -f docker-compose.yml run --rm dev_env 'sh -x'

container.make.%:
	docker-compose -f docker-compose.yml run --rm dev_env 'make $*'
