#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include <ctime>        // für Zeit/aktuelles Jahr
#include <cctype>       // für std::tolower

#define DELIMITER ',' // Delimiter als Makro
#define INPUT_ENDING ".csv"
#define INPUT_A "Meldungen_"
#define INPUT_A_HEADER "Name,Vorname,Geburtsjahr,Graduierung,Verein,Land,Kategorie,Geschlecht,Kommentar"
#define TEILNEHMERLISTE_HEADER "Name,Vorname,Geburtsjahr,Graduierung,Verein,Land,Kategorie,ID,file_ID,Geschlecht,Kommentar"

#define W 'w'
#define M 'm'

#define ERWACHSEN "E"


namespace fs = std::filesystem;

// Struktur fuer die Teilnehmerdaten
struct Teilnehmer {
    std::string name;
    std::string vorname;
    int geburtsjahr;
    std::string graduierung;
    std::string verein;
    std::string land;
    std::string gewicht;
    char geschlecht;
    std::string kommentar;
};

bool startsWithAnyPrefix(const std::string& filename,
    const std::vector<std::string>& prefixes)
{
    for (const auto& p : prefixes) {
        if (filename.rfind(p, 0) == 0) {
            return true;                // beginnt mit Präfix
        }
    }
    return false;
}

char detectDelimiter(const std::string& line) {
    if (line.find(',') != std::string::npos) return ',';
    if (line.find(';') != std::string::npos) return ';';
    return '\0';
}

// Funktion zum Parsen einer Zeile
Teilnehmer parseLine(const std::string& line, char delimiter) {
    Teilnehmer t;
    std::istringstream stream(line);
    std::string token;

    std::getline(stream, t.name, delimiter);
    std::getline(stream, t.vorname, delimiter);
    std::getline(stream, token, delimiter);
    t.geburtsjahr = std::stoi(token);

    std::getline(stream, t.graduierung, delimiter);
    std::getline(stream, t.verein, delimiter);
    std::getline(stream, t.land, delimiter);
    std::getline(stream, t.gewicht, delimiter);

    std::getline(stream, token, delimiter);
    t.geschlecht = token.empty() ? '\0' : token[0];

    std::getline(stream, t.kommentar, delimiter);

    return t;
}

// berechnet anhand des Alters die Altersklasse (U9..U21 oder Erwachsen)
std::string berechneAltersklasse(int alter) {
    for (const auto& ac : cfg.ageClasses) {
        if (alter <= ac.max_age)
            return ac.name;
    }
    return ERWACHSEN; // Erwachsene
}

// verfeinert die Altersklasse mit Geschlechtsangabe
// Bei Erwachsenen wird Meanner bzw. Frauen zurückgegeben (statt Uxxm)
std::string berechneGeschlechtsklasse(const std::string& altersklasse, char geschlecht) {

    // Normalisieren des Geschlechtszeichens (klein/ groß)
    char g = std::tolower(static_cast<unsigned char>(geschlecht));

    if (altersklasse == ERWACHSEN) {

        return (g == M) ? cfg.adult.male : cfg.adult.female;
    }
    return altersklasse + ((g == M) ? cfg.sex.male : cfg.sex.female);
}

// Hauptfunktion: aus Geburtsjahr + Geschlecht die finale Kategorie bauen
// (z. B. "U15m-30" oder "W-30")
std::string berechneKategorie(int geburtsjahr, char geschlecht) {

    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    int aktuellesJahr = now->tm_year + 1900;

    int alter = aktuellesJahr - geburtsjahr;
    if (alter < 0) alter = 0;

    std::string altersklasse = berechneAltersklasse(alter);
    std::string geschlechtsklasse = berechneGeschlechtsklasse(altersklasse, geschlecht);

    return geschlechtsklasse + cfg.weightSuffix;
}

