// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileWeapon.h"

#include "Projectile.h"
#include "Engine/SkeletalMeshSocket.h"

void AProjectileWeapon::FireWeapon(const FVector& HitTarget)
{
	Super::FireWeapon(HitTarget);
	if (!HasAuthority())return;
	const USkeletalMeshSocket* MuzzleSocket=GetEquippedMeshComponent()->GetSocketByName("MuzzleFlash");
	APawn* InstigatorPawn=Cast<APawn>(GetOwner());
	
	if (MuzzleSocket && InstigatorPawn)
	{
		FTransform MuzzleSocketTransform=MuzzleSocket->GetSocketTransform(GetEquippedMeshComponent());
		FVector ToTarget=HitTarget-MuzzleSocketTransform.GetLocation();
		FRotator ToRotation=ToTarget.Rotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator=InstigatorPawn;
		 UWorld* staticWorld=GetWorld();
		if (staticWorld)
		{
			staticWorld->SpawnActor<AProjectile>(ProjectileClass,MuzzleSocketTransform.GetLocation(),ToRotation,SpawnParams);
		}
	}
}
