// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerController.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Blaster/BlasterCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/GameMode/BlasterGameMode.h"
#include "Blaster/HUD/Announcement.h"
#include "Kismet/GameplayStatics.h"
#include "Blaster//BlasterComponents/CombatComponent.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Components/image.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blaster//HUD//ReturnToMainMenu.h"
#include "Blaster/BlasterTypes/Announcement.h"

void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	BlasterHUD = Cast<ABlasterHUD>(GetHUD());
	ServerCheckMatchState(); // tomato

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MenuMappingContext, 0);
	}
	//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	//bool bIsServer = HasAuthority();
	//UE_LOG(LogTemp, Warning, TEXT("Running on: %s"), bIsServer ? TEXT("Server") : TEXT("Client"));

	//bool bIsBlasterHUDValid = BlasterHUD != nullptr;
	//UE_LOG(LogTemp, Warning, TEXT("BlasterHUD Validity: %s"), bIsBlasterHUDValid ? TEXT("True") : TEXT("False"));

	//bool bIsCharacterOverlayValid = bIsBlasterHUDValid && BlasterHUD->CharacterOverlay != nullptr;
	//UE_LOG(LogTemp, Warning, TEXT("CharacterOverlay Validity: %s"), bIsCharacterOverlayValid ? TEXT("True") : TEXT("False"));

	//bool bIsRedTeamScoreValid = bIsCharacterOverlayValid && BlasterHUD->CharacterOverlay->RedTeamScore != nullptr;
	//UE_LOG(LogTemp, Warning, TEXT("RedTeamScore Validity: %s"), bIsRedTeamScoreValid ? TEXT("True") : TEXT("False"));

	//bool bIsBlueTeamScoreValid = bIsCharacterOverlayValid && BlasterHUD->CharacterOverlay->BlueTeamScore != nullptr;
	//UE_LOG(LogTemp, Warning, TEXT("BlueTeamScore Validity: %s"), bIsBlueTeamScoreValid ? TEXT("True") : TEXT("False"));


}

void ABlasterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent == nullptr) return;

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ReturnMainMenuAction, ETriggerEvent::Started, this, &ABlasterPlayerController::ShowReturnToMainMenu);
	}
}

void ABlasterPlayerController::ShowReturnToMainMenu()
{
	if (ReturnToMainMenuWidget == nullptr) return;
	if (ReturnToMainMenu == nullptr)
	{
		ReturnToMainMenu = CreateWidget<UReturnToMainMenu>(this, ReturnToMainMenuWidget);
	}
	if (ReturnToMainMenu)
	{
		bReturnToMainMenuOpen = !bReturnToMainMenuOpen;
		if (bReturnToMainMenuOpen)
		{
			ReturnToMainMenu->MenuSetup();
		}
		else
		{
			ReturnToMainMenu->MenuTearDown();
		}
	}
}

void ABlasterPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABlasterPlayerController, MatchState);
	DOREPLIFETIME(ABlasterPlayerController, bShowTeamScores);
}

void ABlasterPlayerController::HideTeamScores()
{
	//tomato
	//FString RoleText = (GetLocalRole() == ROLE_Authority) ? TEXT("Server") : TEXT("Client"); // tomato
	//UE_LOG(LogTemp, Warning, TEXT("HideTeamScores called on: %s"), *RoleText);

	//bool bIsBlasterHUDValid = BlasterHUD != nullptr;
	//UE_LOG(LogTemp, Warning, TEXT("BlasterHUD Validity: %s"), bIsBlasterHUDValid ? TEXT("True") : TEXT("False"));

 //   bool bIsCharacterOverlayValid = bIsBlasterHUDValid && BlasterHUD->CharacterOverlay != nullptr;
 //   UE_LOG(LogTemp, Warning, TEXT("CharacterOverlay Validity: %s"), bIsCharacterOverlayValid ? TEXT("True") : TEXT("False"));

 //   bool bIsRedTeamScoreValid = bIsCharacterOverlayValid && BlasterHUD->CharacterOverlay->RedTeamScore != nullptr;
 //   UE_LOG(LogTemp, Warning, TEXT("RedTeamScore Validity: %s"), bIsRedTeamScoreValid ? TEXT("True") : TEXT("False"));

 //   bool bIsBlueTeamScoreValid = bIsCharacterOverlayValid && BlasterHUD->CharacterOverlay->BlueTeamScore != nullptr;
 //   UE_LOG(LogTemp, Warning, TEXT("BlueTeamScore Validity: %s"), bIsBlueTeamScoreValid ? TEXT("True") : TEXT("False"));


	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->RedTeamScore &&
		BlasterHUD->CharacterOverlay->BlueTeamScore;

	if (bHUDValid)
	{
		//FString RoleTextTwo = (GetLocalRole() == ROLE_Authority) ? TEXT("Server") : TEXT("Client"); // tomato
		//UE_LOG(LogTemp, Warning, TEXT("HideTeamScores bHUDisValid line 106: %s"), *RoleTextTwo);

		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText());
		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText());
	}
}

