// Copyright 2024-2026 Daniel Massa. All Rights Reserved.

#include "IHK_LernspielSaveGame.h"
#include "Misc/DateTime.h"

UIHK_LernspielSaveGame::UIHK_LernspielSaveGame()
{
    bHasProgress = false;
    LastQuestionIndex = 0;
    CorrectAnswersSoFar = 0;
    ActivePriorityFilter = 0;
    SaveGameSlotName = TEXT("IHK_Lernspiel_Save");
    SaveGameUserIndex = 0;
    TotalSessions = 0;
    BestScore = 0;
    AverageScore = 0.0f;
    LastPlayedDate = FDateTime::Now();
}

void UIHK_LernspielSaveGame::UpdateQuestionProgress(FName QuestionID, FString Category, bool bCorrect, int32 Priority)
{
    FQuestionProgress* Progress = QuestionProgressMap.Find(QuestionID);
    if (!Progress)
    {
        FQuestionProgress NewProgress;
        NewProgress.QuestionID = QuestionID;
        NewProgress.Category = Category;
        NewProgress.Priority = Priority;
        NewProgress.Repetitions = 0;
        NewProgress.EaseFactor = 2.5f;
        NewProgress.IntervalDays = 0;
        NewProgress.NextReviewDate = FDateTime::Now();
        NewProgress.LastGrade = 0;
        NewProgress.TimesAsked = 0;
        NewProgress.TimesCorrect = 0;
        NewProgress.BestPercentage = 0.0f;
        QuestionProgressMap.Add(QuestionID, NewProgress);
        Progress = QuestionProgressMap.Find(QuestionID);
    }

    if (Progress)
    {
        Progress->TimesAsked++;
        if (bCorrect)
        {
            Progress->TimesCorrect++;
        }
        Progress->LastGrade = bCorrect ? 4 : 1;
        Progress->BestPercentage = FMath::Max(Progress->BestPercentage, 
            (Progress->TimesAsked > 0) ? (static_cast<float>(Progress->TimesCorrect) / Progress->TimesAsked * 100.0f) : 0.0f);
        
        // SM-2 Spaced Repetition Update
        UpdateSpacedRepetition(QuestionID, Progress->LastGrade);
    }

    // Update Category Stats
    FQuestionProgress* CatProgress = CategoryStats.Find(Category);
    if (!CatProgress)
    {
        FQuestionProgress NewCatProgress;
        NewCatProgress.QuestionID = QuestionID;
        NewCatProgress.Category = Category;
        NewCatProgress.Priority = Priority;
        NewCatProgress.TimesAsked = 0;
        NewCatProgress.TimesCorrect = 0;
        CategoryStats.Add(Category, NewCatProgress);
        CatProgress = CategoryStats.Find(Category);
    }

    if (CatProgress)
    {
        CatProgress->TimesAsked++;
        if (bCorrect)
        {
            CatProgress->TimesCorrect++;
        }
    }
}

void UIHK_LernspielSaveGame::UpdateSpacedRepetition(FName QuestionID, int32 Grade)
{
    FQuestionProgress* Progress = QuestionProgressMap.Find(QuestionID);
    if (!Progress)
    {
        return;
    }

    // SM-2 Algorithm
    if (Grade >= 3)
    {
        if (Progress->Repetitions == 0)
        {
            Progress->IntervalDays = 1;
        }
        else if (Progress->Repetitions == 1)
        {
            Progress->IntervalDays = 6;
        }
        else
        {
            Progress->IntervalDays = FMath::RoundToInt(Progress->IntervalDays * Progress->EaseFactor);
        }
        Progress->Repetitions++;
    }
    else
    {
        Progress->Repetitions = 0;
        Progress->IntervalDays = 1;
    }

    // Update EaseFactor
    Progress->EaseFactor = Progress->EaseFactor + (0.1f - (5 - Grade) * (0.08f + (5 - Grade) * 0.02f));
    if (Progress->EaseFactor < 1.3f)
    {
        Progress->EaseFactor = 1.3f;
    }

    Progress->NextReviewDate = FDateTime::Now() + FTimespan::FromDays(Progress->IntervalDays);
}

void UIHK_LernspielSaveGame::AddSessionResult(int32 Percentage, int32 Correct, int32 Total, int32 PriorityFilter)
{
    FSessionResult Result;
    Result.Date = FDateTime::Now();
    Result.Percentage = Percentage;
    Result.CorrectAnswers = Correct;
    Result.TotalQuestions = Total;
    Result.PriorityFilter = PriorityFilter;

    SessionHistory.Add(Result);
    TotalSessions++;

    // Update Best Score
    if (Percentage > BestScore)
    {
        BestScore = Percentage;
    }

    // Update Average Score
    if (TotalSessions > 0)
    {
        int32 TotalPercentage = 0;
        for (const FSessionResult& Session : SessionHistory)
        {
            TotalPercentage += Session.Percentage;
        }
        AverageScore = static_cast<float>(TotalPercentage) / TotalSessions;
    }

    LastPlayedDate = FDateTime::Now();
}

FQuestionProgress UIHK_LernspielSaveGame::GetQuestionProgress(FName QuestionID) const
{
    const FQuestionProgress* Progress = QuestionProgressMap.Find(QuestionID);
    if (Progress)
    {
        return *Progress;
    }
    return FQuestionProgress();
}

FQuestionProgress UIHK_LernspielSaveGame::GetCategoryStats(FString Category) const
{
    const FQuestionProgress* Stats = CategoryStats.Find(Category);
    if (Stats)
    {
        return *Stats;
    }
    return FQuestionProgress();
}

void UIHK_LernspielSaveGame::GetOverallStats(int32& OutSessions, int32& OutBestScore, float& OutAvgScore) const
{
    OutSessions = TotalSessions;
    OutBestScore = BestScore;
    OutAvgScore = AverageScore;
}

void UIHK_LernspielSaveGame::ResetAllProgress()
{
    bHasProgress = false;
    LastQuestionIndex = 0;
    CorrectAnswersSoFar = 0;
    ActivePriorityFilter = 0;
    QuestionProgressMap.Empty();
    CategoryStats.Empty();
    SessionHistory.Empty();
    TotalSessions = 0;
    BestScore = 0;
    AverageScore = 0.0f;
    LastPlayedDate = FDateTime::Now();
}

TArray<FName> UIHK_LernspielSaveGame::GetDueQuestions() const
{
    TArray<FName> DueQuestions;
    FDateTime Now = FDateTime::Now();

    for (const auto& Pair : QuestionProgressMap)
    {
        if (Pair.Value.NextReviewDate <= Now)
        {
            DueQuestions.Add(Pair.Key);
        }
    }

    return DueQuestions;
}
