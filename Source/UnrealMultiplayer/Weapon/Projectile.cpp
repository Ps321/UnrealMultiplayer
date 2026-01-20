// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates=true;
	BoxCollision=CreateDefaultSubobject<UBoxComponent>("BoxCollsision");
	SetRootComponent(BoxCollision);
	BoxCollision->SetCollisionObjectType(ECC_WorldDynamic);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	ProjectileMovement=CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovement->bRotationFollowsVelocity=true;
}
// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (Tracer)
	{
		TracerComponent=UGameplayStatics::SpawnEmitterAttached(Tracer,BoxCollision,FName(),GetActorLocation(),GetActorRotation(),EAttachLocation::KeepWorldPosition);
	}
	BoxCollision->OnComponentHit.AddDynamic(this,&AProjectile::OnHit);
}



void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void AProjectile::Destroyed()
{
	Super::Destroyed();
	if (ImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactParticle,GetActorLocation());
	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation());
	}
}



