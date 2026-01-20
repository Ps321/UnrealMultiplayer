// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALMULTIPLAYER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class ABlasterCharacter;
	void EquipWeapon(class AWeapon* WeapontoEquip);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	ABlasterCharacter* Character;
	UPROPERTY(ReplicatedUsing=OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	bool bIsAiming;

	UFUNCTION()
	void OnRep_EquippedWeapon();
	UFUNCTION()
	void FireWeapon(bool bIsFiring);

	bool bIsFiringWeapon;

	UFUNCTION(NetMulticast,Reliable)
	void MulticastFireWeapon(const FVector_NetQuantize& TraceHitTarget);
	UFUNCTION(Server, Reliable)
	void ServerFireWeapon(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& HitResult);


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkingSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAimWalkingSpeed;
	
};
