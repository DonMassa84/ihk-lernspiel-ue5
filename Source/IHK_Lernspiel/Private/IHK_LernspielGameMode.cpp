// Copyright 2024-2026 Daniel Massa. All Rights Reserved.

#include "IHK_LernspielGameMode.h"
#include "IHK_LernspielSaveGame.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

AIHK_LernspielGameMode::AIHK_LernspielGameMode()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AIHK_LernspielGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    LoadQuestions();
    LoadSaveGame();
    
    CurrentGameState = EGameState::MainMenu;
}

void AIHK_LernspielGameMode::LoadQuestions()
{
    if (!QuestionsDataTable.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("QuestionsDataTable not set in GameMode!"));
        return;
    }

    UDataTable* DataTable = QuestionsDataTable.LoadSynchronous();
    if (!DataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load QuestionsDataTable!"));
        return;
    }

    LoadedQuestions.Empty();
    TArray<FName> RowNames = DataTable->GetRowNames();
    
    for (const FName& RowName : RowNames)
    {
        FQuestionRow* Row = DataTable->FindRow<FQuestionRow>(RowName, TEXT(""));
        if (Row)
        {
            LoadedQuestions.Add(*Row);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Loaded %d questions from DataTable"), LoadedQuestions.Num());
}

void AIHK_LernspielGameMode::FilterQuestionsByPriority(EQuestionPriority Priority)
{
    ActivePriorityFilter = Priority;
    FilteredQuestions.Empty();

    if (Priority == EQuestionPriority::All)
    {
        FilteredQuestions = LoadedQuestions;
    }
    else
    {
        int32 PriorityValue = static_cast<int32>(Priority);
        for (const FQuestionRow& Question : LoadedQuestions)
        {
            if (Question.Priority == PriorityValue)
            {
                FilteredQuestions.Add(Question);
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Filtered to %d questions (Priority: %d)"), FilteredQuestions.Num(), static_cast<int32>(Priority));
}

void AIHK_LernspielGameMode::ShuffleQuestions()
{
    for (int32 i = FilteredQuestions.Num() - 1; i > 0; --i)
    {
        int32 j = FMath::RandRange(0, i);
        FilteredQuestions.Swap(i, j);
    }
}

void AIHK_LernspielGameMode::LoadSaveGame()
{
    if (SaveGameClass)
    {
        SaveGameInstance = Cast<UIHK_LernspielSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlotName.ToString(), SaveGameUserIndex));
        if (!SaveGameInstance)
        {
            SaveGameInstance = Cast<UIHK_LernspielSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
            SaveGameInstance->SaveGameSlotName = SaveGameSlotName.ToString();
            SaveGameInstance->SaveGameUserIndex = SaveGameUserIndex;
        }
    }
}

void AIHK_LernspielGameMode::SaveProgress()
{
    if (SaveGameInstance)
    {
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameSlotName.ToString(), SaveGameUserIndex);
    }
}

void AIHK_LernspielGameMode::StartNewGame(EQuestionPriority PriorityFilter)
{
    CurrentGameState = EGameState::Playing;
    ActivePriorityFilter = PriorityFilter;
    CurrentQuestionIndex = 0;
    CorrectAnswers = 0;

    FilterQuestionsByPriority(PriorityFilter);
    ShuffleQuestions();

    if (FilteredQuestions.Num() > 0)
    {
        ShowNextQuestion();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No questions available for selected priority!"));
        CurrentGameState = EGameState::MainMenu;
    }

    UpdateProgress();
}

void AIHK_LernspielGameMode::ContinueGame()
{
    if (SaveGameInstance && SaveGameInstance->bHasProgress)
    {
        CurrentGameState = EGameState::Playing;
        CurrentQuestionIndex = SaveGameInstance->LastQuestionIndex;
        CorrectAnswers = SaveGameInstance->CorrectAnswersSoFar;
        ActivePriorityFilter = static_cast<EQuestionPriority>(SaveGameInstance->ActivePriorityFilter);
        
        FilterQuestionsByPriority(ActivePriorityFilter);
        
        if (CurrentQuestionIndex < FilteredQuestions.Num())
        {
            ShowNextQuestion();
        }
        else
        {
            StartNewGame(ActivePriorityFilter);
        }
    }
    else
    {
        StartNewGame(EQuestionPriority::All);
    }

    UpdateProgress();
}

void AIHK_LernspielGameMode::ShowNextQuestion()
{
    if (CurrentQuestionIndex < FilteredQuestions.Num())
    {
        const FQuestionRow& Question = FilteredQuestions[CurrentQuestionIndex];
        OnQuestionChanged.Broadcast(Question);
        CurrentGameState = EGameState::Playing;
    }
    else
    {
        FinishGame();
    }
}

void AIHK_LernspielGameMode::SubmitAnswer(int32 SelectedAnswerIndex)
{
    if (CurrentQuestionIndex >= FilteredQuestions.Num())
    {
        return;
    }

    const FQuestionRow& CurrentQuestion = FilteredQuestions[CurrentQuestionIndex];
    bool bIsCorrect = (SelectedAnswerIndex == CurrentQuestion.CorrectAnswerIndex);

    if (bIsCorrect)
    {
        CorrectAnswers++;
    }

    // Update SaveGame with spaced repetition data
    if (SaveGameInstance)
    {
        SaveGameInstance->UpdateQuestionProgress(
            CurrentQuestion.QuestionID,
            CurrentQuestion.Category,
            bIsCorrect,
            CurrentQuestion.Priority
        );
        SaveGameInstance->LastQuestionIndex = CurrentQuestionIndex;
        SaveGameInstance->CorrectAnswersSoFar = CorrectAnswers;
        SaveGameInstance->ActivePriorityFilter = static_cast<int32>(ActivePriorityFilter);
        SaveGameInstance->bHasProgress = true;
        SaveProgress();
    }

    OnAnswerSubmitted.Broadcast(bIsCorrect);
    CurrentGameState = EGameState::QuestionFeedback;

    // Show feedback for 3 seconds, then continue
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, [this]()
    {
        CurrentQuestionIndex++;
        ShowNextQuestion();
        UpdateProgress();
    }, 3.0f, false);
}

