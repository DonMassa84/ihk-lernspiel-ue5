# IHK Lernspiel - UE 5.4.4 Design Document

**Version**: 1.0  
**Engine**: Unreal Engine 5.4.4 (frozen for v1)  
**Target**: IHK-Projektleitung Zertifizierung - Zielvereinbarungsfragen F1–F11  
**IHK-Beratung**: 2026-11-10 12:45, Jägerstr. 30 Stuttgart  
**Status**: PRE_PROJECT (kein .uproject, UE nicht installiert)

---

## 1. Project Overview

### 1.1 Purpose
Blueprint-only Lernspiel für die 11 IHK-Zielvereinbarungsfragen (F1–F11). Kein C++, reine Blueprint-Architektur für schnelle Iteration und Designer-Zugriff.

### 1.2 Scope (v1)
- 11 Fragen (F1–F11) aus Zielvereinbarung
- 4 UMG Widgets (MainMenu, Question, Results, Stats)
- DataTable-gesteuerte Fragen
- SaveGame für Fortschritt
- WebGL Export für Netlify (docs/projektportal/ihk-lernspiel/)

### 1.3 Non-Goals (v1)
- Kein C++, kein GAS, kein Multiplayer
- Kein komplexes AI, kein Inventory
- Keine 3D-World, reines UI-Spiel

---

## 2. Architecture Principles (Blueprint Best Practices)

### 2.1 Blueprint Types Used
| Type | Purpose |
|------|---------|
| **Blueprint Class** | Alle wiederverwendbaren Actor (UI, GameMode, SaveGame) |
| **Widget Blueprint** | UMG UI (4 Screens) |
| **Blueprint Interface** | Kommunikation UI ↔ GameMode |
| **DataTable** | Fragen-Katalog (CSV → DT) |
| **Struct** | Frage-Schema, SaveGame-Daten |
| **GameMode** | Spiellogik, Zustandsmaschine |

### 2.2 Folder Structure
```
Content/
├── Blueprints/
│   ├── GameMode/
│   │   └── BP_GameMode_IHK.uasset
│   ├── UI/
│   │   ├── Menus/
│   │   │   └── WBP_MainMenu.uasset
│   │   ├── Gameplay/
│   │   │   ├── WBP_Question.uasset
│   │   │   ├── WBP_Results.uasset
│   │   │   └── WBP_Stats.uasset
│   │   └── Common/
│   │       └── WBP_QuestionCard.uasset
│   ├── Components/
│   │   └── BP_ProgressTracker.uasset
│   └── Interfaces/
│       └── BPI_Lernspiel.uasset
├── Data/
│   ├── DataTables/
│   │   └── DT_Questions.uasset
│   ├── Structs/
│   │   ├── ST_Question.uasset
│   │   └── ST_SaveData.uasset
│   └── Curves/
├── SaveGames/
│   └── BP_SaveGame_IHK.uasset
├── UI/
│   └── Fonts/ (optional)
└── Levels/
    └── L_MainMenu.umap
```

---

## 3. Data Schema

### 3.1 ST_Question (DataTable Row)
```cpp
struct ST_Question
{
    FName           QuestionID;       // F1, F2, ... F11
    FText           QuestionText;     // Volltext der Frage
    TArray<FText>   AnswerOptions;    // 4 Optionen (A-D)
    int32           CorrectIndex;     // 0-3
    FText           Explanation;      // Begründung bei Falsch
    FName           Category;         // Themenbereich
    int32           Difficulty;       // 1-3
    FText           Reference;        // Quelle (Skript-Seite, Gesetz)
};
```

### 3.2 DT_Questions CSV Import
```csv
QuestionID,QuestionText,OptionA,OptionB,OptionC,OptionD,CorrectIndex,Explanation,Category,Difficulty,Reference
F1,"Was versteht man unter...","Antwort A","Antwort B","Antwort C","Antwort D",1,"Begründung...","Projektmanagement",2,"Skript S.12"
```

### 3.3 ST_SaveData
```cpp
struct ST_SaveData
{
    TMap<FName, bool>   AnsweredQuestions;    // F1=true, F2=false...
    TMap<FName, int32>  UserAnswers;          // F1=1 (Index)
    int32               CorrectCount;
    int32               TotalAttempted;
    float               PlayTimeSeconds;
    FDateTime           LastPlayed;
    FString             PlayerName;
};
```

---

## 4. Gameplay Flow

### 4.1 State Machine (GameMode)
```
[MainMenu] 
    → Start Game → [QuestionScreen: F1]
    → Answer → [CheckAnswer] → Correct? → [Feedback] → Next Question
    → ... → F11 → [ResultsScreen]
    → [StatsScreen] → [MainMenu]
```

