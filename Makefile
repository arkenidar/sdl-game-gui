# SDL2 Game GUI with buttons and text rendering

# Requirements : compilation requires SDL2 and SDL2_ttf development libraries .

# On Windows with MSYS2 + MinGW64: pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf --needed
# On Debian/Ubuntu: sudo apt install libsdl2-dev libsdl2-ttf-dev
# On Fedora: sudo dnf install SDL2-devel SDL2_ttf-devel
# On Arch Linux: sudo pacman -S sdl2 sdl2_ttf
# On macOS: brew install sdl2 sdl2_ttf

build:
	g++ -o sdl-game-gui sdl-game-gui.cpp $(shell pkg-config --cflags --libs SDL2_ttf)