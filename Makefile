TARGET = hyprlooks.so

# Auto-discover all .cpp and .hpp files under src/
SRC := $(shell find src -name '*.cpp')
HDR := $(shell find src -name '*.hpp')

CXXFLAGS += -shared -fPIC --no-gnu-unique -std=c++26 -Wall -O2

# Default: build against installed Hyprland headers (hyprland.pc).
# To build against a source checkout instead:
#   make HYPRLAND_HEADERS=/path/to/Hyprland
ifdef HYPRLAND_HEADERS
INCLUDES = -I$(HYPRLAND_HEADERS) -I$(HYPRLAND_HEADERS)/src -I$(HYPRLAND_HEADERS)/protocols \
	`pkg-config --cflags hyprlang hyprutils hyprcursor hyprgraphics aquamarine pixman-1 libdrm pangocairo libinput libudev wayland-server xkbcommon`
else
INCLUDES = `pkg-config --cflags hyprland pixman-1 libdrm pangocairo libinput libudev wayland-server xkbcommon`
endif

all: $(TARGET)

$(TARGET): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
