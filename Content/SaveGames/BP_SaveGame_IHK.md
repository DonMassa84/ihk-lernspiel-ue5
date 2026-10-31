/**
 * SaveGame_IHK — IHK Lernspiel
 * Blueprint: BP_SaveGame_IHK (Parent: SaveGame)
 * 
 * Variablen:
 * - bHasProgress (Boolean) — Default: false
 * - BestScore (Integer) — Default: 0
 * - TotalSessions (Integer) — Default: 0
 * - CategoryProgress (Map<FString, FCategoryProgress>)
 * - LastPlayedDate (FDateTime)
 * - UnlockedTopics (Array<FName>) — Default: ["F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11"]
 * 
 * Struct FCategoryProgress:
 *   - QuestionsTotal (Integer)
 *   - QuestionsCorrect (Integer)
 *   - BestPercentage (Float)
 * 
 * Funktionen:
 * - UpdateProgress(QuestionID, Category, bCorrect)
 *   -> CategoryProgress[Category].QuestionsTotal++
 *   -> If bCorrect: CategoryProgress[Category].QuestionsCorrect++
 *   -> Recalculate BestPercentage
 *   -> If SessionFinished: TotalSessions++, Update BestScore
 * 
 * - GetCategoryStats(Category) -> FCategoryProgress
 * - GetOverallStats() -> (Sessions, BestScore, AvgScore)
 * - ResetAll() -> Clear all, bHasProgress = false
 */

// Blueprint Event Graph:
// - SaveGame Slot Name: "IHK_Lernspiel_Save"
// - User Index: 0