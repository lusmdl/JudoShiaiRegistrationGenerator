# JudoShiaiMeldungenGenerator

Der **JudoShiaiMeldungenGenerator** ist eine Webanwendung (HTML, CSS, JavaScript), die Kämpferdaten aus mehreren CSV-Dateien zusammenführt und für Judo-Wettkämpfe aufbereitet. Sie läuft komplett im Browser: keine Installation, kein Server, keine Uploads. Alle Daten bleiben auf deinem Rechner.

Mehr zur Handhabung der Meldungsdateien und zu den einzelnen Spalten findest du im [WIKI](https://github.com/lusmdl/JudoShiaiRegistrationGenerator/wiki), insbesondere in den [Column Guidelines](https://github.com/lusmdl/JudoShiaiRegistrationGenerator/wiki/Column-Guidelines).

---

## Schnellstart

1. Die Datei **`judoshia.html`** im Browser öffnen (Doppelklick genügt, funktioniert auch offline).
2. Oben zwischen den beiden Modi wählen:
   - **1 · Meldungen zusammenfassen**: vorhandene `Meldungen*.csv`-Dateien einlesen und zu einer Teilnehmerliste zusammenführen.
   - **2 · Neue Meldungen-Datei**: Judoka erfassen und als neue Meldungsdatei exportieren.
3. Mit **„Beispieldaten laden“** lässt sich Modus 1 sofort ausprobieren.

Unterstützt werden aktuelle Versionen von Chrome, Edge, Firefox und Safari.

---

## Funktionen

### Modus 1: Meldungen zusammenfassen

1. **CSV-Dateien wählen** (mehrere Dateien gleichzeitig möglich, z. B. alle Dateien eines Ordners markieren).
2. **Dateien erkennen:** Anhand der Präfixe aus der Konfiguration wird jede Datei als **Typ A**, **Typ B** oder als **ignoriert** eingestuft. Dateien werden nach Namen sortiert und erhalten fortlaufende **`file_ID`s** (ab 1).
3. **Übersicht:** Kennzahlen zu Dateien, Typ A/B, ignorierten Dateien und Datensätzen.
4. **Tabellen je Datei:** Jede eingelesene Datei wird als eigene Tabelle angezeigt. Mit **„Tabelle kopieren“** landet sie formatiert in der Zwischenablage und lässt sich direkt in eine E-Mail oder ein Dokument einfügen.
5. **Teilnehmerliste:** Alle Datensätze mit automatisch vergebener `ID` und `file_ID`.
   - **Filter:** Suche (Freitext), Kategorie, Verein, Land, Geschlecht, Graduierung, Quelldatei (`file_ID`), Geburtsjahr von/bis.
   - **Sortierung:** per Klick auf eine Spaltenüberschrift (nochmals klicken kehrt die Reihenfolge um).
   - **„Ansicht kopieren“:** kopiert die aktuell gefilterte und sortierte Tabelle in die Zwischenablage.
   - **„Teilnehmerliste.csv exportieren“:** exportiert die komplette Liste.
   - **„Gefilterte exportieren“:** exportiert nur die aktuell sichtbaren Zeilen.

### Modus 2: Neue Meldungen-Datei

1. **Dateityp wählen:** Typ A (klassisch, mit Kategorie) oder Typ B (vereinfacht, Kategorie wird berechnet). Die Felder passen sich entsprechend an.
2. **Dateiname:** Präfix aus der Konfiguration wählen (Dropdown), danach den Namen eingeben, üblicherweise den Verein. Beispiel: `Meldungen_` + `Berlin` → `Meldungen_Berlin.csv`.
3. **Verein vorausgefüllt:** Der Name aus dem Dateinamen wird als Standard in das Feld „Verein“ übernommen. Du kannst dort jederzeit etwas anderes eintragen. Nach dem Speichern oder Verwerfen springt das Feld wieder auf den Standard zurück.
4. **Eingabehilfen:**
   - **Gürtelgrad** als Dropdown (9k bis 1k, 1d bis 10d) mit Gürtelfarbe.
   - **Geburtsjahr** per Kalenderansicht (Jahresraster mit Blättern, Anzeige von Alter und Altersklasse) oder direkter Eingabe.
   - **Land** als Dropdown mit allen ISO-3166-1-Alpha-3-Ländern. Deutschland steht als `GER`, die Turnier-Länder (GER, AUT, BEL, CZE, DNK, FRA, LUX, NLD, POL, CHE) stehen oben.
   - **Gewicht** (nur Typ A, optional): fließt als Suffix in die berechnete Kategorie ein, z. B. `U15m-50`.
   - **„Kategorie automatisch berechnen“** (nur Typ A).
5. **Validierung** beim Speichern: fehlerhafte Felder werden rot markiert, die Fehler aufgelistet.
6. **Vorschau** der CSV, danach **exportieren**, **kopieren** oder alle Datensätze löschen.

> Die erfassten Datensätze liegen nur im Speicher der geöffneten Seite. Vor dem Schließen oder Neuladen die Datei exportieren.

---

## Dateiformate

Als Trennzeichen wird **Komma oder Semikolon** automatisch anhand der Kopfzeile erkannt. Die Kopfzeile selbst wird übersprungen.

### Typ A (klassisch)

```
Name,Vorname,Geburtsjahr,Graduierung,Verein,Land,Kategorie,Geschlecht,Kommentar
```

Die Kategorie ist bereits enthalten und wird unverändert übernommen.

### Typ B (vereinfacht)

```
Vorname,Nachname,Geburtsjahr,Graduierung,Verein,Land,Geschlecht,Kommentar
```

Die Kategorie wird beim Einlesen aus Geburtsjahr und Geschlecht berechnet.

### Ausgabe: `Teilnehmerliste.csv`

```
Name,Vorname,Geburtsjahr,Graduierung,Verein,Land,Kategorie,ID,file_ID,Geschlecht,Kommentar
```

### Feldregeln

| Feld | Regel |
|------|-------|
| Name / Vorname | Pflicht, Text, max. 50 Zeichen |
| Geburtsjahr | Pflicht, 4-stellige Ganzzahl (1900 bis aktuelles Jahr) |
| Graduierung | Pflicht, Zahl + `k` (Kyu, 1 bis 9) oder `d` (Dan, 1 bis 10). Ohne `k`/`K` gilt der Grad als Dan |
| Verein | Pflicht, Text, max. 100 Zeichen |
| Land | Pflicht, ISO-3166-1-Alpha-3-Code, z. B. `GER`, `AUT`, `CHE` |
| Kategorie | Optional, Format `[Altersklasse][m/w]-[Gewichtsklasse]`. Eine angegebene Kategorie wird unverändert übernommen |
| Gewicht | Optional (nur Formular, Typ A), positive Ganzzahl in kg, „kg“ wird entfernt |
| Geschlecht | `m` oder `w`. Enthält die Angabe ein `w`, gilt sie als weiblich, alles andere als männlich |
| Kommentar | Optional, Text, max. 200 Zeichen |

Im Formular werden Kommas in Eingaben durch Leerzeichen ersetzt, damit die CSV-Struktur erhalten bleibt.

Gürtelgrade im Überblick:

| Grad | Gürtelfarbe |
|------|-------------|
| 9k / 8k / 7k | Weiß / Weiß-Gelb / Gelb |
| 6k / 5k / 4k | Gelb-Orange / Orange / Orange-Grün |
| 3k / 2k / 1k | Grün / Blau / Braun |
| 1d bis 5d | Schwarz |
| 6d bis 8d | Rot-Weiß |
| 9d / 10d | Rot |

---

## Konfiguration (`config.json`)

Die Konfiguration steuert, welche Dateien erkannt werden und wie Kategorien berechnet werden. Sie wird im aufklappbaren Bereich **„Konfiguration (config.json)“** angezeigt:

- **config.json importieren:** lädt eine Datei von deinem Computer.
- **Änderungen übernehmen:** aktiviert Änderungen, die direkt im Textfeld gemacht wurden.
- **config.json exportieren:** speichert die aktuelle Konfiguration.

Standard-Konfiguration:

```json
{
  "input_types": {
    "A": { "prefixes": ["Meldungen_", "MeldungKlassisch_", "Meisterschaft_"] },
    "B": { "prefixes": ["Weihnachtsturnier_", "MeldungEinfach_", "JCOTurnier_"] }
  },

  "kids_classes": [
    { "name": "___FATAL___ERROR___", "max_age": 6 },
    { "name": "U9",  "max_age": 8 },
    { "name": "U11", "max_age": 10 },
    { "name": "U13", "max_age": 12 },
    { "name": "U15", "max_age": 14 },
    { "name": "U18", "max_age": 17 },
    { "name": "U21", "max_age": 20 }
  ],

  "adult_class": { "male": "M", "female": "W" },
  "sex": { "male": "m", "female": "w" },
  "weight_suffix": "-0"
}
```

| Schlüssel | Bedeutung |
|-----------|-----------|
| `input_types.A.prefixes` | `.csv`-Dateien, deren Name mit einem dieser Präfixe beginnt, werden als **Typ A** gelesen |
| `input_types.B.prefixes` | `.csv`-Dateien, deren Name mit einem dieser Präfixe beginnt, werden als **Typ B** gelesen |
| `kids_classes` | Altersklassen in aufsteigender Reihenfolge. Es gilt die erste Klasse, bei der das Alter `max_age` nicht überschreitet |
| `adult_class` | Bezeichnung der Klasse für Erwachsene (männlich / weiblich) |
| `sex` | Kürzel, das hinter der Altersklasse angehängt wird |
| `weight_suffix` | Platzhalter für die Gewichtsklasse bei berechneten Kategorien |

Die Konfiguration wird beim Neuladen der Seite auf die Standardwerte zurückgesetzt. Eigene Werte entweder per Import laden oder die Standardwerte im Quelltext anpassen (Konstante `DEF` im Skript von `judoshia.html`).

### Berechnung der Kategorie

1. Alter = aktuelles Jahr − Geburtsjahr.
2. Altersklasse = erste Klasse aus `kids_classes` mit `Alter <= max_age`, sonst Erwachsene.
3. Kinder: Altersklasse + Geschlechtskürzel (`U15` + `m` = `U15m`). Erwachsene: `adult_class.male` bzw. `.female`.
4. Zum Schluss wird das Gewichts-Suffix angehängt (`weight_suffix` oder, falls im Formular angegeben, `-<Gewicht>`).

Beispiele mit `weight_suffix` = `-0`: ein Junge Jahrgang 2012 (Alter 14) ergibt `U15m-0`, eine erwachsene Frau ergibt `W-0`.

> Die Klasse `___FATAL___ERROR___` in der Standard-Konfiguration ist ein bewusster Warnmarker: Wer 6 Jahre oder jünger ist, fällt in der Ausgabe sofort auf.

---

## Export und Zwischenablage

- **Exporte** (Teilnehmerliste, neue Meldungsdatei, config.json) werden als Datei heruntergeladen. Je nach Browser fragt dieser nach dem Speicherort.
- **Kopieren:** Tabellen werden sowohl als formatierte Tabelle (für E-Mails und Office) als auch als Tab-getrennter Text kopiert. Falls der Browser den Zugriff auf die Zwischenablage blockiert, zeigt der Button „Kopieren nicht möglich“ an.

---

## Technik und Entwicklung

- **Eine einzige Datei:** HTML, CSS und JavaScript liegen gemeinsam in `judoshia.html`. Es gibt keine externen Bibliotheken, Frameworks oder Build-Schritte.
- **Datenschutz:** Dateien werden ausschließlich lokal im Browser gelesen und verarbeitet. Es findet keine Übertragung an einen Server statt.
- **Darstellung:** Das Layout ist responsiv und passt sich dem hellen oder dunklen Farbschema des Systems an.

### Aufbau des Skripts

| Bereich | Inhalt |
|---------|--------|
| Config | Standardwerte (`DEF`), Import, Export, Übernehmen |
| Logik | Trennzeichen-Erkennung, Parser für Typ A und B, Kategorieberechnung |
| Tabellen | Darstellung, Filter, Sortierung, Kopieren in die Zwischenablage, CSV-Export |
| Modus 2 | Formular, Validierung, Gürtel-Dropdown, Jahreskalender, Länderliste (ISO 3166-1), Verein-Vorbelegung |

### Erweiterbarkeit
- Neue Dateitypen, Altersklassen oder Präfixe lassen sich größtenteils über die Konfiguration ergänzen.
- Neue Spalten erfordern eine Anpassung der Parser, der Spaltenliste `COLS` und der Kopfzeilen im Skript.

### Veröffentlichen (optional)
Da die Anwendung nur aus einer statischen Datei besteht, lässt sie sich z. B. über **GitHub Pages** kostenlos bereitstellen: `judoshia.html` im Repository ablegen (am besten als `index.html`) und Pages in den Repository-Einstellungen aktivieren.

---

## Zusammenarbeit über GitHub

### Workflow
1. **Forks:** Andere Entwickler können das Repository forken und Änderungen vorschlagen.
2. **Pull Requests:** Änderungen werden als Pull Requests eingereicht und vor der Integration geprüft.
3. **Branches:** Neue Features oder Bugfixes in eigenen Branches entwickeln (z. B. `feature/graduierung-erweitern`).

---

## Fazit

Die Anwendung ist leichtgewichtig, ohne Installation nutzbar und über die Konfiguration flexibel anpassbar. Die Tabellenansichten, Filter und Kopierfunktionen machen die zusammengeführten Meldungen auf einen Blick sichtbar und leicht weiterzugeben.

Für Fragen oder Verbesserungsvorschläge wende dich an das Entwicklungsteam oder erstelle ein [Issue](https://github.com/lusmdl/JudoShiaiRegistrationGenerator/issues/new) im GitHub-Repository.
