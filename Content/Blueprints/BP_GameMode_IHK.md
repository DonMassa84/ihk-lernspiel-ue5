/**
 * GameMode_Base — IHK Lernspiel
 * Blueprint: BP_GameMode_IHK (Parent: GameModeBase)
 * 
 * Variablen:
 * - DT_Questions (DataTable) — Reference to DT_Questions
 * - CurrentQuestionIndex (Integer) — Default: 0
 * - QuestionsArray (Array<FQuestionRow>) — Runtime cache
 * - CorrectAnswers (Integer) — Default: 0
 * - TotalQuestions (Integer) — Default: 11
 * - bGameFinished (Boolean) — Default: false
 * - SaveGameInstance (BP_SaveGame_IHK)
 * 
 * Funktionen:
 * - BeginPlay()
 *   -> Load DT_Questions into QuestionsArray
 *   -> Load SaveGame (progress, unlocked topics)
 *   -> Open MainMenu Widget
 * 
 * - StartNewGame()
 *   -> CurrentQuestionIndex = 0
 *   -> CorrectAnswers = 0
 *   -> Shuffle QuestionsArray (optional)
 *   -> Open QuestionWidget
 * 
 * - AnswerQuestion(bIsCorrect)
 *   -> If bIsCorrect: CorrectAnswers++
 *   -> CurrentQuestionIndex++
 *   -> If CurrentQuestionIndex >= TotalQuestions: GameFinished()
 *   -> Else: ShowNextQuestion()
 * 
 * - GameFinished()
 *   -> bGameFinished = true
 *   -> Calculate Score (Percentage)
 *   -> Save Progress to SaveGame
 *   -> Open ResultsWidget
 * 
 * - GetCurrentQuestion() -> FQuestionRow
 *   -> Return QuestionsArray[CurrentQuestionIndex]
 * 
 * - GetProgress() -> Float (0.0-1.0)
 *   -> Return CurrentQuestionIndex / TotalQuestions
 */

// FQuestionRow Struct (matches CSV columns)
struct FQuestionRow
{
    FName QuestionID;        // F1, F2, ...
    int32 Priority;          // 1, 2, 3
    FString Category;        // Scope, Type, Documentation, Timeline, Focus, Formal, DataAccess, Coach, ChangeControl, Evaluation
    FString QuestionText;    // Full question
    FString ModelAnswer;     // Expected answer
    FString Keywords;        // Semicolon-separated
    int32 Difficulty;        // 1-3
};