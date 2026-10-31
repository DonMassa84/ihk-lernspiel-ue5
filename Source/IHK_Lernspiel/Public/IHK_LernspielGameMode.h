// Copyright 2024-2026 Daniel Massa. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IHK_LernspielGameMode.generated.h"

class UIHK_LernspielSaveGame;
class UDataTable;
struct FQuestionRow;

UENUM(BlueprintType)
enum class EQuestionPriority : uint8
{
    Prio1_Critical    UMETA(DisplayName = "Priorität 1 - Kritisch"),
    Prio2_Important   UMETA(DisplayName = "Priorität 2 - Wichtig"),
    Prio3_NiceToHave  UMETA(DisplayName = "Priorität 3 - Nice to Have"),
    All               UMETA(DisplayName = "Alle Prioritäten")
};

UENUM(BlueprintType)
enum class EGameState : uint8
{
    MainMenu,
    Playing,
    QuestionFeedback,
    Results,
    Stats
};

USTRUCT(BlueprintType)
struct FQuestionRow
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
    FName QuestionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
    int32 Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
    FString Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question", Meta = (MultiLine = "true"))
    FString QuestionText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question", Meta = (MultiLine = "true"))
    FString ModelAnswer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
    TArray<FString> AnswerOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
    int32 CorrectAnswerIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question", Meta = (MultiLine = "true"))
    FString Hint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
    int32 Points;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
    int32 Difficulty;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestionChanged, const FQuestionRow&, NewQuestion);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnswerSubmitted, bool, bIsCorrect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameFinished, int32, CorrectAnswers, int32, TotalQuestions);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProgressUpdated, float, Progress);

UCLASS(Blueprintable, BlueprintType)
class IHK_LERNSPIEL_API AIHK_LernspielGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AIHK_LernspielGameMode();

    virtual void BeginPlay() override;

    // Game Flow
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void StartNewGame(EQuestionPriority PriorityFilter = EQuestionPriority::All);

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void ContinueGame();

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void SubmitAnswer(int32 SelectedAnswerIndex);

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void SkipQuestion();

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void ReturnToMainMenu();

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void ShowStats();

    // Data Access
    UFUNCTION(BlueprintCallable, Category = "Data")
    FQuestionRow GetCurrentQuestion() const;

    UFUNCTION(BlueprintCallable, Category = "Data")
    float GetProgress() const;

    UFUNCTION(BlueprintCallable, Category = "Data")
    int32 GetCurrentQuestionIndex() const;

    UFUNCTION(BlueprintCallable, Category = "Data")
    int32 GetTotalQuestions() const;

    UFUNCTION(BlueprintCallable, Category = "Data")
    int32 GetCorrectAnswers() const;

    UFUNCTION(BlueprintCallable, Category = "Data")
    EGameState GetCurrentGameState() const;

    UFUNCTION(BlueprintCallable, Category = "Data")
    TArray<FName> GetAvailableCategories() const;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnQuestionChanged OnQuestionChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnAnswerSubmitted OnAnswerSubmitted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnGameFinished OnGameFinished;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnProgressUpdated OnProgressUpdated;

protected:
    // Configuration
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration")
    TSoftObjectPtr<UDataTable> QuestionsDataTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration")
    TSubclassOf<UIHK_LernspielSaveGame> SaveGameClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration")
    FName SaveGameSlotName = TEXT("IHK_Lernspiel_Save");

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration")
    int32 SaveGameUserIndex = 0;

    // Runtime State
    UPROPERTY(Transient)
    TArray<FQuestionRow> LoadedQuestions;

    UPROPERTY(Transient)
    TArray<FQuestionRow> FilteredQuestions;

    UPROPERTY(Transient)
    int32 CurrentQuestionIndex = 0;

    UPROPERTY(Transient)
    int32 CorrectAnswers = 0;

    UPROPERTY(Transient)
    EGameState CurrentGameState = EGameState::MainMenu;

    UPROPERTY(Transient)
    EQuestionPriority ActivePriorityFilter = EQuestionPriority::All;

    UPROPERTY(Transient)
    UIHK_LernspielSaveGame* SaveGameInstance = nullptr;

    // Internal Functions
    void LoadQuestions();
    void FilterQuestionsByPriority(EQuestionPriority Priority);
    void ShuffleQuestions();
    void LoadSaveGame();
    void SaveProgress();
    void ShowNextQuestion();
    void ShowFeedback(bool bIsCorrect);
    void FinishGame();
    void UpdateProgress();
    FString CalculateFeedbackMessage(int32 Percentage) const;
};