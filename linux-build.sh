mkdir -p ./build/int ./build/examples
rm -f ./build/int/*.o ./build/libcore.a

object_files=()
source_files=(
    "./source/math/math.c"
    "./source/debug/debug.c"
    "./source/gapi/linux/gapi_linux.c"
    "./source/window/linux/window_linux.c"
    "./deps/wayland/xdg-shell-protocol.c"
)

includes="-I./deps/ -I./source/"
linkages="-lwayland-client -lwayland-cursor -lwayland-egl -lGL -lEGL -lGLESv2 -lrt"

for file in "${source_files[@]}"; do
    filename="${file##*/}"
    base_name="${filename%.c}"
    obj_path="./build/int/${base_name}.o"
    gcc -c "$file" -o "$obj_path" $includes
    object_files+=("$obj_path")
done

ar rcs ./build/libcore.a "${object_files[@]}"
gcc -o ./build/examples/01-test ./examples/01-hello-world/main.c -L./build $includes -lcore $linkages
./build/examples/01-test