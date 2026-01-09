// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlasterAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

void UBlasterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	BlasterCharacter=Cast<ABlasterCharacter>(TryGetPawnOwner());
}

void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (BlasterCharacter== nullptr)
	{
		BlasterCharacter=Cast<ABlasterCharacter>(TryGetPawnOwner());
	}
	if (BlasterCharacter==nullptr)return;

	FVector Velocity=BlasterCharacter->GetVelocity();
	Velocity.Z=0;
	Speed=Velocity.Size();

	bIsInAir=BlasterCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating=BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size()>0?true:false;
}
