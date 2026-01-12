// Fill out your copyright notice in the Description page of Project Settings.

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

}



void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
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
	// EquippedWeapon->ShowPickupWidget(false);
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
}