### 4.2 Question Flow
1. GameMode lädt nächste Frage aus DataTable
2. Widget zeigt Frage + 4 Buttons (A-D)
3. User klickt → Event Dispatcher `OnAnswerSelected(int32 Index)`
4. GameMode prüft → `CorrectIndex == Index`
5. SaveGame aktualisieren
5. Feedback anzeigen (Grün/Rot + Explanation)
6. Nach 3s → Nächste Frage oder Results

---

## 5. UI/UX Design (UMG)

### 5.1 WBP_MainMenu
- Titel: "IHK Projektleitung - Lernspiel"
- Buttons: "Start", "Statistiken", "Beenden"
- Version-Anzeige (unten rechts)
- Animiert: Fade-In

### 5.2 WBP_Question
- Progress Bar: "Frage 3 von 11"
- Frage-Text (zentriert, groß)
- 4 Answer Buttons (große Touch-Targets 64px min)
- Timer (optional, 120s)
- Keyboard-Navigation (Pfeiltasten + Enter)

### 5.3 WBP_Results
- Score: "8/11 richtig (73%)"
- Liste aller Fragen mit ✓/✗
- Button "Nochmal spielen", "Hauptmenü"

### 5.4 WBP_Stats
- Leitner-Style Boxen (Box 1-5 Visualisierung)
- Durchschnittliche Antwortzeit
- Schwächste Kategorien
- Lernempfehlungen

### 5.5 Accessibility (WCAG 2.2 AA)
- Kontrast 4.5:1
- Fokus-Indikatoren sichtbar
- Screen-Reader Labels
- Tastatur-Navigation komplett

---

## 6. Technical Implementation

### 6.1 GameMode (BP_GameMode_IHK)
**Variablen:**
- `CurrentQuestionIndex` (int)
- `QuestionsArray` (Array<ST_Question>)
- `SaveGameRef` (BP_SaveGame_IHK)
- `PlayerName` (String)

**Funktionen:**
- `LoadQuestions()` → DataTable → Array
- `GetNextQuestion()` → ST_Question
- `CheckAnswer(int32 Index)` → bool
- `SaveProgress()`
- `ResetGame()`

**Event Dispatchers:**
- `OnQuestionChanged(ST_Question)`
- `OnAnswerResult(bool Correct, FText Explanation)`
- `OnGameComplete(ST_SaveData)`

### 6.2 SaveGame (BP_SaveGame_IHK)
- `SaveSlotName` = "IHK_Lernspiel_Save"
- `UserIndex` = 0
- Funktionen: `Save()`, `Load()`, `Reset()`

### 6.3 DataTable Import (Python)
```python
# import_datatable.py
import unreal
import csv

# CSV → DataTable Asset erstellen
# In Editor: Right-Click DT_Questions → Reimport
```

### 6.4 Build Script (WebGL)
```bash
# build_webgl.sh
#!/bin/bash
UE_PATH="/mnt/nvme-data/UnrealEngine/UE_5.4.4"
PROJECT="/home/shadowmaker/UE5_IHK_Lernspiel/IHK_Lernspiel.uproject"
$UE_PATH/Engine/Build/BatchFiles/RunUAT.sh BuildCookRun \
  -project=$PROJECT \
  -platform=HTML5 \
  -configuration=Shipping \
  -targetplatform=HTML5 \
  -build -cook -pak -archive \
  -archivedirectory=/home/shadowmaker/ihk-projektarbeit-frs-archiv/docs/projektportal/ihk-lernspiel/
```

---

## 7. Communication Patterns

### 6.1 Blueprint Interface (BPI_Lernspiel)
```
Functions:
- GetCurrentQuestion() → ST_Question
- SubmitAnswer(int32 Index)
- GetProgress() → (Current, Total, Correct)
- ResetGame()
```

### 6.2 Event Dispatchers (GameMode → UI)
| Event | Payload | Listener |
|-------|---------|----------|
| OnQuestionChanged | ST_Question | WBP_Question |
| OnAnswerResult | bool, FText | WBP_Question |
| OnGameComplete | ST_SaveData | WBP_Results, WBP_Stats |
| OnProgressUpdated | int, int, int | WBP_Stats |

---

## 8. Performance & Optimization

| Area | Strategy |
|------|----------|
| **Tick** | Kein Event Tick! Timer für Countdown |
| **References** | Cachen in BeginPlay |
| **DataTable** | Einmal laden, Array cachen |
| **UI** | Widget Pooling (nicht Spawn/Destroy pro Frage) |
| **Textures** | Keine 4K für UI-Icons (max 512px) |
| **Soft Refs** | DataTable als Soft Object Reference |

