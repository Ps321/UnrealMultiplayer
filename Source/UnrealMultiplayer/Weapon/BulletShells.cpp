// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BulletShells.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
ABulletShells::ABulletShells()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BulletShellMesh=CreateDefaultSubobject<UStaticMeshComponent>("BulletShell");
	SetRootComponent(BulletShellMesh);
	BulletShellMesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	BulletShellMesh->SetSimulatePhysics(true);
	BulletShellMesh->SetEnableGravity(true);
	BulletShellMesh->SetNotifyRigidBodyCollision(true);
	ShellImpulse=10.f;
}

// Called when the game starts or when spawned
void ABulletShells::BeginPlay()
{
	Super::BeginPlay();
	BulletShellMesh->AddImpulse(GetActorForwardVector()*ShellImpulse);
	BulletShellMesh->OnComponentHit.AddDynamic(this,&ABulletShells::OnHit);
	SetLifeSpan(3.f);
}

void ABulletShells::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (BulletShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),BulletShellSound,GetActorLocation());
		BulletShellMesh->SetNotifyRigidBodyCollision(false);
	}
}



