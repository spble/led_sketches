PLATFORM := TEENSY31
BUILD_DIR := build
ARDUINO := arduino
ARDUINO_OPTS := --verify --pref build.path=$(BUILD_DIR)
LOADER := teensy-loader-cli
LOADER_OPTS := -v -s --mcu=$(PLATFORM) 

%.ino.hex: %.ino
	$(ARDUINO) $(ARDUINO_OPTS) $< 


upload: $(SKETCH)/$(SKETCH).ino.hex
	$(LOADER) $(LOADER_OPTS) $(BUILD_DIR)/$(<F)