// CSV-Datei einlesen und Daten speichern
std::vector<Teilnehmer> readInputACSV(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Teilnehmer> teilnehmer;

    if (!file.is_open()) {
        std::cerr << "\nFehler beim Oeffnen der Datei: " << filename;
        return teilnehmer;
    }

    std::string line;
    char delimiter = '\0';

    // Header-Zeile lesen und Delimiter erkennen
    if (std::getline(file, line)) {
        delimiter = detectDelimiter(line);
        if (delimiter == '\0') {
            std::cerr << "Kein gueltiger Delimiter in der Datei gefunden: " << filename << '\n';
            return teilnehmer;
        }
    }

    // Zeilen einlesen und parsen
    while (std::getline(file, line)) {
        teilnehmer.push_back(parseLine(line, delimiter));
    }

    file.close();
    return teilnehmer;
}

std::vector<Teilnehmer> readInputBCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Teilnehmer> teilnehmer;

    if (!file.is_open()) {
        std::cerr << "\nFehler beim Oeffnen der Datei: " << filename;
        return teilnehmer;
    }

    std::string line;

    // Header-Zeile lesen, um den Delimiter zu erkennen (kompatibel zu readInputACSV)
    if (!std::getline(file, line)) {
        file.close();
        return teilnehmer;
    }
    char delimiter = detectDelimiter(line);
    if (delimiter == '\0') {
        // Fallback: falls Header ungewöhnlich formatiert ist, nehmen wir das Standard-DELIMITER ','
        delimiter = ',';
    }

    // Zeilen einlesen
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string token;
        Teilnehmer t;

        // Vorname
        std::getline(ss, t.vorname, delimiter);

        // Nachname
        std::getline(ss, t.name, delimiter);

        // Geburtsjahr: direkt übernehmen (nun erwartet)
        if (std::getline(ss, token, delimiter)) {
            try {
                t.geburtsjahr = token.empty() ? 0 : std::stoi(token);
            }
            catch (...) {
                t.geburtsjahr = 1900; // bei fehlerhafter Zahl
            }
        }
        else {
            t.geburtsjahr = 0;
        }

        // Graduierung
        std::getline(ss, t.graduierung, delimiter);

        // Verein
        std::getline(ss, t.verein, delimiter);

        // Land
        std::getline(ss, t.land, delimiter);

        // Geschlecht: erstes Zeichen (m/w)
        if (std::getline(ss, token, delimiter)) {
            t.geschlecht = token.empty() ? '\0' : token[0];
        }
        else {
            t.geschlecht = '\0';
        }

        // Kommentar (Rest der Zeile)
        std::getline(ss, t.kommentar, delimiter);

        // Kategorie / Gewicht automatisch berechnen (hier wird "gewicht" Feld dafür genutzt)
        t.gewicht = berechneKategorie(t.geburtsjahr, t.geschlecht);

        teilnehmer.push_back(t);
    }

    file.close();
    return teilnehmer;
}

// Teilnehmer in eine CSV-Datei schreiben
void writeCSVFile(const std::string& filename, const std::vector<std::pair<Teilnehmer, int>>& teilnehmerMitFileId) {

    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Fehler beim Öffnen der Datei zum Schreiben: " << filename << '\n';
        return;
    }

    // Header schreiben
    file << TEILNEHMERLISTE_HEADER << "\n";

    // Daten schreiben
    int id = 1; // ID-Zaehler beginnt bei 1
    for (const auto& [t, file_id] : teilnehmerMitFileId) {

        file
            << t.name << DELIMITER
            << t.vorname << DELIMITER
            << t.geburtsjahr << DELIMITER
            << t.graduierung << DELIMITER
            << t.verein << DELIMITER
            << t.land << DELIMITER
            << t.gewicht << DELIMITER
            << id++ << DELIMITER      // id automatisch inkrementieren
            << file_id << DELIMITER
            << t.geschlecht << DELIMITER
            << t.kommentar << '\n'; // eine leere zeile pro datensatz
    }

    file.close();
}

