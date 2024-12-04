# JudoShiaiMeldungenGenerator

Willkommen zur Dokumentation des **JudoShiaiMeldungenGenerators**, einer Software, die Kämpferdaten aus mehreren CSV-Dateien zusammenführt und organisiert. Dieses Dokument beschreibt die Entwicklung, Funktionsweise, Installation und Nutzung der Software sowie die Möglichkeiten zur Zusammenarbeit über GitHub.

Möchten sie mehr über die Handhabung für die Meldungsdateien erfahren schauen sie auf unser [WIKI](https://github.com/lusmdl/JudoShiaiRegistrationGenerator/wiki)

---

## Übersicht

Der **JudoShiaiMeldungenGenerator** wurde in **C++20** entwickelt und verwendet die Entwicklungsumgebung **Visual Studio Community 2022**. Das Hauptziel der Software ist es, mehrere Kämpferlisten (CSV-Dateien) in einer einzigen Datei zusammenzuführen, die leicht für Judo-Wettkämpfe verwendet werden kann.

### Kernfunktionen

1. **Datenzusammenführung**: 
   - Liest alle Dateien im aktuellen Verzeichnis, die mit **`Meldungen`** beginnen und im `.csv`-Format vorliegen.
   - Kombiniert die Daten in eine einzige Datei namens **`Teilnehmerliste.csv`**.

2. **Datenanreicherung**:
   - Fügt den Kämpferdaten eindeutige **`ID`**-Nummern hinzu.
   - Ergänzt eine **`file_id`**, um die Herkunft der Daten (Eingabedatei) zu kennzeichnen.

3. **Zusätzliche Datenverarbeitung**:
   - Liest und verarbeitet spezifische Kämpferattribute wie Name, Verein, Gewicht und Graduierung.
   - Unterstützt Zwischenstufen in den Graduierungen (z. B. Gelb-Orange).

4. **Benutzerfreundlicher CSV-Export**:
   - Exportiert die zusammengeführten Daten in einer gut strukturierten Datei mit allen erforderlichen Spalten.

---

## Voraussetzungen

Um den **JudoShiaiMeldungenGenerator** nutzen zu können, benötigen Sie:

1. **Systemanforderungen**:
   - Betriebssystem: Windows 10 oder höher.
   - Arbeitsspeicher: Mindestens 4 GB RAM.
   - Speicherplatz: Mindestens 100 MB verfügbarer Speicher.

2. **Entwicklungsumgebung**:
   - **Visual Studio Community 2022** (kostenlos erhältlich).
   - Unterstützt C++20.

3. **Compiler**:
   - MSVC (Microsoft C++ Compiler), der in Visual Studio enthalten ist.

---

## Installation und Einrichtung

### 1. Visual Studio Community 2022 installieren
Falls noch nicht installiert:
1. Laden Sie Visual Studio Community 2022 herunter: [Visual Studio Download](https://visualstudio.microsoft.com/de/visual-studio-community/).
2. Wählen Sie bei der Installation das **C++-Entwicklungs-Toolset** aus.

### 2. Projekt aus GitHub klonen
1. Öffnen Sie Visual Studio.
2. Wählen Sie im Startmenü **„Ein Projekt aus der Versionsverwaltung klonen“**.
3. Geben Sie die URL Ihres GitHub-Repositories ein und laden Sie das Projekt herunter.

### 3. Projekt kompilieren
1. Öffnen Sie das heruntergeladene Projekt in Visual Studio.
2. Stellen Sie sicher, dass der Standardcompiler auf **C++20** eingestellt ist:
   - **Rechtsklick auf das Projekt → Eigenschaften → Konfigurationseigenschaften → C/C++ → Sprachstandard → ISO C++20**.
3. Wählen Sie oben die Option **„Release“** oder **„Debug“**, je nach Bedarf.
4. Klicken Sie auf **„Erstellen“** oder drücken Sie **Strg + Umschalt + B**, um das Projekt zu kompilieren.

---

## Nutzung der Software

1. **Eingabedateien vorbereiten**:
   - Stellen Sie sicher, dass sich alle **`Meldungen*.csv`**-Dateien im selben Verzeichnis wie die ausführbare Datei befinden.
   - Die Dateien müssen die korrekten Spalten enthalten (siehe [Spaltenbeschreibung](#spaltenbeschreibung)).

2. **Programm starten**:
   - Führen Sie die generierte **`JudoShiaiMeldungenGenerator.exe`** aus.

3. **Ausgabedatei prüfen**:
   - Nach der Ausführung wird eine konsolidierte Datei namens **`Teilnehmerliste.csv`** erstellt, die alle Kämpferdaten enthält.

---

## Spaltenbeschreibung

Die Software erwartet folgende Spalten in den Eingabedateien:

| **Spalte**       | **Beschreibung**                          | **Beispiel**                     |
|-------------------|------------------------------------------|----------------------------------|
| **Name**          | Nachname des Kämpfers                   | `Meidel`                        |
| **Vorname**       | Vorname des Kämpfers                    | `Louis`                         |
| **Geburtsjahr**   | Jahr der Geburt des Kämpfers            | `2000`                          |
| **Graduierung**   | Gürtelgraduierung (z. B. `1k`, `2d`)    | `1k`                            |
| **Verein**        | Name des Vereins                        | `Judo Club Obernburg`           |
| **Land**          | ISO 3166-1 Alpha-3 Code des Landes      | `GER`                           |
| **Gewicht**       | Gewicht des Kämpfers in Kilogramm       | `73`                            |
| **Geschlecht**    | Geschlecht des Kämpfers                 | `m`                             |
| **Kommentar**     | Zusätzliche Bemerkungen                 | `Benötigt keine Übernachtung`   |

---

## Software-Entwicklungsdetails

### Architektur
- Die Software folgt einer modularen Struktur:
  - **Datenlesen**: Eingabedateien werden zeilenweise gelesen.
  - **Verarbeitung**: Daten werden geparst und in ein standardisiertes Format überführt.
  - **Export**: Daten werden in einer gut strukturierten Datei ausgegeben.

### Verwendete Bibliotheken
- **<filesystem>**: Zur Dateiverwaltung (Eingabedateien suchen).
- **<fstream>**: Zum Lesen und Schreiben von CSV-Dateien.
- **<string> und <sstream>**: Für die Verarbeitung von Text- und Datenfeldern.

### Erweiterbarkeit
- Die Software wurde so gestaltet, dass neue Spalten oder Anforderungen leicht hinzugefügt werden können.
- Die Modularität der Funktionen erlaubt einfache Anpassungen durch Entwickler.

---

## Zusammenarbeit über GitHub

### Repository einrichten
1. Erstellen Sie ein GitHub-Repository für das Projekt.
2. Laden Sie die Quellcodedateien hoch:
   - **main.cpp**: Enthält die gesamte Logik des Programms.
   - **README.md**: Dokumentation des Projekts.
3. Fügen Sie eine `.gitignore`-Datei hinzu, um temporäre Dateien (z. B. Build-Ordner) auszuschließen.

### Workflow für Zusammenarbeit
1. **Forks**:
   - Andere Entwickler können das Repository forken und Änderungen vorschlagen.
2. **Pull Requests**:
   - Änderungen werden als Pull Requests eingereicht und überprüft, bevor sie ins Hauptprojekt integriert werden.
3. **Branches**:
   - Verwenden Sie separate Branches für neue Features oder Bugfixes (z. B. `feature/graduierung-erweitern`).

---

## Fazit

Die Software ist robust, modular und benutzerfreundlich gestaltet. Mit Visual Studio Community 2022 und GitHub als Tools ist eine einfache Installation, Nutzung und Weiterentwicklung garantiert.

Für Fragen oder Verbesserungsvorschläge wenden Sie sich bitte an das Entwicklungsteam oder erstellen Sie ein [Issue](https://github.com/lusmdl/JudoShiaiRegistrationGenerator/issues/new) im GitHub-Repository. Viel Erfolg bei der Nutzung des **JudoShiaiMeldungenGenerators**!