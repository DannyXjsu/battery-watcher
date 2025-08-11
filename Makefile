# tool macros
CC ?= gcc
CXX ?= g++
CFLAGS := -O2
CXXFLAGS := -O2
DBGFLAGS := -g
COBJFLAGS := $(CFLAGS) -c

SERVICE_FILE := battery-watcher.service
SERVICE_PATH := $(XDG_CONFIG_HOME)/systemd/user/

SH_SOUND_PLUG := sound_plug.sh
SH_SOUND_UNPLUG := sound_unplug.sh

# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src
DBG_PATH := debug

PREFIX_INSTALL := /opt

# compile macros
TARGET_NAME := battery-watcher
ifeq ($(OS),Windows_NT)
	TARGET_NAME := $(addsuffix .exe,$(TARGET_NAME))
endif
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
DISTCLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG)
CLEAN_LIST := $(TARGET) \
			  $(TARGET_DEBUG) \
			  $(DISTCLEAN_LIST)

# default rule
default: makedir all

# non-phony targets
$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(COBJFLAGS) -o $@ $<

$(DBG_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(COBJFLAGS) $(DBGFLAGS) -o $@ $<

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CC) $(CFLAGS) $(DBGFLAGS) $(OBJ_DEBUG) -o $@

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: install-service
install-service:
	@cp -v $(SERVICE_FILE) $(SERVICE_PATH)
	@systemctl --user daemon-reload

.PHONY: uninstall-service
uninstall-service:
	@rm -vri $(SERVICE_PATH)/$(SERVICE_FILE)

.PHONY: enable-service
enable-service:
	@systemctl --user enable --now battery-watcher

.PHONY: install
install:
	@mkdir -vp $(PREFIX_INSTALL)/$(TARGET_NAME)
	@install -v -m 755 $(TARGET) $(PREFIX_INSTALL)/$(TARGET_NAME)
	@install -v -m 755 $(SH_SOUND_PLUG) $(PREFIX_INSTALL)/$(TARGET_NAME)
	@install -v -m 755 $(SH_SOUND_UNPLUG) $(PREFIX_INSTALL)/$(TARGET_NAME)

.PHONY: uninstall
uninstall:
	@rm -vri $(PREFIX_INSTALL)/$(TARGET_NAME)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)
