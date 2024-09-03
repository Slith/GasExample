// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "GasExampleCharacter.h"
#include "GasExampleProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	SelectedProjectileClassIndex = INDEX_NONE;
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (SelectedProjectileClassIndex != INDEX_NONE && AvailableProjectileClasses.Num() > SelectedProjectileClassIndex)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.Instigator = Character;
	
			// Spawn the projectile at the muzzle
			World->SpawnActor<AGasExampleProjectile>(AvailableProjectileClasses[SelectedProjectileClassIndex], SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

FName UTP_WeaponComponent::GetSelectedAmmoName() const
{
	if (SelectedProjectileClassIndex != INDEX_NONE && AvailableProjectileClasses.Num() > SelectedProjectileClassIndex)
	{
		if (const UClass* DefaultSelectedProjectileClass = AvailableProjectileClasses[SelectedProjectileClassIndex])
		{
			if (DefaultSelectedProjectileClass->ClassGeneratedBy)
			{
				return DefaultSelectedProjectileClass->ClassGeneratedBy->GetFName();
			}
		}
	}

	return NAME_None;
}

bool UTP_WeaponComponent::AttachWeapon(AGasExampleCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	if (SelectedProjectileClassIndex == INDEX_NONE && AvailableProjectileClasses.Num() > 0)
	{
		SelectedProjectileClassIndex = 0;
	}

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
			EnhancedInputComponent->BindAction(NextProjectileClass, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::OnNextProjectileClass);
			EnhancedInputComponent->BindAction(PreviousProjectileClass, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::OnPreviousProjectileClass);
		}
	}

	return true;
}

void UTP_WeaponComponent::OnNextProjectileClass()
{
	if (AvailableProjectileClasses.Num() > 0)
	{
		if (++SelectedProjectileClassIndex >= AvailableProjectileClasses.Num())
		{
			SelectedProjectileClassIndex = 0;
		}

		OnSelectedAmmoChanged.Broadcast(GetSelectedAmmoName());
	}
}

void UTP_WeaponComponent::OnPreviousProjectileClass()
{
	if (AvailableProjectileClasses.Num() > 0)
	{
		if (--SelectedProjectileClassIndex < 0)
		{
			SelectedProjectileClassIndex = AvailableProjectileClasses.Num() - 1;
		}

		OnSelectedAmmoChanged.Broadcast(GetSelectedAmmoName());
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}