#ifndef SAVEDATA_H
#define SAVEDATA_H

#include <string>
#include <fstream>
#include <vector>
#include <sstream>

struct SaveData {
    int scrap;
    int currentLevel;
    int maxLevelUnlocked;
    std::vector<bool> shopItemsPurchased; // 7 przedmiotów w sklepie

    SaveData() : scrap(0), currentLevel(0), maxLevelUnlocked(0), shopItemsPurchased(7, false) {}

    // Zapisz dane do pliku JSON
    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        // Prosty format JSON
        file << "{\n";
        file << "  \"scrap\": " << scrap << ",\n";
        file << "  \"currentLevel\": " << currentLevel << ",\n";
        file << "  \"maxLevelUnlocked\": " << maxLevelUnlocked << ",\n";
        file << "  \"shopItemsPurchased\": [";

        for (size_t i = 0; i < 7; ++i) {
            if (i > 0) file << ", ";
            file << (i < shopItemsPurchased.size() && shopItemsPurchased[i] ? "true" : "false");
        }

        file << "]\n";
        file << "}\n";
        file.close();
        return true;
    }

    // Wczytaj dane z pliku JSON
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        // Prosty parser JSON
        std::string line;
        shopItemsPurchased.resize(7, false);

        while (std::getline(file, line)) {
            // Usuń białe znaki z początku
            size_t start = line.find_first_not_of(" \t\n\r");
            if (start == std::string::npos) continue;
            line = line.substr(start);

            if (line.find("\"scrap\"") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    std::string value = line.substr(pos + 1);
                    // Usuń białe znaki, przecinki i inne znaki
                    size_t numStart = value.find_first_of("0123456789-");
                    if (numStart != std::string::npos) {
                        size_t numEnd = value.find_first_not_of("0123456789-", numStart);
                        if (numEnd != std::string::npos) {
                            value = value.substr(numStart, numEnd - numStart);
                        } else {
                            value = value.substr(numStart);
                        }
                        try {
                            scrap = std::stoi(value);
                        } catch (...) {}
                    }
                }
            }
            else if (line.find("\"currentLevel\"") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    std::string value = line.substr(pos + 1);
                    size_t numStart = value.find_first_of("0123456789-");
                    if (numStart != std::string::npos) {
                        size_t numEnd = value.find_first_not_of("0123456789-", numStart);
                        if (numEnd != std::string::npos) {
                            value = value.substr(numStart, numEnd - numStart);
                        } else {
                            value = value.substr(numStart);
                        }
                        try {
                            currentLevel = std::stoi(value);
                        } catch (...) {}
                    }
                }
            }
            else if (line.find("\"maxLevelUnlocked\"") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    std::string value = line.substr(pos + 1);
                    size_t numStart = value.find_first_of("0123456789-");
                    if (numStart != std::string::npos) {
                        size_t numEnd = value.find_first_not_of("0123456789-", numStart);
                        if (numEnd != std::string::npos) {
                            value = value.substr(numStart, numEnd - numStart);
                        } else {
                            value = value.substr(numStart);
                        }
                        try {
                            maxLevelUnlocked = std::stoi(value);
                        } catch (...) {}
                    }
                }
            }
            else if (line.find("\"shopItemsPurchased\"") != std::string::npos) {
                // Wczytaj tablicę shopItemsPurchased
                size_t start = line.find("[");
                size_t end = line.find("]");
                if (start != std::string::npos && end != std::string::npos) {
                    std::string arrayContent = line.substr(start + 1, end - start - 1);
                    size_t index = 0;
                    size_t pos = 0;

                    while (pos < arrayContent.length() && index < 5) {
                        // Pomiń białe znaki i przecinki
                        while (pos < arrayContent.length() &&
                               (arrayContent[pos] == ' ' || arrayContent[pos] == ',' ||
                                arrayContent[pos] == '\t')) {
                            pos++;
                        }

                        if (pos >= arrayContent.length()) break;

                        if (arrayContent.substr(pos, 4) == "true") {
                            shopItemsPurchased[index] = true;
                            pos += 4;
                            index++;
                        }
                        else if (arrayContent.substr(pos, 5) == "false") {
                            shopItemsPurchased[index] = false;
                            pos += 5;
                            index++;
                        }
                        else {
                            pos++;
                        }
                    }
                }
            }
        }

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

