# SubShooTica

Podwodna gra shoot'em up zainspirowana Subnautica.

# Instalacja i Kompilacja

## **Windows - Visual Studio**
1. Pobierz i zainstaluj Visual Studio (z C++ tools)
2. Pobierz i zainstaluj Git
3. Pobierz i zainstaluj CMake
4. Otwórz projekt w Visual Studio i poczekaj na instalację dependencji przez CMake
5. Skompiluj projekt - **Assets automatycznie skopiują się do bin/**

## **Windows - CLion**
1. Otwórz projekt w CLion
2. CMake automatycznie pobierze SFML i inne dependencje
3. Skompiluj projekt (Ctrl+F9)
4. **Assets automatycznie skopiują się do cmake-build-debug/bin/**
5. Uruchom grę (Shift+F10)

## **Ważne!**
- Folder `Assets/` jest **automatycznie kopiowany** do katalogu z exe podczas kompilacji
- Nie ma potrzeby ręcznego kopiowania plików
- Jeśli masz problemy z brakami plików, uruchom ponownie kompilację

# Struktura Projektu

```
SubShooTica/
├── Assets/          # Wszystkie zasoby gry (automatycznie kopiowane do bin/)
│   ├── Fonts/       # Czcionki
│   ├── Media/       # Grafiki, tła, dźwięki
│   │   ├── Map/     # Mapy poziomów
│   │   ├── tutorial/ # Zasoby tutorialu
│   │   └── shop_items/ # Przedmioty w sklepie
│   ├── Audio/       # Muzyka i dźwięki
│   ├── save*.json   # Pliki zapisów gry
│   └── *.txt        # Pliki konfiguracyjne
├── Src/             # Kod źródłowy
└── cmake-build-debug/bin/  # Tu trafia exe i Assets po kompilacji
```



# Authors
[Daniel Cebula](https://github.com/ESMExtreme/)
[Filip Będkowski](https://github.com/curdefaya)
[Marek Bochenek](https://github.com/Chlep6g)

# Credits

[Subnautica](https://subnautica.fandom.com/wiki/Subnautica_Wiki)
