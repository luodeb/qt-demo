# Makefile wrapper for qt-demo

# Default qmake path. You can override this by running: make QMAKE=/path/to/qmake
QMAKE ?= /opt/homebrew/opt/qt@5/bin/qmake

# Build directory
BUILD_DIR := build
PRO_FILE := qt-demo.pro

# The application bundle path (macOS)
TARGET_APP := $(BUILD_DIR)/qt-demo.app

.PHONY: all clean distclean run

# Default target
all: $(BUILD_DIR)/Makefile
	@echo "Building project..."
	$(MAKE) -C $(BUILD_DIR)

# Rule to generate the Makefile using qmake
$(BUILD_DIR)/Makefile: $(PRO_FILE)
	@echo "Running qmake..."
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && $(QMAKE) ../$(PRO_FILE)

# Clean build artifacts
clean:
	@echo "Removing build directory..."
	rm -rf $(BUILD_DIR)

# Run the application
run: all
	@echo "Running application..."
	open $(TARGET_APP)
