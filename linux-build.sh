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
)

includes="-L./deps/cglm -I./deps/ -I./source/"
linkages="-lwayland-client -lwayland-cursor -lwayland-egl -lGL -lEGL -lGLESv2 -lrt -lcglm"
compiler="gcc"

for file in "${source_files[@]}"; do
    filename="${file##*/}"
    base_name="${filename%.c}"
    obj_path="./build/int/${base_name}.o"
    $compiler -c "$file" -o "$obj_path" $includes
    object_files+=("$obj_path")
done

ar rcs ./build/libcore.a "${object_files[@]}"

gcc -o ./build/examples/01-test ./examples/01-hello-world/main.c -L./build $includes -lcore $linkages
./build/examples/01-test