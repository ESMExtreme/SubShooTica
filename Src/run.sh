#!/bin/bash
set -e
set -o pipefail

SRC="main.cpp"
OUT="kropka"
IMGUI_DIR="./imgui"
GLSL_VERSION="#version 330 core"

echo "== Build & run (Linux only) =="

# dependencies quick hint
echo "Upewnij się, że zainstalowane: libglfw3-dev libglew-dev libgl1-mesa-dev build-essential git"

# clone imgui if missing
if [ ! -d "$IMGUI_DIR" ]; then
    echo "imgui not found in $IMGUI_DIR — clonowanie..."
    git clone --depth 1 https://github.com/ocornut/imgui.git "$IMGUI_DIR"
fi

# verify backend headers exist
if [ ! -f "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp" ] || [ ! -f "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp" ]; then
    echo "Brakuje backendów w ${IMGUI_DIR}/backends. Sprawdź zawartość katalogu."
    exit 1
fi

# compile command
CXX="g++"
CXXFLAGS="-std=c++11 -O2 -DIMGUI_IMPL_OPENGL_LOADER_GLEW -I${IMGUI_DIR} -I${IMGUI_DIR}/backends"
SRC_FILES="$SRC \
${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp \
${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
LIBS="-lglfw -lGLEW -lGL -ldl -pthread"

CMD="$CXX $CXXFLAGS $SRC_FILES $LIBS -o $OUT"
echo "Komenda kompilacji:"
echo "$CMD"
echo

# run compilation and capture output
if ! $CMD 2>&1 | tee build.log; then
    echo "Kompilacja nie powiodła się. Zawartość build.log:"
    tail -n 80 build.log
    echo "Wklej tutaj pełny błąd (ostatnie linie build.log)."
    exit 1
fi

echo "Kompilacja zakończona sukcesem."
chmod +x "$OUT" || true
echo "Uruchamiam ./$(pwd)/$OUT"
./"$OUT"