//void ABlasterPlayerController::InitTeamScores()
//{
//
//	//tomato
//	//FString RoleText = (GetLocalRole() == ROLE_Authority) ? TEXT("Server") : TEXT("Client"); // tomato
//	//UE_LOG(LogTemp, Warning, TEXT("InitTeamScores called on: %s"), *RoleText);
//
//	//bool bIsBlasterHUDValid = BlasterHUD != nullptr;
//	//UE_LOG(LogTemp, Warning, TEXT("BlasterHUD Validity: %s"), bIsBlasterHUDValid ? TEXT("True") : TEXT("False"));
//
//	//bool bIsCharacterOverlayValid = bIsBlasterHUDValid && BlasterHUD->CharacterOverlay != nullptr;
//	//UE_LOG(LogTemp, Warning, TEXT("CharacterOverlay Validity: %s"), bIsCharacterOverlayValid ? TEXT("True") : TEXT("False"));
//
//	//bool bIsRedTeamScoreValid = bIsCharacterOverlayValid && BlasterHUD->CharacterOverlay->RedTeamScore != nullptr;
//	//UE_LOG(LogTemp, Warning, TEXT("RedTeamScore Validity: %s"), bIsRedTeamScoreValid ? TEXT("True") : TEXT("False"));
//
//	//bool bIsBlueTeamScoreValid = bIsCharacterOverlayValid && BlasterHUD->CharacterOverlay->BlueTeamScore != nullptr;
//	//UE_LOG(LogTemp, Warning, TEXT("BlueTeamScore Validity: %s"), bIsBlueTeamScoreValid ? TEXT("True") : TEXT("False"));
//
//	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
//	bool bHUDValid = BlasterHUD &&
//		BlasterHUD->CharacterOverlay &&
//		BlasterHUD->CharacterOverlay->RedTeamScore &&
//		BlasterHUD->CharacterOverlay->BlueTeamScore;
//
//	if (bHUDValid)
//	{
//		//FString RoleTextTwo = (GetLocalRole() == ROLE_Authority) ? TEXT("Server") : TEXT("Client"); // tomato
//		//UE_LOG(LogTemp, Warning, TEXT("InitTeamScores bHUDisValid line 106: %s"), *RoleTextTwo);
//
//		FString Zero = "0";
//		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText::FromString(Zero));
//		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText::FromString(Zero));
//	}
//}

void ABlasterPlayerController::SetHUDRedTeamScore(int32 RedScore)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->RedTeamScore;

	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), RedScore);
		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText::FromString(ScoreText));
	}
}

void ABlasterPlayerController::SetHUDBlueTeamScore(int32 BlueScore)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->BlueTeamScore;

	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), BlueScore);
		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText::FromString(ScoreText));
	}
}


void ABlasterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(InPawn);

	if (BlasterCharacter)
	{
		SetHUDHealth(BlasterCharacter->GetHealth(), BlasterCharacter->GetMaxHealth());
	}
}

void ABlasterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();
	CheckTimeSync(DeltaTime);
	PollInit();
	CheckPing(DeltaTime);

	//DebugNumber += DeltaTime;
	//if (DebugNumber > 5.f)
	//{
	//	if (HasAuthority())
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("tick test"));
	//		bShowTeamScores = !bShowTeamScores;
	//		DebugNumber = 0.f;
	//	}
	//}
}

void ABlasterPlayerController::PollInit()
{
	if (CharacterOverlay == nullptr)
	{
		if (BlasterHUD && BlasterHUD->CharacterOverlay)
		{
			CharacterOverlay = BlasterHUD->CharacterOverlay;
			if (CharacterOverlay)
			{
				if (bInitializeHealth) SetHUDHealth(HUDHealth, HUDMaxHealth);
				if (bInitializeShield) SetHUDShield(HUDShield, HUDMaxShield);
				if (bInitializeScore) SetHUDScore(HUDScore);
				if (bInitializeDeaths) SetHUDDeaths(HUDDeaths);
				if (bInitializeCarriedAmmo) SetHUDCarriedAmmo(HUDCarriedAmmo);
				if (bInitializeWeaponAmmo) SetHUDWeaponAmmo(HUDWeaponAmmo);
				if (bInitializeTeamScores) InitTeamScores(); //added if update team score fails

				ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn());
				if (BlasterCharacter && BlasterCharacter->GetCombat())
				{
					if (bInitializeGrenades) SetHUDGrenades(BlasterCharacter->GetCombat()->GetGrenades());
				}
				BlasterHUD->CharacterOverlay->HighPingImage->SetOpacity(0.f);
			}
		}
	}
}

void ABlasterPlayerController::CheckPing(float DeltaTime)
{
	if (HasAuthority()) return;
	HighPingRunningTime += DeltaTime;
	if (HighPingRunningTime > CheckPingFrequency)
	{
		PlayerState = PlayerState == nullptr ? GetPlayerState<APlayerState>() : PlayerState;
		if (PlayerState)
		{
			//UE_LOG(LogTemp, Warning, TEXT("PlayerState->GetPing() * 4 : %d"), PlayerState->GetCompressedPing() * 4);
			if (PlayerState->GetCompressedPing() * 4 > HighPingThreshold) // compressed value, roughly 1/4
			{
				HighPingWarning();
				PingAnimationRunningTime = 0.f;
				ServerReportPingStatus(true);
			}
			else
			{
				ServerReportPingStatus(false);

			}
		}
		HighPingRunningTime = 0.f;
	}

	bool bHighPingAnimationPlaying =
		BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HighPingAnimation &&
		BlasterHUD->CharacterOverlay->IsAnimationPlaying(BlasterHUD->CharacterOverlay->HighPingAnimation);
	if (bHighPingAnimationPlaying)
	{
		PingAnimationRunningTime += DeltaTime;
		if (PingAnimationRunningTime > HighPingDuration)
		{
			StopHighPingWarning();
		}
	}
}

// if ping is too high, this gets activated
void ABlasterPlayerController::ServerReportPingStatus_Implementation(bool bHighPing) 
{
	HighPingDelegate.Broadcast(bHighPing);
}

void ABlasterPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void ABlasterPlayerController::HighPingWarning()
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	//BigDebugLog();
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HighPingImage &&
		BlasterHUD->CharacterOverlay->HighPingAnimation;

	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->HighPingImage->SetOpacity(1.f);
		BlasterHUD->CharacterOverlay->PlayAnimation(
			BlasterHUD->CharacterOverlay->HighPingAnimation,
			0.f,
			5
		);
	}
}
void ABlasterPlayerController::StopHighPingWarning()
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HighPingImage &&
		BlasterHUD->CharacterOverlay->HighPingAnimation;

	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->HighPingImage->SetOpacity(0.f);
		if (BlasterHUD->CharacterOverlay->IsAnimationPlaying(BlasterHUD->CharacterOverlay->HighPingAnimation))
		{
			BlasterHUD->CharacterOverlay->StopAnimation(BlasterHUD->CharacterOverlay->HighPingAnimation);
		}
	}
}

