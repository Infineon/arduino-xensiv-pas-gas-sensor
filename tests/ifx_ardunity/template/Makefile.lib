FQBN  ?=
PORT  ?=
TESTS ?=
UNITY_PATH ?=./ifx_ardunity/Unity

# These vars are used for example-tests:
SENSOR_TYPE ?= 
BOARD_TYPE ?=

$(info FQBN  : $(FQBN))
$(info PORT  : $(PORT))
$(info TESTS : $(TESTS))
$(info UNITY_PATH : $(UNITY_PATH))
$(info SENSOR_TYPE  : $(SENSOR_TYPE))
$(info BOARD_TYPE  : $(BOARD_TYPE))



				
# XENSIV PAS GAS sensor tests
XENSIV_PAS_GAS: TESTS=-DTEST_XENSIV_PAS_GAS

XENSIV_PAS_GAS: unity flash


# $(EXAMPLES): arduino compile


### Arduino targets
clean:
	-rm -rf build/


arduino: clean
	mkdir build
# Copy all unity file needed for Arduino tests
	find ifx_ardunity -name '*.[hc]*' -not -path '*/Unity/*' -print -exec \cp {} build \;
# TODO: Copy test file needed 
	cp ./test_xensiv_pas_gas.cpp build/

unity: arduino
	find $(UNITY_PATH) -name '*.[hc]' \( -path '*extras*' -a -path '*src*' -or -path '*src*' -a \! -path '*example*' \) -exec \cp {} build \;
	cp test_main.ino build/build.ino


.ONESHELL:
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
							--build-property compiler.cpp.extra_flags="$(TESTS) $(SENSOR_TYPE_VAR) $(BOARD_TYPE_VAR)" \
							--library ../ \
			        build

# 	                        --build-property compiler.c.extra_flags="\"-DUNITY_INCLUDE_CONFIG_H=1\"" \
					
endif


upload:	
ifeq ($(PORT),)
	$(error "Must set variable PORT (Windows port naming convention, ie COM16) in order to be able to flash Arduino sketches !")
endif
ifeq ($(FQBN),)
	$(error "Must set variable FQBN in order to be able to flash Arduino sketches !")
else
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) build
endif


flash: compile upload


monitor:
ifeq ($(PORT),)
	$(error "Must set variable PORT (Windows port naming convention, ie COM16) in order to be able to flash Arduino sketches !")
endif
	arduino-cli monitor -c baudrate=115200 -p $(PORT) --fqbn $(FQBN)



# For WSL and Windows :
# download arduino-cli from : https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Windows_64bit.zip
prepare:
	arduino-cli core update-index
	arduino-cli core install Infineon:xmc
	arduino-cli core update-index
	arduino-cli core search Infineon
	arduino-cli core list
	arduino-cli board listall
	arduino-cli board listall Infineon
