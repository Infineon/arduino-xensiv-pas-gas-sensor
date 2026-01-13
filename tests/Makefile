# Makefile for running unit tests and handling Arduino builds
# Variables for project configuration
FQBN  ?=
PORT  ?=
TESTS ?=
UNITY_PATH ?= ./ifx_ardunity/Unity
BAUD_RATE ?= 115200
SERIAL ?=
Q?=@
ENABLE_SYNC ?= 0
SENSOR_TYPE ?= 
BOARD_TYPE ?=
.PHONY: print_args clean check_unity_path unity flash compile upload monitor

# Default target: Show help when no target is specified
.DEFAULT_GOAL := help

help:
	@echo "============================================================================================================"
	@echo "Makefile Usage Guide"
	@echo "============================================================================================================"
	@echo ""
	@echo "Usage:"
	@echo "  make [TARGET] [VARIABLES]"
	@echo ""
	@echo "Available Targets:"
	@echo "  help                     Show this help guide and documentation"
	@echo "  list_tests               List all available test targets"
	@echo "  print_args               Print current configuration variables for debugging"
	@echo "  clean                    Clean the build directory (removes all intermediate files)"
	@echo "  prepare_test_environment Prepare the environment by copying common, test-specific files"
	@echo "  compile                  Compile the sketch for the specified board"
	@echo "  upload                   Upload the compiled sketch to the board via the specified port"
	@echo "  flash                    Perform both 'compile' and 'upload' in one go"
	@echo "  monitor                  Open the serial monitor for the given board and port"
	@echo ""
	@echo "Test Targets:"
	@echo "  test_<test_name>         Runs a specific test by preparing the test environment, compiling, uploading the firmware,"
	@echo "                           and running it on the target board."
	@echo "                           Example: test_digitalio_single or test_wifi_sta"
	@echo ""
	@echo "Variables:"
	@echo "  FQBN=<board_name>        Fully Qualified Board Name (default: empty). Used to specify the microcontroller board."
	@echo "                           Example: infineon:psoc6:cy8ckit_062s2_ai"
	@echo ""
	@echo "  PORT=<serial_port>       Serial port to connect to the board (default: empty)."
	@echo "                           Examples: /dev/ttyUSB0"
	@echo ""
	@echo "  ENABLE_SYNC=<0|1>        Enable (1) or disable (0) synchronization for multi boards (default: 1)."
	@echo "                           This is primarily used in CI/CD HIL checks."
	@echo ""
	@echo "  BAUD_RATE=<rate>         Baud rate for serial communication (default: 115200)."
	@echo "                           Example: BAUD_RATE=9600"
	@echo ""
	@echo "  SERIAL=<serial_number>   Optional serial number for advanced port identification (default: empty)."
	@echo "                           Example: SERIAL=123456789ABC"
	@echo ""
	@echo "Examples:"
	@echo "  1. Prepare the test environment:"
	@echo "     make prepare_test_environment"
	@echo ""
	@echo "  2. Run a specific test for DigitalIO with board and port:"
	@echo "     make FQBN=infineon:psoc6:cy8ckit_062s2_ai PORT=/dev/ttyUSB0 test_digitalio_single"
	@echo ""
	@echo "  3. Run a test with synchronization disabled:"
	@echo "     make FQBN=infineon:psoc6:cy8ckit_062s2_ai PORT=/dev/ttyUSB0 test_digitalio_single ENABLE_SYNC=0"
	@echo ""
	@echo "  4. Open the serial monitor with a specific port:"
	@echo "     make PORT=/dev/ttyUSB0 monitor"
	@echo ""
	@echo "  5. Combine running a test and opening the serial monitor:"
	@echo "     make FQBN=infineon:psoc6:cy8ckit_062s2_ai PORT=/dev/ttyUSB0 test_digitalio_single monitor"
	@echo ""
	@echo "============================================================================================================"
	@echo ""



