#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "Ui.h"
#include "Config.h"
#include "FileProcessor.h"

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
    int userChoice = 1;
    std::cout << "1\n"; //std::cin >> userChoice;
    //std::cin.ignore(); // Eingabe-Puffer bereinigen nach std::cin >> userChoice
    SetConsoleColor();

    switch (userChoice) {
    case 1: {

        std::vector<std::pair<Teilnehmer, int>> alleTeilnehmerMitFileId; // Teilnehmer + file_id
        int file_id = 1; // Start der file_id
        int gesamtDatensaetze = 0; // Zaehlt die Gesamtzahl der Datensaetze

        // Benutzer nach Verzeichnis fragen
        std::string inputDir;
        std::cout << "Bitte geben Sie das Verzeichnis ein, in dem die CSV-Dateien liegen.\n";
        SetConsoleColor(C_OPT);
        std::cout << " -'.'\t= aktuelles Arbeitsverzeichnis -> " << std::filesystem::current_path();
        printUser();
        //std::cin.ignore(); // Eingabepuffer leeren
        std::getline(std::cin, inputDir);

        if (!fs::exists(inputDir) || !fs::is_directory(inputDir)) {
            std::cerr << "Das angegebene Verzeichnis existiert nicht oder ist kein Ordner.\n";
            return 1;
        }

        std::cout << "Starte Verarbeitung der Dateien...\n";

        // Verzeichnis durchsuchen
        for (const auto& entry : fs::directory_iterator(inputDir)) {
            if (entry.is_regular_file()) {
                const std::string filename = entry.path().filename().string();

                // typ A (klassisch)
                if (startsWithAnyPrefix(filename, cfg.typeAPrefixes) && filename.ends_with(INPUT_ENDING))
                {
                    std::cout << filename;
                    int Datenseatze = 0;
                    auto teilnehmer = readInputACSV(entry.path().string());

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
                    auto teilnehmer = readInputACSV(entry.path().string());

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