void createNewCSVFile() {

    std::cout << "Wie soll der Name der neuen Datei nach 'Meldungen_' lauten?";
    std::string fileSuffix;

    printUser();
    SetConsoleColor(C_USER);
    std::cin >> fileSuffix;
    SetConsoleColor();

    std::string filename = INPUT_A + fileSuffix + INPUT_ENDING;

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Fehler beim Erstellen der Datei: " << filename << "\n";
        return;
    }

    // Header in die Datei schreiben
    file << INPUT_A_HEADER << "\n";
    std::cout << "\nDatei '" << filename << "' wurde angelegt. Erstellen wir Datensaetze!\n";


    while (true) {
        Teilnehmer t;

        // Benutzereingaben für jeden Datenpunkt
        std::cout << "\nGeben Sie die Daten zum Judoka ein:\n";

        std::cout << "Name\t\t(Bauer)\t\t: ";

        SetConsoleColor(C_USER);
        std::cin.ignore();
        std::getline(std::cin, t.name);
        SetConsoleColor();

        std::cout << "Vorname\t\t(Anna)\t\t: ";

        SetConsoleColor(C_USER);
        std::getline(std::cin, t.vorname);
        SetConsoleColor();

        std::cout << "Geburtsjahr\t(2000)\t\t: ";

        SetConsoleColor(C_USER);
        std::cin >> t.geburtsjahr;
        SetConsoleColor();

        std::cout << "Guertel\t\t(1k/2d)\t\t: ";

        SetConsoleColor(C_USER);
        std::cin >> t.graduierung;
        SetConsoleColor();

        std::cout << "Verein\t\t(DJK Berlin)\t: ";

        SetConsoleColor(C_USER);
        std::cin.ignore();
        std::getline(std::cin, t.verein);
        SetConsoleColor();

        std::cout << "Land\t\t(GER)\t\t: ";

        SetConsoleColor(C_USER);
        std::cin >> t.land;
        SetConsoleColor();

        std::cout << "Kategorie\t(U18w-57)\t: ";

        SetConsoleColor(C_USER);
        std::cin >> t.gewicht;
        SetConsoleColor();

        std::cout << "Geschlecht\t(m / w)\t\t: ";

        SetConsoleColor(C_USER);
        std::cin >> t.geschlecht;
        SetConsoleColor();

        std::cout << "Kommentar\t\t\t: ";

        SetConsoleColor(C_USER);
        std::cin.ignore();
        std::getline(std::cin, t.kommentar);
        SetConsoleColor();

        // Datensatz anzeigen
        std::cout << "\nDer eingegebene Datensatz lautet:\n";

        SetConsoleColor(C_USER);
        std::cout
            << t.name << DELIMITER
            << t.vorname << DELIMITER
            << t.geburtsjahr << DELIMITER
            << t.graduierung << DELIMITER
            << t.verein << DELIMITER
            << t.land << DELIMITER
            << t.gewicht << DELIMITER
            << t.geschlecht << DELIMITER
            << t.kommentar << "\n";
        SetConsoleColor();

        // Benutzer entscheiden lassen
        std::cout << "\nWollen Sie diesen Datensatz speichern?\n";

        SetConsoleColor(C_OPT);
        std::cout << " -'j'\t= Datensatz speichern und naechsten eingeben\n";
        std::cout << " -'n'\t= Datensatz verwerfen\n";
        std::cout << " -'end'\t= Datensatz speichern und Programm beenden\n";

        SetConsoleColor();

        printUser();
        SetConsoleColor(C_USER);
        std::string choice;
        std::cin >> choice;
        SetConsoleColor();

        if (choice == "n" || choice == "N") {
            std::cout << "Datensatz wurde verworfen.\n";
            continue; // Nächsten Datensatz eingeben
        }

        // Datensatz speichern
        file << t.name << DELIMITER
            << t.vorname << DELIMITER
            << t.geburtsjahr << DELIMITER
            << t.graduierung << DELIMITER
            << t.verein << DELIMITER
            << t.land << DELIMITER
            << t.gewicht << DELIMITER
            << t.geschlecht << DELIMITER
            << t.kommentar << "\n";
        std::cout << "Datensatz gespeichert.\n";

        if (choice == "end" || choice == "END") {
            std::cout << "Programm wird beendet.\n";
            break; // Programm beenden
        }

    }

    file.close();
    std::cout << "\nDie Datei '" << filename << "' wurde erfolgreich gespeichert.\n";
}