---

## 9. Project Settings (IHK_Lernspiel.uproject)

```json
{
  "FileVersion": 3,
  "EngineAssociation": "5.4",
  "Category": "",
  "Description": "IHK Lernspiel für Zielvereinbarungsfragen F1-F11",
  "Modules": [
    {
      "Name": "IHK_Lernspiel",
      "Type": "Runtime",
      "LoadingPhase": "Default",
      "AdditionalDependencies": ["UMG", "DataTable", "Json", "Http"]
    }
  ],
  "Plugins": [
    {"Name": "WebBrowser", "Enabled": true}
  ],
  "TargetPlatforms": ["Windows", "Linux", "HTML5"]
}
```

### DefaultEngine.ini (Auszug)
```ini
[/Script/EngineSettings.GameMapsSettings]
GameDefaultMap=/Game/Levels/L_MainMenu
EditorStartupMap=/Game/Levels/L_MainMenu
GlobalDefaultGameMode=/Game/Blueprints/GameMode/BP_GameMode_IHK.BP_GameMode_IHK_C

[/Script/EngineSettings.GeneralProjectSettings]
ProjectID=XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
ProjectName=IHK Lernspiel
CompanyName=Shadowmaker
CopyrightNotice=2026 IHK Projektleitung

[/Script/HardwareTargeting.HardwareTargetingSettings]
TargetedHardwareClass=Desktop
AppliedTargetedHardwareClass=Desktop
DefaultGraphicsPerformance=Maximum
AppliedDefaultGraphicsPerformance=Maximum
```

---

## 10. Milestones & Gate Criteria

| Gate | Deliverable | Criteria |
|------|-------------|----------|
| **G0** | Project Setup | .uproject, Ordnerstruktur, Git init |
| **G1** | Data Schema | ST_Question, DT_Questions.csv, Import ok |
| **G2** | GameMode + SaveGame | Fragen laden, Save/Load, Reset |
| **G3** | UI Screens | 4 Widgets funktional, Navigation |
| **G4** | Gameplay Loop | F1→F11 durchspielbar, Feedback |
| **G5** | Polish | Timer, Animationen, Accessibility |
| **G6** | WebGL Build | HTML5 Export läuft auf Netlify |
| **G7** | IHK-Ready | Getestet, Dokumentation, Deploy |

**Gate Rules**: Nur GO/NO-GO/BLOCKED mit Beweis (Log, Screenshot, Artefakt). Kein "Prepared" = Success.

---

## 11. Dependencies & Environment

| Component | Version | Source |
|-----------|---------|--------|
| UE Engine | 5.4.4 | Epic Games Launcher → `/mnt/nvme-data/UnrealEngine/UE_5.4.4/` |
| Toolchain | clang 16.0.6 v22 | UE native (CentOS 7 sysroot) |
| OS Build | Ubuntu 24.04 / Mint 22.3 | Host |
| Deploy | Netlify | `docs/projektportal/` auto-deploy |

---

## 12. Risk Mitigation

| Risk | Likelihood | Impact | Mitigation |
|------|------------|--------|------------|
| UE 5.4.4 Linux Build bricht ab | Mittel | Hoch | Vorab Test-Build vor Gate 2 |
| WebGL Performance (Mobile) | Niedrig | Mittel | Profiling, Texture-Limits |
| DataTable Import Fehler | Niedrig | Hoch | Python-Script validiert CSV vor Import |
| IHK-Frist (2026-11-10) | - | Kritisch | Wöchentliche Reviews, Scope-Freeze G5 |

---

## 13. Appendix: Naming Conventions

| Asset Type | Prefix | Example |
|------------|--------|---------|
| Blueprint Class | `BP_` | `BP_GameMode_IHK` |
| Widget Blueprint | `WBP_` | `WBP_Question` |
| Blueprint Interface | `BPI_` | `BPI_Lernspiel` |
| DataTable | `DT_` | `DT_Questions` |
| Struct | `ST_` | `ST_Question` |
| Enum | `E_` | `E_QuestionCategory` |
| SaveGame | `BP_SaveGame_` | `BP_SaveGame_IHK` |
| Level | `L_` | `L_MainMenu` |
| Material | `M_` | `M_UI_Background` |
| Texture | `T_` | `T_Icon_Checkmark` |

---

**Next Action**: UE 5.4.4 via Epic Launcher installieren → Gate 1 (Project Setup)