// Fill out your copyright notice in the Description page of Project Settings.

#include "Elemental.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/Vector.h"
#include "Projectile.h"
#include "Wall.h"
#include "DrawDebugHelpers.h"

#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AElemental::AElemental()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseHorizontalRate = 45.f;
	BaseVerticalRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	PlayerCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
// 	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
// 	Mesh->SetupAttachment(RootComponent);

// 	static ConstructorHelpers::FObjectFinder<USkeletalMeshComponent> SkeletalMesh(TEXT("/Game/StarterContent/Mannequin/SK_Mannequin.SK_Mannequin"));
// 	// check if path is valid
// 	if (SkeletalMesh.Succeeded())
// 	{
// 		// mesh = valid path
// 		Mesh->SetSkeletalMesh(SkeletalMesh.Object);
// 		// set relative location of mesh
// 		Mesh->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
// 	}
}

// Called to bind functionality to input
void AElemental::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &AElemental::LightAttack);
	PlayerInputComponent->BindAction("Wall", IE_Pressed, this, &AElemental::Wall);
	

	PlayerInputComponent->BindAxis("MoveForward", this, &AElemental::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AElemental::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AElemental::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AElemental::LookUpAtRate);

}

void AElemental::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseHorizontalRate * GetWorld()->GetDeltaSeconds());
}

void AElemental::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseVerticalRate * GetWorld()->GetDeltaSeconds());
}

/*Moves the character forward given at a rate given from the input*/
void AElemental::MoveForward(float InputValue){

	//gets forward movement based on current rotation value
	if ((Controller != nullptr) && (InputValue != 0.0f)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, InputValue);
	}
}

void AElemental::MoveRight(float InputValue) {
	//gets forward movement based on current rotation value
	if ((Controller != nullptr) && (InputValue != 0.0f)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, InputValue);
	}
}

void AElemental::LightAttack() {
	const FVector loc = this->GetActorLocation().operator+(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X).operator*(800));
	const FRotator rot = this->GetControlRotation();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(loc, rot);
	Projectile->LaunchProjectile();
	
}
/*
	Optimize where the range is customizable 
*/
void AElemental::Wall() {
	//AWall* FireWall;


	FHitResult Results = FHitResult(ForceInit);
	FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FVector CameraLoc= PlayerCamera->GetComponentLocation();
	FVector TraceLine = PlayerCamera->GetForwardVector().operator*(650);

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(Results, CameraLoc, TraceLine.operator+(CameraLoc), ECC_Visibility, TraceParams);
	
	if (bIsHit) {
		DrawDebugLine(GetWorld(), CameraLoc, TraceLine.operator+(CameraLoc), FColor::Green, false, 5.f, ECC_WorldStatic, 1.f);
		DrawDebugBox(GetWorld(), Results.ImpactPoint, FVector(2.f, 2.f, 2.f), FColor::Blue, false, 5.f, ECC_WorldStatic, 1.f);
	}

	else {
		DrawDebugLine(GetWorld(), CameraLoc, TraceLine.operator+(CameraLoc), FColor::Purple, false, 5.f, ECC_WorldStatic, 1.f);
	}

}

