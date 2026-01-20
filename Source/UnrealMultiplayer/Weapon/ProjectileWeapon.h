// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNREALMULTIPLAYER_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()
	virtual void FireWeapon(const FVector& HitTarget) override;
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;
	
};
