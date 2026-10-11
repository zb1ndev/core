mkdir -p ./build/int ./build/examples

object_files=()
source_files=(
    "./source/core.c"
    "./source/math/math.c"
    "./source/debug/debug.c"
    "./source/window/window_linux.c"
)

includes="-I./deps/ -I./source/"
linkages="-lwayland-client -lwayland-cursor -lwayland-egl -lEGL -lGLESv2"

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