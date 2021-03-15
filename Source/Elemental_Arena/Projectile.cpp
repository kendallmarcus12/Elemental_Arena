// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Sphere;

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->bAutoActivate = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	// check if path is valid
	if (SphereMesh.Succeeded())
	{
		// mesh = valid path
		Mesh->SetStaticMesh(SphereMesh.Object);
		// set relative location of mesh
		Mesh->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
	}

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effects"));
	Particle->SetupAttachment(Mesh);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (ParticleAsset.Succeeded())
	{
		Particle->SetTemplate(ParticleAsset.Object);
	}


}

void AProjectile::LaunchProjectile(){
	Movement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	// Tell the projectile to move
	Movement->Activate();
}


