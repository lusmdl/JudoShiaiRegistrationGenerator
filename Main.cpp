#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#define DELIMITER ',' // Delimiter als Makro

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

    std::getline(stream, t.gewicht, DELIMITER);

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
    // Header-Zeile ueberspringen
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
void writeCSVFile(const std::string& filename, const std::vector<std::pair<Teilnehmer, int>>& teilnehmerMitFileId) {

    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Fehler beim Öffnen der Datei zum Schreiben: " << filename << '\n';
        return;
    }

    // Header schreiben
    file << "Name,Vorname,Geburtsjahr,Graduierung,Verein,Land,Kategorie,ID,file_ID,Geschlecht,Kommentar\n";

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

int main() {
    std::cout << "Willkommen zum JudoShiaiMeldungenGenerator!\n\n";
    std::cout << "Bitte waehlen Sie eine Option:\n";
    std::cout << "1. Vorhandene Meldungen*.csv-Dateien aus dem Verzeichnis auslesen und zusammenfassen.\n";
    std::cout << "2. Eine neue Meldungen*.csv-Datei hinzufuegen.\n";
    std::cout << "\nIhre Wahl (1 oder 2): ";

    int userChoice;
    std::cin >> userChoice;

    switch (userChoice) {
    case 1: {
        std::cin.ignore(); // Eingabe-Puffer bereinigen nach std::cin >> userChoice

        std::vector<std::pair<Teilnehmer, int>> alleTeilnehmerMitFileId; // Teilnehmer + file_id
        int file_id = 1; // Start der file_id
        int gesamtDatensaetze = 0; // Zaehlt die Gesamtzahl der Datensaetze

        std::cout << "\nStarte Verarbeitung der Dateien...\n";

        // Verzeichnis durchsuchen
        for (const auto& entry : fs::directory_iterator(".")) {
            if (entry.is_regular_file()) {
                const std::string filename = entry.path().filename().string();

                // Pruefen, ob der Dateiname mit "Meldungen" beginnt und auf ".csv" endet
                if (filename.starts_with("Meldungen") && filename.ends_with(".csv")) {
                    std::cout << "Lese Datei: " << filename << "\t(file_id " << file_id << ") ...\t";
                    auto teilnehmer = readCSVFile(filename);

                    int anzahlDatensaetze = teilnehmer.size(); // Anzahl der Datensaetze in dieser Datei
                    gesamtDatensaetze += anzahlDatensaetze;   // Zur Gesamtsumme hinzufuegen

                    std::cout << anzahlDatensaetze << " Datensaetze in Datei gefunden.\n";

                    // Alle Teilnehmer aus der Datei mit derselben file_id versehen
                    for (const auto& t : teilnehmer) {
                        alleTeilnehmerMitFileId.emplace_back(t, file_id);
                    }

                    file_id++; // file_id inkrementieren fuer die naechste Datei
                }
            }
        }

        // Ergebnisse schreiben
        const std::string outputFilename = "Teilnehmerliste.csv"; // Neuer Dateiname
        writeCSVFile(outputFilename, alleTeilnehmerMitFileId);

        // Zusammenfassung
        std::cout << "Verarbeitung abgeschlossen.\n\n";
        std::cout << "Gesamtanzahl der Datensaetze in der Teilnehmerliste: " << gesamtDatensaetze << "\n";
        std::cout << "Ergebnisse wurden in " << outputFilename << " geschrieben.\n";

        // Benutzerinteraktion vor Beendigung
        std::cout << "\n\nDruecken Sie die Eingabetaste, um das Programm zu beenden..." << std::endl;
        std::cin.get(); // Warten auf Eingabe von Enter

        break;
    }
    case 2: {
        std::cin.ignore(); // Eingabe-Puffer bereinigen nach std::cin >> userChoice

        std::cout << "\nDie Funktion zum Hinzufuegen neuer Dateien ist noch nicht implementiert.\n";
        std::cout << "Druecken Sie die Eingabetaste, um das Programm zu beenden..." << std::endl;
        std::cin.get(); // Warten auf Eingabe von Enter

        break;
    }
    default: {
        std::cout << "\nUngueltige Eingabe! Bitte starten Sie das Programm erneut und waehlen Sie eine gueltige Option.\n";
        break;
    }
    }

    return 0;
}

