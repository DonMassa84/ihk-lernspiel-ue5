# IHK Lernspiel — Unreal Engine 5 Prototype

Minimalistisches Lernspiel für die 11 Zielvereinbarungsfragen (F1–F11) zur IHK-Beratung am 10.11.2026.

## Features
- **11 Fragen** aus `ZIELVEREINBARUNG_FRAGEN_2026-11-10.md` (Prioritäten 1–3)
- **DataTable-getrieben** — Fragen per CSV pflegbar
- **Blueprint-only** — kein C++ nötig
- **Spaced Repetition ready** — SaveGame speichert Fortschritt pro Kategorie
- **WebGL-Export** — spielbar im Browser (Netlify/GitHub Pages)

## Projektstruktur
```
UE5_IHK_Lernspiel/
├── Content/
│   ├── Blueprints/
│   │   ├── BP_GameMode_IHK.md      # GameMode Logik (Text-Doku)
│   │   └── BP_SaveGame_IHK.md      # SaveGame Struktur
│   ├── Data/
│   │   └── DT_Questions.csv        # 11 Fragen (Import → DataTable)
│   ├── UI/
│   │   └── WBP_Structure.md        # Widget-Hierarchien (4 Widgets)
│   └── SaveGames/
│       └── BP_SaveGame_IHK.md
├── import_datatable.py             # Python-Script für UE DataTable-Import
├── build_webgl.sh                  # WebGL Build + Deploy Script
└── README.md
```

## Schnellstart (Unreal Engine 5.4+)

### 1. Projekt anlegen
```bash
# Epic Games Launcher → Unreal Engine 5.4+ installieren
# Neues Projekt: "Blank" → Blueprints → Desktop/Console → "IHK_Lernspiel"
# Projektordner: /home/shadowmaker/UE5_IHK_Lernspiel
```

### 2. Struct & DataTable anlegen
Im Unreal Editor (Python Script Editor oder Commandlet):
```python
# Im UE Python Editor ausführen:
exec(open("/home/shadowmaker/UE5_IHK_Lernspiel/import_datatable.py").read())
```
Erstellt:
- `/Game/Data/QuestionRowStruct` (Struct)
- `/Game/Data/DT_Questions` (DataTable mit 11 Zeilen)

### 3. Blueprints erstellen (im Editor)

**BP_GameMode_IHK** (Parent: GameModeBase)
- Variablen wie in `Blueprints/BP_GameMode_IHK.md` anlegen
- Events: BeginPlay, StartNewGame, AnswerQuestion, GameFinished
- In Project Settings → Maps & Modes → GameMode Override setzen

**BP_SaveGame_IHK** (Parent: SaveGame)
- Variablen wie in `SaveGames/BP_SaveGame_IHK.md` anlegen
- Slot Name: "IHK_Lernspiel_Save"

**WBP_MainMenu, WBP_Question, WBP_Results, WBP_Stats** (Parent: UserWidget)
- Hierarchie wie in `UI/WBP_Structure.md` nachbauen
- Bindings zu GameMode/SaveGame variables
- Buttons → Events verknüpfen

### 4. Level & Test
- Neues Level: `MainMenu` → Widget `WBP_MainMenu` im Level Blueprint spawnen
- Play → Testen

### 5. WebGL Build & Deploy
```bash
chmod +x build_webgl.sh
./build_webgl.sh
```
Output landet in `ihk-projektarbeit-frs-archiv/docs/projektportal/ihk-lernspiel/`

Netlify deployt automatisch bei `git push` (via `netlify.toml` → `docs/projektportal`).

## CSV-Format (DT_Questions.csv)
| Spalte | Typ | Beispiel |
|--------|-----|----------|
| QuestionID | FName | F1 |
| Priority | Int32 | 1 |
| Category | String | Scope |
| QuestionText | String | "Zu meinem Verständnis..." |
| ModelAnswer | String | "Abgrenzung: Pilot..." |
| Keywords | String | "Pilot;Nicht-Ziele;Abgrenzung" |
| Difficulty | Int32 | 1 |

Neue Fragen → CSV ergänzen → Re-Import → DataTable aktualisiert sich.

## Erweiterungsideen
- **Spaced Repetition (SM-2)** in SaveGame implementieren
- **Kategorien-Filter** (nur Prio 1, nur Formal, …)
- **Text-to-Speech** für Fragen (Web Speech API im WebGL)
- **Export/Import** JSON für Austausch mit Team
- **Multi-User** via Firebase/PlayFab (Highscores, Sync)

## Lizenz
MIT — frei für Lernzwecke, IHK-Vorbereitung, FRS-Projekt.

---

**Stand**: 2026-10-14 | **UE Version**: 5.4+ | **Ziel**: WebGL / Desktop