void ABlasterPlayerController::ServerCheckMatchState_Implementation()
{
	ABlasterGameMode* GameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		LevelStartingTime = GameMode->LevelStartingTime;
		WarmupTime = GameMode->WarmupTime;
		MatchTime = GameMode->MatchTime;
		CooldownTime = GameMode->CooldownTime;
		MatchState = GameMode->GetMatchState(); // tomato  so this line pings the next function
		ClientJoinMidGame(MatchState, LevelStartingTime, WarmupTime, MatchTime, CooldownTime);
	}
}

void ABlasterPlayerController::ClientJoinMidGame_Implementation(FName StateOfMatch, float StartingTime, float Warmup, float Match, float Cooldown)
{
	LevelStartingTime = StartingTime;
	WarmupTime = Warmup;
	MatchTime = Match;
	CooldownTime = Cooldown;
	MatchState = StateOfMatch;
	OnMatchStateSet(MatchState); //        this then pings OnMatchStateSet, which is already being pinged on beginplay
	if (BlasterHUD && MatchState == MatchState::WaitingToStart)
	{
		BlasterHUD->AddAnnouncement();
	}
}

void ABlasterPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HealthBar &&
		BlasterHUD->CharacterOverlay->HealthText;

	if (bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		BlasterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		BlasterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		bInitializeHealth = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

void ABlasterPlayerController::SetHUDShield(float Shield, float MaxShield)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->ShieldBar &&
		BlasterHUD->CharacterOverlay->ShieldText;

	if (bHUDValid)
	{
		const float ShieldPercent = Shield / MaxShield;
		BlasterHUD->CharacterOverlay->ShieldBar->SetPercent(ShieldPercent);
		FString ShieldText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Shield), FMath::CeilToInt(MaxShield));
		BlasterHUD->CharacterOverlay->ShieldText->SetText(FText::FromString(ShieldText));
	}
	else
	{
		bInitializeShield = true;
		HUDShield = Shield;
		HUDMaxShield = MaxShield;
	}
}

void ABlasterPlayerController::SetHUDScore(float Score)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->ScoreAmount;

	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		BlasterHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
	else
	{
		bInitializeScore = true;
		HUDScore = Score;
	}
}

void ABlasterPlayerController::SetHUDDeaths(int32 Deaths)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->DeathsAmount;

	if (bHUDValid)
	{
		FString DeathsText = FString::Printf(TEXT("%d"), Deaths);
		BlasterHUD->CharacterOverlay->DeathsAmount->SetText(FText::FromString(DeathsText));
	}
	else
	{
		bInitializeDeaths = true;
		HUDDeaths = Deaths;
	}
}

void ABlasterPlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->WeaponAmmoAmount;

	if (bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		BlasterHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
	}
	else
	{
		bInitializeWeaponAmmo = true;
		HUDWeaponAmmo = Ammo;
	}
}

void ABlasterPlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->CarriedAmmoAmount;

	if (bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		BlasterHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	}
	else
	{
		bInitializeCarriedAmmo = true;
		HUDCarriedAmmo = Ammo;
	}
}

void ABlasterPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->MatchCountdownText;

	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			BlasterHUD->CharacterOverlay->MatchCountdownText->SetText(FText());
			return;
		}

		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;
		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

		BlasterHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));
	}
}

void ABlasterPlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->Announcement &&
		BlasterHUD->Announcement->WarmupTime;

	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			BlasterHUD->Announcement->WarmupTime->SetText(FText());
			return;
		}
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		BlasterHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));
	}
}

void ABlasterPlayerController::SetHUDGrenades(int32 Grenades)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->GrenadesText;
	if (bHUDValid)
	{
		FString GrenadesText = FString::Printf(TEXT("%d"), Grenades);
		BlasterHUD->CharacterOverlay->GrenadesText->SetText(FText::FromString(GrenadesText));
	}
	else
	{
		bInitializeGrenades = true;
		HUDGrenades = Grenades;
	}
}

