#!/bin/bash

SRC="main.cpp"
OUT="kropka"

echo "Na jakim systemie chcesz kompilować? (linux/windows)"
read system

if [ "$system" = "linux" ]; then
    echo "Kompiluję dla Linux..."
    g++ -Wall -std=c++11 "$SRC" -lglfw -lGLEW -lGL -o "$OUT"
    if [ $? -eq 0 ]; then
        echo "Kompilacja zakończona sukcesem!"
        echo "Uruchamiam program..."
        ./"$OUT"
    else
        echo "Błąd kompilacji!"
    fi

elif [ "$system" = "windows" ]; then
    echo "Kompiluję dla Windows (MinGW)..."
    g++ -Wall -std=c++11 "$SRC" -I"C:/mingw/include" -L"C:/mingw/lib" -lglew32 -lglfw3 -lopengl32 -lgdi32 -o "$OUT.exe"
    if [ $? -eq 0 ]; then
        echo "Kompilacja zakończona sukcesem!"
        echo "Uruchamiam program..."
        ./"$OUT.exe"
    else
        echo "Błąd kompilacji!"
    fi

else
    echo "Nieznany system. Wpisz 'linux' albo 'windows'."
fi
