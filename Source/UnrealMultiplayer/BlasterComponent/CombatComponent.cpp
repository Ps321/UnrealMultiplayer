// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
DEFINE_LOG_CATEGORY_STATIC(LogBlaster, Log, All);

#include "BlasterComponent/CombatComponent.h"

#include "Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon/Weapon.h"


// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MaxWalkingSpeed=600.f;
	MaxAimWalkingSpeed=450.0f;
}



void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (Character)Character->GetCharacterMovement()->MaxWalkSpeed=MaxWalkingSpeed;
}


void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Character)
	{
		Character->GetCharacterMovement()->bOrientRotationToMovement=false;
		Character->bUseControllerRotationYaw=true;
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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