void ABlasterPlayerController::SetHUDTime()
{
	float TimeLeft = 0.f;
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::Cooldown) TimeLeft = CooldownTime + WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);

	if (HasAuthority())
	{
		if (BlasterGameMode == nullptr)
		{
			BlasterGameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));
			LevelStartingTime = BlasterGameMode->LevelStartingTime;
		}
		BlasterGameMode = BlasterGameMode == nullptr ? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
		if (BlasterGameMode)
		{
			SecondsLeft = FMath::CeilToInt(BlasterGameMode->GetCountdownTime() + LevelStartingTime);
		}
	}

	if (CountdownInt != SecondsLeft)
	{
		if (MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
		{
			SetHUDAnnouncementCountdown(TimeLeft);
		}
		if (MatchState == MatchState::InProgress)
		{
			SetHUDMatchCountdown(TimeLeft);
		}
	}

	CountdownInt = SecondsLeft;
}

void ABlasterPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void ABlasterPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	SingleTripTime = 0.5f * RoundTripTime;
	float CurrentServerTime = TimeServerReceivedClientRequest + SingleTripTime;
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

float ABlasterPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	else return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void ABlasterPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void ABlasterPlayerController::OnMatchStateSet(FName State, bool bTeamsMatch)
{
	MatchState = State;

	if (MatchState == MatchState::InProgress)
	{
		//FString RoleText = (GetLocalRole() == ROLE_Authority) ? TEXT("Server") : TEXT("Client"); // tomato
		//UE_LOG(LogTemp, Warning, TEXT("line 574 OnMatchStateSet called on: %s"), *RoleText);
		//UE_LOG(LogTemp, Warning, TEXT("bTeamsMatch: %s"), bTeamsMatch ? TEXT("true") : TEXT("false"));

		HandleMatchHasStarted(bTeamsMatch);
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}


void ABlasterPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		FString RoleText = (GetLocalRole() == ROLE_Authority) ? TEXT("Server") : TEXT("Client"); // tomato
		UE_LOG(LogTemp, Warning, TEXT("line line 590 OnMatchStateSet() is called on: %s"), *RoleText);
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

//void ABlasterPlayerController::HandleMatchHasStarted(bool bTeamsMatch)
//{
//	// tomato
//	//FString RoleTextt = (GetLocalRole() == ROLE_Authority) ? TEXT("Server") : TEXT("Client");
//	//UE_LOG(LogTemp, Warning, TEXT("The Next Line is:  %s"), *RoleTextt);
//	//UE_LOG(LogTemp, Warning, TEXT("bTeamsMatch: % s"), bTeamsMatch ? TEXT("true") : TEXT("false"));
//
//	if (HasAuthority())
//	{
//		UE_LOG(LogTemp, Warning, TEXT("HandleMatchHasStarted on line 634, bTeamsMatch is: %s"), bTeamsMatch ? TEXT("true") : TEXT("false"));
//		bShowTeamScores = bTeamsMatch;
//	}
//
//	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
//	if (BlasterHUD)
//	{
//		if (BlasterHUD->CharacterOverlay == nullptr) BlasterHUD->AddCharacterOverlay();
//		if (BlasterHUD->Announcement)
//		{
//			BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
//		}
//		if (!HasAuthority()) return;
//		if (bTeamsMatch)
//		{
//			InitTeamScores();
//		}
//		else
//		{
//			//FString RoleText = (GetLocalRole() == ROLE_Authority) ? TEXT("Server") : TEXT("Client");
//			//UE_LOG(LogTemp, Warning, TEXT("line 618 HideTeamScores called on: %s"), *RoleText);
//			HideTeamScores();
//		}
//	}
//}

void ABlasterPlayerController::HandleMatchHasStarted(bool bTeamsMatch)
{
	if (HasAuthority())
		bShowTeamScores = bTeamsMatch;

	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if (BlasterHUD)
	{
		if (BlasterHUD->CharacterOverlay == nullptr) BlasterHUD->AddCharacterOverlay();
		if (BlasterHUD->Announcement)
		{
			BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (!HasAuthority())
	{
		if (!bShowTeamScores)
		{
			HideTeamScores();
		}
		return;
	}

	if (bTeamsMatch)
	{
		InitTeamScores();
	}
	else
	{
		HideTeamScores();
	}
}


void ABlasterPlayerController::OnRep_ShowTeamScores()
{
	//FString RoleText = (GetLocalRole() == ROLE_Authority) ? TEXT("Server") : TEXT("Client");
	//UE_LOG(LogTemp, Warning, TEXT("OnRep_ShowTeamScores")); // tomato

	if (bShowTeamScores)
	{
		//FString RoleTextTwo = (GetLocalRole() == ROLE_Authority) ? TEXT("Server") : TEXT("Client");
		//UE_LOG(LogTemp, Warning, TEXT("line 709 InitTeamScores called on: %s"), *RoleTextTwo);
		InitTeamScores();
	}
	else
	{
		//FString RoleTextThree = (GetLocalRole() == ROLE_Authority) ? TEXT("Server") : TEXT("Client");
		//UE_LOG(LogTemp, Warning, TEXT("line 715 HideTeamScores called on: %s"), *RoleTextThree);
		HideTeamScores();
	}
}

FString ABlasterPlayerController::GetInfoText(const TArray<class ABlasterPlayerState*>& Players)
{
	ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
	if (BlasterPlayerState == nullptr) return FString();
	FString InfoTextString;
	if (Players.Num() == 0)
	{
		InfoTextString = Announcement::ThereIsNoWinner;
	}
	else if (Players.Num() == 1 && Players[0] == BlasterPlayerState)
	{
		InfoTextString = Announcement::YouAreTheWinner;
	}
	else if (Players.Num() == 1)
	{
		InfoTextString = FString::Printf(TEXT("Winner: \n%s"), *Players[0]->GetPlayerName());
	}
	else if (Players.Num() > 1)
	{
		InfoTextString = Announcement::PlayersTiedForTheWin;
		InfoTextString.Append(FString("\n"));
		for (auto TiedPlayer : Players)
		{
			InfoTextString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
		}
	}

	return InfoTextString;
}

FString ABlasterPlayerController::GetTeamsInfoText(ABlasterGameState* BlasterGameState)
{
	if (BlasterGameState == nullptr) return FString();
	FString InfoTextString;

	const int32 RedTeamScore = BlasterGameState->RedTeamScore;
	const int32 BlueTeamScore = BlasterGameState->BlueTeamScore;

	if (RedTeamScore == 0 && BlueTeamScore == 0)
	{
		InfoTextString = Announcement::ThereIsNoWinner;
	}
	else if (RedTeamScore == BlueTeamScore)
	{
		InfoTextString = FString::Printf(TEXT("%s\n"), *Announcement::TeamsTiedForTheWin);
		InfoTextString.Append(Announcement::RedTeam);
		InfoTextString.Append(TEXT("\n"));
		InfoTextString.Append(Announcement::BlueTeam);
		InfoTextString.Append(TEXT("\n"));
	}
	else if (RedTeamScore > BlueTeamScore)
	{
		InfoTextString = Announcement::RedTeamWins;
		InfoTextString.Append(TEXT("\n"));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::RedTeam, RedTeamScore));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::BlueTeam, BlueTeamScore));
	}
	else if (BlueTeamScore > RedTeamScore)
	{
		InfoTextString = Announcement::BlueTeamWins;
		InfoTextString.Append(TEXT("\n"));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::BlueTeam, BlueTeamScore));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::RedTeam, RedTeamScore));
	}

	return InfoTextString;
}


