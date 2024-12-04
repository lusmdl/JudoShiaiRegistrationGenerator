#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#define DELIMITER ',' // Delimiter als Makro

namespace fs = std::filesystem;

// Struktur für die Teilnehmerdaten
struct Teilnehmer {
    std::string name;
    std::string vorname;
    int geburtsjahr;
    std::string graduierung;
    std::string verein;
    std::string land;
    char geschlecht;
    std::string kommentar;
};

// Funktion zum Parsen einer Zeile
Teilnehmer parseLine(const std::string& line) {
    Teilnehmer t;
    std::istringstream stream(line);
    std::string token;

    // Felder aus der CSV-Zeile extrahieren
    std::getline(stream, t.name, DELIMITER);
    std::getline(stream, t.vorname, DELIMITER);

    std::getline(stream, token, DELIMITER);
    t.geburtsjahr = std::stoi(token);

    std::getline(stream, t.graduierung, DELIMITER);
    std::getline(stream, t.verein, DELIMITER);
    std::getline(stream, t.land, DELIMITER);

    std::getline(stream, token, DELIMITER);
    t.geschlecht = token.empty() ? '\0' : token[0];

    std::getline(stream, t.kommentar, DELIMITER);

    return t;
}

// CSV-Datei einlesen und Daten speichern
std::vector<Teilnehmer> readCSVFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Teilnehmer> teilnehmer;

    if (!file.is_open()) {
        std::cerr << "Fehler beim Öffnen der Datei: " << filename << '\n';
        return teilnehmer;
    }

    std::string line;
    // Header-Zeile überspringen
    if (!std::getline(file, line)) {
        return teilnehmer;
    }

    // Zeilen einlesen und parsen
    while (std::getline(file, line)) {
        teilnehmer.push_back(parseLine(line));
    }

    file.close();
    return teilnehmer;
}

// Teilnehmer in eine CSV-Datei schreiben
void writeCSVFile(const std::string& filename, const std::vector<Teilnehmer>& teilnehmer) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Fehler beim Öffnen der Datei zum Schreiben: " << filename << '\n';
        return;
    }

    // Header schreiben
    file << "Name,Vorname,Geburtsjahr,Graduierung,Verein,Land,ID,Geschlecht,Kommentar\n";

    // Daten schreiben
    int id = 1; // ID-Zähler beginnt bei 1
    for (const auto& t : teilnehmer) {
        file << t.name << DELIMITER
            << t.vorname << DELIMITER
            << t.geburtsjahr << DELIMITER
            << t.graduierung << DELIMITER
            << t.verein << DELIMITER
            << t.land << DELIMITER
            << id++ << DELIMITER // ID automatisch inkrementieren
            << t.geschlecht << DELIMITER
            << t.kommentar << '\n';
    }

    file.close();
}

int main() {
    std::vector<Teilnehmer> alleTeilnehmer;

    // Verzeichnis durchsuchen
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.is_regular_file()) {
            const std::string filename = entry.path().filename().string();

            // Prüfen, ob der Dateiname mit "Meldungen" beginnt und auf ".csv" endet
            if (filename.starts_with("Meldungen") && filename.ends_with(".csv")) {
                std::cout << "Lese Datei: " << filename << '\n';
                auto teilnehmer = readCSVFile(filename);
                alleTeilnehmer.insert(alleTeilnehmer.end(), teilnehmer.begin(), teilnehmer.end());
            }
        }
    }

    // Ergebnisse ausgeben
    std::cout << "Es wurden " << alleTeilnehmer.size() << " Einträge aus mehreren Dateien gelesen:\n";
    for (const auto& t : alleTeilnehmer) {
        std::cout << "Name: " << t.name
            << ", Vorname: " << t.vorname
            << ", Geburtsjahr: " << t.geburtsjahr
            << ", Graduierung: " << t.graduierung
            << ", Verein: " << t.verein
            << ", Land: " << t.land
            << ", Geschlecht: " << t.geschlecht
            << ", Kommentar: " << t.kommentar << '\n';
    }

    // Ergebnisse schreiben
    const std::string outputFilename = "Teilnehmer.csv";
    writeCSVFile(outputFilename, alleTeilnehmer);

    std::cout << "Ergebnisse wurden in " << outputFilename << " geschrieben.\n";

    std::cin.get();  // Wartet auf eine Benutzereingabe

    return 0;
}
