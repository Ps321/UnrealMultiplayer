// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

#include "BulletShells.h"
#include "Character/BlasterCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	WeaponMesh=CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(WeaponMesh);

	AreaSphere=CreateDefaultSubobject<USphereComponent>("Area Sphere");
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget=CreateDefaultSubobject<UWidgetComponent>("PickupWidget");
	PickupWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this,&AWeapon::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this,&AWeapon::OnSphereEndOverlap);
	}
}
// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon,CurrentWeaponState);
}


void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABlasterCharacter* BlasterCharacter=Cast<ABlasterCharacter>(OtherActor);

	if (BlasterCharacter)
	{
		BlasterCharacter->SetOverlappingWeapon(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABlasterCharacter* BlasterCharacter=Cast<ABlasterCharacter>(OtherActor);

	if (BlasterCharacter)
	{
		BlasterCharacter->SetOverlappingWeapon(nullptr);
	}
}

void AWeapon::OnRep_WeaponState()
{
	switch (CurrentWeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
	}
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	CurrentWeaponState=State;
	switch (State)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


void AWeapon::ShowPickupWidget(bool bShow)
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShow);
	}
}

void AWeapon::FireWeapon(const FVector& HitTarget)
{
	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation,false);
	}

	if (BulletShell)
	{
		const USkeletalMeshSocket* ShellSocket=GetEquippedMeshComponent()->GetSocketByName("AmmoEject");
	
		
			FTransform ShellSocketTransform=ShellSocket->GetSocketTransform(GetEquippedMeshComponent());

			const FRotator RandRotator = UKismetMathLibrary::RandomRotator();

			const FRotator LerpedRot = UKismetMathLibrary::RLerp(ShellSocketTransform.GetRotation().Rotator(), RandRotator, .35f, true);
			UWorld* staticWorld=GetWorld();
			if (staticWorld)
			{
				staticWorld->SpawnActor<ABulletShells>(BulletShell,ShellSocketTransform.GetLocation(),LerpedRot);
			}
	}
	
}