void ABlasterPlayerController::HandleCooldown()
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if (BlasterHUD)
	{
		BlasterHUD->CharacterOverlay->RemoveFromParent();
		bool bHUDValid = BlasterHUD->Announcement &&
			BlasterHUD->Announcement->AnnouncementText &&
			BlasterHUD->Announcement->InfoText;

		if (bHUDValid)
		{
			BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
			FString AnnouncementText = Announcement::NewMatchStartsIn;
			BlasterHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText));

			ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
			ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();

			if (BlasterGameState && BlasterPlayerState)
			{
				TArray<ABlasterPlayerState*> TopPlayers = BlasterGameState->TopScoringPlayers;
				FString InfoTextString = bShowTeamScores ? GetTeamsInfoText(BlasterGameState) : GetInfoText(TopPlayers);
				BlasterHUD->Announcement->InfoText->SetText(FText::FromString(InfoTextString));

			}
		}
	}

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn());
	if (BlasterCharacter && BlasterCharacter->GetCombat())
	{
		BlasterCharacter->bDisableGameplay = true;
		BlasterCharacter->GetCombat()->FireButtonPressed(false);
	}
}

void ABlasterPlayerController::BroadcastElim(APlayerState* Attacker, APlayerState* Victim)
{
	ClientElimAnnouncement(Attacker, Victim);
}

