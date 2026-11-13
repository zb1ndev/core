#!/bin/bash

echo "[LOG] building core..."
echo

set -e
generate_protocols=0
install_dependencies=0

for arg in "$@"; do

    if [[ "$arg" == "-g" ]]; then
        
        generate_protocols=1

    elif [[ "$arg" == "-i" ]]; then
        
        install_dependencies=1
    
    fi

done

if [[ $install_dependencies -eq 1 ]]; then

    # @todo install dependencies
    echo "[LOG] installing dependencies..."

    sudo apt-get update
    sudo apt-get install -y libwayland-dev libwayland-bin libgl-dev libegl-dev libgles-dev libcglm-dev wayland-protocols
    
    echo "[LOG] finished installing dependencies"
    echo

fi

scanner="wayland-scanner"
protocol_dir="$(pkg-config --variable=pkgdatadir wayland-protocols)"
protocol_out="deps/wayland"

mkdir -p "$protocol_out"

generate_protocol() {

    local name="$1"
    local xml="$2"

    "$scanner" client-header "$xml" "$protocol_out/$name-client-protocol.h"
    "$scanner" private-code  "$xml" "$protocol_out/$name-protocol.c"
    echo "[LOG] generated $name"

}

if [[ $generate_protocols -eq 1 ]]; then

    echo "[LOG] started generating wayland protocols..."
    
    generate_protocol "xdg-shell" "$protocol_dir/stable/xdg-shell/xdg-shell.xml"
    generate_protocol "pointer-constraints-unstable-v1" "$protocol_dir/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml"
    generate_protocol "relative-pointer-unstable-v1" "$protocol_dir/unstable/relative-pointer/relative-pointer-unstable-v1.xml"
    generate_protocol "xdg-toplevel-icon-v1" "$protocol_dir/staging/xdg-toplevel-icon/xdg-toplevel-icon-v1.xml"
    
    echo "[LOG] finished generating wayland protocol"
    echo

fi

mkdir -p ./build/int ./build/examples
rm -f ./build/int/*.o ./build/libcore.a

object_files=()
source_files=(
    "./deps/glad/glad.c"
    "./source/debug/debug.c"
    "./source/input/input-state.c"
    "./deps/wayland/xdg-shell-protocol.c"
    "./source/window/linux/window-linux.c"
    "./source/graphics/linux/graphics-linux.c"
    "./deps/wayland/xdg-toplevel-icon-v1-protocol.c"
    "./deps/wayland/relative-pointer-unstable-v1-protocol.c"
    "./deps/wayland/pointer-constraints-unstable-v1-protocol.c"
)

includes="-L./deps/cglm -I./deps/ -I./source/ -I./examples/02-simple-renderer-ogl -I./examples/02-simple-renderer-ogl/deps"
linkages="-lwayland-client -lwayland-cursor -lwayland-egl -lGL -lEGL -lGLESv2 -lrt -lcglm -lm"
cflags=""
compiler="gcc"

for file in "${source_files[@]}"; do
    filename="${file##*/}"
    base_name="${filename%.c}"
    obj_path="./build/int/${base_name}.o"
    $compiler -c "$file" -o "$obj_path" $cflags $includes
    object_files+=("$obj_path")
done

ar rcs ./build/libcore.a "${object_files[@]}"

gcc -o ./build/examples/01-main ./examples/01-hello-world-ogl/main.c -L./build $includes -lcore $linkages
./build/examples/01-main

# gcc -o ./build/examples/02-main ./examples/02-simple-renderer-ogl/main.c -L./build  $includes -lcore $linkages
# ./build/examples/02-main