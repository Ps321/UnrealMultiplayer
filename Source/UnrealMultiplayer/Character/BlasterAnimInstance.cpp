// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlasterAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
	bIsWeaponEquipped=BlasterCharacter->IsWeaponEquipped();
	bIsCrouch=BlasterCharacter->IsCrouched();
	bIsAiming=BlasterCharacter->IsAiming();

	FRotator Rotation=BlasterCharacter->GetBaseAimRotation();
	FRotator MovementRotation=UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity());
	FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation,Rotation);
	DeltaRotator=FMath::RInterpTo(DeltaRotator,Delta,DeltaSeconds,6.f);
	YawOffset=DeltaRotator.Yaw;

	CharacterRotationLastFrame=CharacterRotation;
	CharacterRotation=BlasterCharacter->GetActorRotation();
	FRotator DeltaRot=UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation,CharacterRotationLastFrame);
	float Target=DeltaRot.Yaw/DeltaSeconds;
	float Interp=FMath::FInterpTo(Lean,Target,DeltaSeconds,6.f);
	Lean=FMath::Clamp(Interp,-90,90);
	
}