void ABlasterPlayerController::ClientElimAnnouncement_Implementation(APlayerState* Attacker, APlayerState* Victim)
{
	APlayerState* Self = GetPlayerState<APlayerState>();
	if (Attacker && Victim && Self)
	{
		BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
		if (BlasterHUD)
		{
			if (Attacker == Self && Victim != Self) // you killed someone
			{
				BlasterHUD->AddElimAnnouncement("You", Victim->GetPlayerName());
				return;
			}
			if (Victim == Self && Attacker != Self) // someone killed you
			{
				BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), "you");
				return;
			}
			if (Attacker == Victim && Attacker == Self) // you killed yourself
			{
				BlasterHUD->AddElimAnnouncement("you", "yourself");
				return;
			}
			if (Attacker == Victim && Attacker != Self) // someone killed themselves
			{
				BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), "themselves");
				return;
			}
			BlasterHUD->AddElimAnnouncement(Attacker->GetPlayerName(), Victim->GetPlayerName());
		}
	}
}

void ABlasterPlayerController::InitTeamScores()
{
	if (!HasAuthority()) UE_LOG(LogTemp, Warning, TEXT("InitTeamScoresCalled"));
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->RedTeamScore &&
		BlasterHUD->CharacterOverlay->BlueTeamScore;

	if (bHUDValid)
	{
		if (!HasAuthority()) UE_LOG(LogTemp, Warning, TEXT("bHUDIsValidTeamScores"));
		FString Zero("0");

		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText::FromString(Zero));
		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText::FromString(Zero));
	}
	//Change from course. Add to pollinit if Character Overlay is invalid
	else
	{
		bInitializeTeamScores = true;
		if (!HasAuthority()) UE_LOG(LogTemp, Warning, TEXT("bInitializeTeamScores called"));

	}
}

void ABlasterPlayerController::BigDebugLog()
{
	UE_LOG(LogTemp, Warning, TEXT("BlasterHUD is %s, Running on %s"),
		BlasterHUD ? TEXT("valid") : TEXT("invalid"),
		GetWorld()->IsNetMode(NM_ListenServer) ? TEXT("Server") : TEXT("Client"));

	UE_LOG(LogTemp, Warning, TEXT("CharacterOverlay is %s, Running on %s"),
		(BlasterHUD && BlasterHUD->CharacterOverlay) ? TEXT("valid") : TEXT("invalid"),
		GetWorld()->IsNetMode(NM_ListenServer) ? TEXT("Server") : TEXT("Client"));

	UE_LOG(LogTemp, Warning, TEXT("HighPingImage is %s, Running on %s"),
		(BlasterHUD && BlasterHUD->CharacterOverlay && BlasterHUD->CharacterOverlay->HighPingImage) ? TEXT("valid") : TEXT("invalid"),
		GetWorld()->IsNetMode(NM_ListenServer) ? TEXT("Server") : TEXT("Client"));

	UE_LOG(LogTemp, Warning, TEXT("HighPingAnimation is %s, Running on %s"),
		(BlasterHUD && BlasterHUD->CharacterOverlay && BlasterHUD->CharacterOverlay->HighPingImage && BlasterHUD->CharacterOverlay->HighPingAnimation) ? TEXT("valid") : TEXT("invalid"),
		GetWorld()->IsNetMode(NM_ListenServer) ? TEXT("Server") : TEXT("Client"));
}
