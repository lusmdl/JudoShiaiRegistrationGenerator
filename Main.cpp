#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include <ctime>        // für Zeit/aktuelles Jahr
#include <cctype>       // für std::tolower
#include "lib/json.hpp"

using json = nlohmann::json;

#define DELIMITER ',' // Delimiter als Makro
#define INPUT_ENDING ".csv"
#define INPUT_A "Meldungen_"
#define INPUT_A_HEADER "Name,Vorname,Geburtsjahr,Graduierung,Verein,Land,Kategorie,Geschlecht,Kommentar"
#define TEILNEHMERLISTE_HEADER "Name,Vorname,Geburtsjahr,Graduierung,Verein,Land,Kategorie,ID,file_ID,Geschlecht,Kommentar"
#define CONFIG_FILE_NAME "config.json"

// kürzel für klassen
#define MIN_KLASSE "-0"

#define W 'w'
#define M 'm'

#define ERWACHSEN "E"
#define MEANNER "M"
#define FRAUEN "W"

#define C_NORM enum_ccolor::WHITE
#define C_HEAD enum_ccolor::CYAN
#define C_OPT enum_ccolor::RED
#define C_USER enum_ccolor::BRIGHT_WHITE

namespace fs = std::filesystem;

struct AgeClass {
    std::string name;
    int max_age;
};

struct AdultClass {
    std::string male;
    std::string female;
};

struct SexClass {
    std::string male;
    std::string female;
};

struct Config {
    std::vector<std::string> typeAPrefixes;
    std::vector<std::string> typeBPrefixes;

    std::vector<AgeClass> ageClasses;
    AdultClass adult;
    SexClass sex;

    std::string weightSuffix;
};

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


enum class enum_ccolor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_YELLOW = 14,
    BRIGHT_WHITE = 15
};


Config loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Konnte config.json nicht öffnen!");
    }

    json j;
    file >> j;

    Config cfg;

    // Prefixes
    cfg.typeAPrefixes = j["input_types"]["A"]["prefixes"].get<std::vector<std::string>>();
    cfg.typeBPrefixes = j["input_types"]["B"]["prefixes"].get<std::vector<std::string>>();

    // Age classes (dynamic)
    for (auto& ac : j["kids_classes"]) {
        cfg.ageClasses.push_back({
            ac["name"].get<std::string>(),
            ac["max_age"].get<int>()
            });
    }

    // Adult classes
    cfg.adult.male = j["adult_class"]["male"].get<std::string>();
    cfg.adult.female = j["adult_class"]["female"].get<std::string>();

    // Geschlecht
    cfg.sex.male = j["sex"]["male"].get<std::string>();
    cfg.sex.female = j["sex"]["female"].get<std::string>();

    // Weight suffix
    cfg.weightSuffix = j["weight_suffix"].get<std::string>();

    return cfg;
}


Config cfg = loadConfig(CONFIG_FILE_NAME);

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


// Funktion, um die Konsolenfarbe zu setzen
void SetConsoleColor(enum_ccolor color = C_NORM) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<int>(color));
}


