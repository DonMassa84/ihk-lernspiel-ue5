// Copyright 2024-2026 Daniel Massa. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "IHK_LernspielSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FQuestionProgress
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
    FName QuestionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
    FString Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
    int32 Priority;

    // SM-2 Spaced Repetition
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spaced Repetition")
    int32 Repetitions = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spaced Repetition")
    float EaseFactor = 2.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spaced Repetition")
    int32 IntervalDays = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spaced Repetition")
    FDateTime NextReviewDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spaced Repetition")
    int32 LastGrade = 0; // 0-5

    // Session Stats
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 TimesAsked = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 TimesCorrect = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float BestPercentage = 0.0f;
};

USTRUCT(BlueprintType)
struct FSessionResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session")
    FDateTime Date;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session")
    int32 Percentage = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session")
    int32 CorrectAnswers = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session")
    int32 TotalQuestions = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session")
    int32 PriorityFilter = 0; // EQuestionPriority as int
};

UCLASS(Blueprintable, BlueprintType)
class IHK_LERNSPIEL_API UIHK_LernspielSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UIHK_LernspielSaveGame();

    // Progress Tracking
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress", SaveGame)
    bool bHasProgress = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress", SaveGame)
    int32 LastQuestionIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress", SaveGame)
    int32 CorrectAnswersSoFar = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress", SaveGame)
    int32 ActivePriorityFilter = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress", SaveGame)
    FString SaveGameSlotName = TEXT("IHK_Lernspiel_Save");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress", SaveGame)
    int32 SaveGameUserIndex = 0;

    // Per-Question Progress (SM-2)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress", SaveGame)
    TMap<FName, FQuestionProgress> QuestionProgressMap;

    // Category Stats
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", SaveGame)
    TMap<FString, FQuestionProgress> CategoryStats;

    // Session History
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", SaveGame)
    TArray<FSessionResult> SessionHistory;

    // Overall Stats
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", SaveGame)
    int32 TotalSessions = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", SaveGame)
    int32 BestScore = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", SaveGame)
    float AverageScore = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", SaveGame)
    FDateTime LastPlayedDate;

    // Functions
    UFUNCTION(BlueprintCallable, Category = "Progress")
    void UpdateQuestionProgress(FName QuestionID, FString Category, bool bCorrect, int32 Priority);

    UFUNCTION(BlueprintCallable, Category = "Progress")
    void UpdateSpacedRepetition(FName QuestionID, int32 Grade); // 0-5 SM-2 grade

    UFUNCTION(BlueprintCallable, Category = "Progress")
    void AddSessionResult(int32 Percentage, int32 Correct, int32 Total, int32 PriorityFilter);

    UFUNCTION(BlueprintCallable, Category = "Stats")
    FQuestionProgress GetQuestionProgress(FName QuestionID) const;

    UFUNCTION(BlueprintCallable, Category = "Stats")
    FQuestionProgress GetCategoryStats(FString Category) const;

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void GetOverallStats(int32& OutSessions, int32& OutBestScore, float& OutAvgScore) const;

    UFUNCTION(BlueprintCallable, Category = "Progress")
    void ResetAllProgress();

    UFUNCTION(BlueprintCallable, Category = "Progress")
    TArray<FName> GetDueQuestions() const; // Questions where NextReviewDate <= Now
};