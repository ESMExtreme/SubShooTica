#ifndef SAVEDATA_H
#define SAVEDATA_H

#include <string>
#include <fstream>
#include <iostream>

struct SaveData {
    int scrap;
    int currentLevel;
    int maxLevelUnlocked;

    SaveData() : scrap(0), currentLevel(0), maxLevelUnlocked(0) {}

    // Zapisz dane do pliku
    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Nie można otworzyć pliku do zapisu: " << filename << std::endl;
            return false;
        }
        file << scrap << "\n";
        file << currentLevel << "\n";
        file << maxLevelUnlocked << "\n";
        file.close();
        return true;
    }

    // Wczytaj dane z pliku
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Nie można otworzyć pliku do odczytu: " << filename << std::endl;
            return false;
        }
        file >> scrap;
        file >> currentLevel;
        file >> maxLevelUnlocked;
        file.close();
        return true;
    }

    // Sprawdź czy plik zapisu istnieje
    static bool fileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }
};

#endif // SAVEDATA_H