void printUser() {

    SetConsoleColor(enum_ccolor::GREEN);
    std::cout << "\nuser@generator"; 
    SetConsoleColor(enum_ccolor::WHITE); 
    std::cout << ":";
    SetConsoleColor(enum_ccolor::LIGHT_MAGENTA); 
    std::cout << "~ $ ";
    SetConsoleColor(enum_ccolor::WHITE);
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


// CSV-Datei einlesen und Daten speichern
std::vector<Teilnehmer> readInputACSV(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Teilnehmer> teilnehmer;

    if (!file.is_open()) {
        std::cerr << "Fehler beim Öffnen der Datei: " << filename << '\n';
        return teilnehmer;
    }

    std::string line;
    char delimiter = '\0';

    // Header-Zeile lesen und Delimiter erkennen
    if (std::getline(file, line)) {
        delimiter = detectDelimiter(line);
        if (delimiter == '\0') {
            std::cerr << "Kein gültiger Delimiter in der Datei gefunden: " << filename << '\n';
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

/* -----------------------------
   NEU: Funktionen zur Kategorieberechnung
   -----------------------------
   Diese Funktionen wurden hinzugefügt, damit aus dem Geburtsjahr
   + Geschlecht automatisch die Alters-/Geschlechtsklasse und
   die finale Kategorie (z.B. "U15m-30") gebildet wird.
*/

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


/* -----------------------------
   NEU: Reader für B-Dateien
   -----------------------------
   Format erwartet (Header): Vorname,Nachname,Geburtsjahr,Geschlecht,Graduierung,Land,Verein,Kommentar
   WICHTIG: Hier wird **Geburtsjahr** in der Datei angegeben (z.B. 2015).
   Die Funktion erkennt zusätzlich auch ; als Delimiter (wie readInputACSV).
*/
std::vector<Teilnehmer> readInputBCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Teilnehmer> teilnehmer;

    if (!file.is_open()) {
        std::cerr << "Fehler beim Öffnen der Datei: " << filename << '\n';
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

int main() {

    SetConsoleColor(C_HEAD);
    std::cout << "Willkommen zum JudoShia Meldungen Generator\n\n";
    SetConsoleColor();

    std::cout << "Was wollen Sie tun?\n";
    SetConsoleColor(C_OPT);
    std::cout << " -'1'\t= Vorhandene Meldungen*.csv-Dateien aus dem Verzeichnis auslesen und zusammenfassen.\n";
    std::cout << " -'2'\t= Eine neue Meldungen*.csv-Datei hinzufuegen.";
    SetConsoleColor();


    printUser();
    SetConsoleColor(C_USER);
    int userChoice;
    std::cin >> userChoice;
    SetConsoleColor();

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

                // typ A (klassisch)
                if (startsWithAnyPrefix(filename, cfg.typeAPrefixes) && filename.ends_with(INPUT_ENDING))
                {
                    std::cout << filename;
                    int Datenseatze = 0;
                    auto teilnehmer = readInputACSV(filename);

                    for (const auto& t : teilnehmer) {

                        alleTeilnehmerMitFileId.emplace_back(t, file_id);
                        gesamtDatensaetze++;
                        Datenseatze++;
                    }

                    std::cout << "\t (A)  \t" << Datenseatze << "\n";
                    file_id++;
                }

                // typ B (vereinfacht)
                else if (startsWithAnyPrefix(filename, cfg.typeBPrefixes) && filename.ends_with(INPUT_ENDING))
                {
                    std::cout << filename;
                    int Datenseatze = 0;
                    auto teilnehmer = readInputBCSV(filename);

                    for (const auto& t : teilnehmer) {

                        alleTeilnehmerMitFileId.emplace_back(t, file_id);
                        gesamtDatensaetze++;
                        Datenseatze++;
                    }

                    std::cout << "\t (B) \t" << Datenseatze << "\n";

                    file_id++;
                }

                else {
                    //std::cout << "Ignoriere: " << filename << "\n";
                }

                // optional: weitere Dateinamen-Patterns können hier ergänzt werden
            }
        }

        // Ergebnisse schreiben
        const std::string outputFilename = "Teilnehmerliste.csv"; // Neuer Dateiname
        writeCSVFile(outputFilename, alleTeilnehmerMitFileId);

        // Zusammenfassung
        std::cout << "Verarbeitung abgeschlossen.\n\n";
        std::cout << "Gesamtanzahl der Datensaetze in der Teilnehmerliste: " << gesamtDatensaetze << "\n";
        std::cout << "Ergebnisse wurden in " << outputFilename << " geschrieben.\n";

        

        break;
    }
    case 2: {
        createNewCSVFile(); // Neue Datei erstellen und Datensätze hinzufügen
        break;
    }
    default: {
        std::cout << "\nUngueltige Eingabe! Bitte starten Sie das Programm erneut und waehlen Sie eine gueltige Option.\n";
        break;
    }
    }

    // Benutzerinteraktion vor Beendigung
    std::cout << "\n\nDruecken Sie die Eingabetaste, um das Programm zu beenden..." << std::endl;
    std::cin.get(); // Warten auf Eingabe von Enter

    return 0;
}

