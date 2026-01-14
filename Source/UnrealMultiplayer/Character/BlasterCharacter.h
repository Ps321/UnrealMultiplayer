// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterComponent/CombatComponent.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

UCLASS()
class UNREALMULTIPLAYER_API ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void EquipItem();
	void CrouchPlayer();
	void UnCrouchPlayer();
	void Aim();
	void Unaim();
	void AimOffset(float DeltaSeconds);

private:	
	UPROPERTY(VisibleAnywhere,Category =Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere,Category =Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess="true"))
	class UWidgetComponent* OverHeadWidget;

	UPROPERTY(ReplicatedUsing=OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);
	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	UFUNCTION(Server, Reliable)
	void ServerAiming(bool isAiming);

	float AO_Yaw;
	float AO_Pitch;
	FRotator CurrentRotation;
	FRotator StartingRotation;
public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	FORCEINLINE bool IsWeaponEquipped(){return (Combat && Combat->EquippedWeapon);}
	FORCEINLINE bool IsAiming() const { return Combat->bIsAiming; }
	void SetAiming(bool value);
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE AWeapon* GetEquippedWeapon() const {return (Combat && Combat->EquippedWeapon)?Combat->EquippedWeapon:nullptr;}
};