void AIHK_LernspielGameMode::SkipQuestion()
{
    SubmitAnswer(-1); // -1 = skipped/incorrect
}

void AIHK_LernspielGameMode::FinishGame()
{
    CurrentGameState = EGameState::Results;
    int32 TotalQuestions = FilteredQuestions.Num();
    int32 Percentage = (TotalQuestions > 0) ? (CorrectAnswers * 100 / TotalQuestions) : 0;

    if (SaveGameInstance)
    {
        SaveGameInstance->AddSessionResult(Percentage, CorrectAnswers, TotalQuestions, ActivePriorityFilter);
        SaveGameInstance->bHasProgress = false; // Reset for next game
        SaveProgress();
    }

    OnGameFinished.Broadcast(CorrectAnswers, TotalQuestions);
}

void AIHK_LernspielGameMode::ReturnToMainMenu()
{
    CurrentGameState = EGameState::MainMenu;
    CurrentQuestionIndex = 0;
    CorrectAnswers = 0;
    FilteredQuestions.Empty();
    OnProgressUpdated.Broadcast(0.0f);
}

void AIHK_LernspielGameMode::ShowStats()
{
    CurrentGameState = EGameState::Stats;
}

void AIHK_LernspielGameMode::UpdateProgress()
{
    float Progress = 0.0f;
    if (FilteredQuestions.Num() > 0)
    {
        Progress = static_cast<float>(CurrentQuestionIndex) / static_cast<float>(FilteredQuestions.Num());
    }
    OnProgressUpdated.Broadcast(Progress);
}

FQuestionRow AIHK_LernspielGameMode::GetCurrentQuestion() const
{
    if (CurrentQuestionIndex < FilteredQuestions.Num())
    {
        return FilteredQuestions[CurrentQuestionIndex];
    }
    return FQuestionRow();
}

float AIHK_LernspielGameMode::GetProgress() const
{
    if (FilteredQuestions.Num() > 0)
    {
        return static_cast<float>(CurrentQuestionIndex) / static_cast<float>(FilteredQuestions.Num());
    }
    return 0.0f;
}

int32 AIHK_LernspielGameMode::GetCurrentQuestionIndex() const
{
    return CurrentQuestionIndex + 1; // 1-based for display
}

int32 AIHK_LernspielGameMode::GetTotalQuestions() const
{
    return FilteredQuestions.Num();
}

int32 AIHK_LernspielGameMode::GetCorrectAnswers() const
{
    return CorrectAnswers;
}

EGameState AIHK_LernspielGameMode::GetCurrentGameState() const
{
    return CurrentGameState;
}

TArray<FName> AIHK_LernspielGameMode::GetAvailableCategories() const
{
    TSet<FName> Categories;
    for (const FQuestionRow& Question : LoadedQuestions)
    {
        Categories.Add(FName(*Question.Category));
    }
    return Categories.Array();
}

FString AIHK_LernspielGameMode::CalculateFeedbackMessage(int32 Percentage) const
{
    if (Percentage >= 90) return TEXT("Ausgezeichnet — Gesprächsreif!");
    if (Percentage >= 70) return TEXT("Gut — Noch 1-2 Fragen wiederholen");
    if (Percentage >= 50) return TEXT("Ausbaufähig — Leitfaden noch mal lesen");
    return TEXT("Nachholbedarf — Alle Dokumente durcharbeiten");
}