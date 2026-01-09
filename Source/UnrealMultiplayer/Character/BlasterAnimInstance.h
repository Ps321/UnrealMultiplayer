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

	
};
