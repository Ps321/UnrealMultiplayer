// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
DEFINE_LOG_CATEGORY_STATIC(LogBlaster, Log, All);

#include "BlasterComponent/CombatComponent.h"

#include "Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon/Weapon.h"
#define TRACE_LENGTH 80000.0f

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxWalkingSpeed=600.f;
	MaxAimWalkingSpeed=450.0f;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}



void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetComponentTickEnabled(true);
	if (Character)Character->GetCharacterMovement()->MaxWalkSpeed=MaxWalkingSpeed;
}
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Character)
	{
		Character->GetCharacterMovement()->bOrientRotationToMovement=false;
		Character->bUseControllerRotationYaw=true;
	}
}

void UCombatComponent::FireWeapon(bool bIsFiring)
{
	bIsFiringWeapon=bIsFiring;
	if (!bIsFiring)return;
	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
	ServerFireWeapon(HitResult.ImpactPoint);
}

void UCombatComponent::MulticastFireWeapon_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if (EquippedWeapon==nullptr)return;
	if (Character)
	{
		Character->PlayFireMontage(bIsAiming);
		EquippedWeapon->FireWeapon(TraceHitTarget);
	}
}

void UCombatComponent::ServerFireWeapon_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	MulticastFireWeapon(TraceHitTarget);
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& HitResult)
{
	FVector2D ViewportSize;
	if (GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	FVector2D CrosshairLocation(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);
	FVector WorldCrosshairLocation;
	FVector Direction;

	bool bScreentoWorld=UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this,0),
		CrosshairLocation,
		WorldCrosshairLocation,
		Direction);
	

	if (bScreentoWorld)
	{
		FVector Start=WorldCrosshairLocation;
		FVector End=Start+Direction*TRACE_LENGTH;

		GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_Visibility);
		if (!HitResult.bBlockingHit)
		{
			HitResult.ImpactPoint=End;
		}
	}
}



void UCombatComponent::EquipWeapon(AWeapon* WeapontoEquip)
{
	if (Character==nullptr || WeapontoEquip == nullptr)return;

	EquippedWeapon=WeapontoEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* handSocket=Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if(handSocket)
	{
		handSocket->AttachActor(EquippedWeapon,Character->GetMesh());
	}
	UE_LOG(LogBlaster, Warning, TEXT("aaya %s"), *GetNameSafe(Character));

	EquippedWeapon->SetOwner(Character);
	Character->GetCharacterMovement()->bOrientRotationToMovement=false;
	Character->bUseControllerRotationYaw=true;
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, bIsAiming);
}

