// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterCharacter.h"
#include "Animation/AnimInstance.h"
#include "BlasterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALMULTIPLAYER_API UBlasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(BluePrintReadOnly, Category = Character,meta=(AllowPrivateAccess="true"))
	class ABlasterCharacter* BlasterCharacter;
	UPROPERTY(BluePrintReadOnly, Category = Character,meta=(AllowPrivateAccess="true"))
	float Speed;
	UPROPERTY(BluePrintReadOnly, Category = Character,meta=(AllowPrivateAccess="true"))
	bool bIsInAir;
	UPROPERTY(BluePrintReadOnly, Category = Character,meta=(AllowPrivateAccess="true"))
	bool bIsAccelerating;

	UPROPERTY(BluePrintReadOnly, Category = Character,meta=(AllowPrivateAccess="true"))
	bool bIsWeaponEquipped;
	UPROPERTY(BluePrintReadOnly, Category = Character,meta=(AllowPrivateAccess="true"))
	bool bIsCrouch;
	UPROPERTY(BluePrintReadOnly, Category = Character,meta=(AllowPrivateAccess="true"))
	bool bIsAiming;

	UPROPERTY(BluePrintReadOnly, Category = Character,meta=(AllowPrivateAccess="true"))
	float YawOffset;
	UPROPERTY(BluePrintReadOnly, Category = Character,meta=(AllowPrivateAccess="true"))
	float Lean;

	FRotator DeltaRotator;
	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	AWeapon* EquippedWeapon;

	UPROPERTY(BluePrintReadOnly, Category = Character,meta=(AllowPrivateAccess="true"))
	float AO_Yaw;
	UPROPERTY(BluePrintReadOnly, Category = Character,meta=(AllowPrivateAccess="true"))
	float AO_Pitch;
	UPROPERTY(BluePrintReadOnly, Category = Character,meta=(AllowPrivateAccess="true"))
	FTransform LeftHandTransform;
	
};
