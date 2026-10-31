/**
 * Widget_MainMenu — IHK Lernspiel
 * Blueprint: WBP_MainMenu (Parent: UserWidget)
 * 
 * UI Hierarchie:
 * - CanvasPanel
 *   - Image_Background (Fullscreen, FRS-Blau #1a3c5e)
 *   - VerticalBox_Centered (Alignment: Center)
 *     - Text_Title: "IHK-Beratung 10.11.2026"
 *     - Text_Subtitle: "Zielvereinbarungs-Fragen Trainer"
 *     - Text_Info: "11 Fragen | 3 Prioritäten | PRE_PROJECT"
 *     - Button_Start: "Neues Spiel starten"
 *     - Button_Continue: "Fortsetzen" (Visibility: Bound to SaveGame.bHasProgress)
 *     - Button_Stats: "Statistiken"
 *     - Button_Quit: "Beenden"
 * 
 * Events:
 * - OnStartClicked -> GameMode->StartNewGame()
 * - OnContinueClicked -> GameMode->LoadProgress() -> Open QuestionWidget
 * - OnStatsClicked -> Open StatsWidget
 * - OnQuitClicked -> Quit Game
 */

 /**
 * Widget_Question — IHK Lernspiel
 * Blueprint: WBP_Question (Parent: UserWidget)
 * 
 * UI Hierarchie:
 * - CanvasPanel
 *   - ProgressBar_Top (Bind: GameMode->GetProgress)
 *   - Text_Progress: "Frage {Current}/{Total} | Priorität {Priority}"
 *   - Text_Category: "Kategorie: {Category}"
 *   - MultiLineText_Question: "{QuestionText}" (Wrap, Auto-Size)
 *   - VerticalBox_Answers
 *     - Button_Answer_1: "Antwort eingeben..." (TextInput)
 *     - Button_Submit: "Prüfen"
 *     - Button_Skip: "Überspringen"
 *   - Text_Feedback (Visibility: Hidden initially)
 *   - Text_ModelAnswer (Visibility: Hidden initially)
 * 
 * Events:
 * - OnSubmitClicked
 *   -> Get UserInput from TextInput
 *   -> Compare with ModelAnswer (Keyword-Matching + Fuzzy)
 *   -> Show Feedback (Richtig/Falsch)
 *   -> Show ModelAnswer
 *   -> Delay 2s -> GameMode->AnswerQuestion(bIsCorrect)
 * 
 * - OnSkipClicked -> GameMode->AnswerQuestion(false)
 * 
 * - Construct() -> Bind GameMode->GetCurrentQuestion() to UI texts
 */

 /**
 * Widget_Results — IHK Lernspiel
 * Blueprint: WBP_Results (Parent: UserWidget)
 * 
 * UI Hierarchie:
 * - CanvasPanel
 *   - Text_Result: "Ergebnis: {CorrectAnswers}/11 ({Percentage}%)"
 *   - Text_Feedback: (Basierend auf Percentage)
 *     - 90-100%: "Ausgezeichnet — Gesprächsreif!"
 *     - 70-89%: "Gut — Noch 1-2 Fragen wiederholen"
 *     - 50-69%: "Ausbaufähig — Leitfaden noch mal lesen"
 *     - <50%: "Nachholbedarf — Alle Dokumente durcharbeiten"
 *   - ListView_QuestionReview (Scrollable)
 *     - Entry: QuestionID | Kategorie | Richtig/Falsch | Deine Antwort | Musterantwort
 *   - Button_Retry: "Nochmal spielen"
 *   - Button_Menu: "Hauptmenü"
 * 
 * Events:
 * - OnRetryClicked -> GameMode->StartNewGame()
 * - OnMenuClicked -> Open MainMenu
 */

 /**
 * Widget_Stats — IHK Lernspiel
 * Blueprint: WBP_Stats (Parent: UserWidget)
 * 
 * UI Hierarchie:
 * - CanvasPanel
 *   - Text_Title: "Lernstatistiken"
 *   - ProgressBar_Overall: "Gesamtfortschritt"
 *   - Text_Sessions: "Spiele: {Count} | Best Score: {Best}% | Avg: {Avg}%"
 *   - ListView_CategoryStats
 *     - Entry: Kategorie | Fragen | Richtig | Quote
 *   - Button_Reset: "Fortschritt zurücksetzen" (Confirmation Dialog)
 *   - Button_Back: "Zurück"
 */