print_args:
#	@:
# Info for debugging
	$(info )
	$(info Test configuration)
	$(info ----------------------------------)
	$(info FQBN      	: $(FQBN))
	$(info UNITY_PATH  	: $(UNITY_PATH))
	$(info PORT      	: $(PORT))
	$(info SERIAL    	: $(SERIAL))
	$(info BAUD_RATE 	: $(BAUD_RATE))
	$(info SENSOR_TYPE  : $(SENSOR_TYPE))
	$(info BOARD_TYPE  	: $(BOARD_TYPE))
	$(info ----------------------------------)
	$(info )

# Clean and create build directory for arduino compilation
clean:
	$(Q) -rm -rf build/*
	$(Q) mkdir -p build

# Check if UNITY_PATH variable is set
check_unity_path:
ifndef UNITY_PATH
	$(error "Must set variable UNITY_PATH in order to be able to compile Arduino unit tests!")
endif

prepare_test_environment: print_args clean check_unity_path
	$(Q) find $(UNITY_PATH) -name '*.[hc]' \( -path '*extras*' -a -path '*src*' -or -path '*src*' -a \! -path '*example*' \) -exec \cp {} build \;
	$(Q) find ifx_ardunity -name '*.[hc]*' -a ! -path '*/Unity/*' -exec \cp {} build \;
	$(Q) cp -r ../src/corelib/*.h build/
	$(Q) cp test_main.ino build/build.ino 
	$(Q) cp ./test_xensiv_pas_gas_globals.h build/

# Test target example
test_%: prepare_test_environment
	$(Q) cp ./test_$*.cpp build/
	@if [ -z "$(TESTS)" ]; then \
        echo "Must set variable TESTS in order to be able to flash a specific arduino core test !"; \
        exit 1; \
    fi
	$(MAKE) flash TESTS=$(TESTS)

# XENSIV PAS GAS sensor tests
test_xensiv_pas_gas: TESTS=-DTEST_XENSIV_PAS_GAS
test_xensiv_pas_gas_r290: TESTS=-DTEST_XENSIV_PAS_GAS_R290
test_xensiv_pas_gas_a2l: TESTS=-DTEST_XENSIV_PAS_GAS_A2L


compile:
ifneq ($(SENSOR_TYPE),)
	$(eval SENSOR_TYPE_VAR=-DSENSOR_TYPE=$(SENSOR_TYPE))
endif
ifneq ($(BOARD_TYPE),)
	$(eval BOARD_TYPE_VAR=-DBOARD_TYPE=$(BOARD_TYPE))
endif

	echo "$(SENSOR_TYPE_VAR) $(BOARD_TYPE_VAR)"

ifeq ($(FQBN),)
	$(error "Must set variable FQBN in order to be able to compile Arduino sketches !")
else
	arduino-cli compile --clean --log --warnings all --fqbn $(FQBN) \
	                        --build-property compiler.c.extra_flags="\"-DUNITY_INCLUDE_CONFIG_H=1\"" \
							--build-property compiler.cpp.extra_flags="$(TESTS) $(SENSOR_TYPE_VAR) $(BOARD_TYPE_VAR) $(if $(filter 1,$(ENABLE_SYNC)),-DENABLE_SYNC,)" \
							--library ../ \
							build					
endif


upload:	compile
ifeq ($(PORT),)
	$(error "Must set variable PORT (Windows port naming convention, ie COM16) in order to be able to flash Arduino sketches !")
endif
ifeq ($(FQBN),)
	$(error "Must set variable FQBN in order to be able to flash Arduino sketches !")
else
ifeq ($(SERIAL),)
	arduino-cli upload --log --log-level info -v -p $(PORT) --fqbn $ $(FQBN) build
else
	arduino-cli upload --log --log-level info -v -p $(PORT) --fqbn $(FQBN) --upload-property upload.port.properties.serialNumber=$(SERIAL) build
endif
endif


flash: compile upload


monitor:
ifeq ($(PORT),)
	$(error "Must set variable PORT (Windows port naming convention, ie COM16) in order to be able to flash Arduino sketches !")
endif
	arduino-cli monitor -c baudrate=$(BAUD_RATE) -p $(PORT)
