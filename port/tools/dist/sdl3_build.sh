#!/bin/sh
# Runs inside an i386 Debian 11 container (dist_linux.sh): SDL3 built from its release source for 32-bit x86 against
# glibc 2.31, so it loads on every distribution the port is for. Its backends (X11, Wayland, PulseAudio, PipeWire,
# ALSA, D-Bus, udev, Vulkan, OpenGL) are loaded when it runs, from the system or from the bundled runtime: nothing
# here becomes a hard dependency. In: /src/SDL3-$SDL_VERSION.tar.gz. Out: /out/sdl (prefix), owned by $OWNER.
set -eu
export DEBIAN_FRONTEND=noninteractive
# (Debian 11 has left the main mirrors for the archive)
cat >/etc/apt/sources.list <<EOF
deb http://archive.debian.org/debian bullseye main
deb http://archive.debian.org/debian bullseye-updates main
deb http://archive.debian.org/debian-security bullseye-security main
EOF
apt-get -o Acquire::Check-Valid-Until=false update -qq
apt-get install -y -qq --no-install-recommends build-essential cmake ninja-build pkg-config ca-certificates \
    libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxfixes-dev libxss-dev libxtst-dev \
    libxkbcommon-dev libwayland-dev wayland-protocols libegl-dev libgl-dev libgles-dev libdrm-dev libgbm-dev \
    libpulse-dev libasound2-dev libpipewire-0.3-dev libdbus-1-dev libudev-dev >/dev/null
rm -rf /tmp/sdl /tmp/sdl-build
mkdir -p /tmp/sdl
tar xzf "/src/SDL3-$SDL_VERSION.tar.gz" -C /tmp/sdl --strip-components=1
cmake -S /tmp/sdl -B /tmp/sdl-build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/out/sdl \
    -DSDL_SHARED=ON -DSDL_STATIC=OFF -DSDL_TESTS=OFF -DSDL_EXAMPLES=OFF -DSDL_INSTALL_DOCS=OFF \
    -DSDL_DEPS_SHARED=ON -DSDL_RPATH=OFF >/tmp/cmake.log || { tail -40 /tmp/cmake.log; exit 1; }
grep -E "^-- +(SDL_(X11|WAYLAND|PULSEAUDIO|PIPEWIRE|ALSA|DBUS|LIBUDEV|VULKAN|OPENGL|OPENGLES|GPU|HIDAPI)[A-Z_]*|  Video|  Audio)" \
    /tmp/cmake.log | head -40 || true
cmake --build /tmp/sdl-build >/tmp/build.log 2>&1 || { tail -40 /tmp/build.log; exit 1; }
rm -rf /out/sdl
cmake --install /tmp/sdl-build >/dev/null
cp /tmp/sdl/LICENSE.txt /out/sdl/LICENSE.txt
chown -R "$OWNER" /out/sdl
