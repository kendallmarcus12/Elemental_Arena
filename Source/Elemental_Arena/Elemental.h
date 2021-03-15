// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "AbilitySystemInterface.h"
#include "Elemental.generated.h"

UCLASS()
class ELEMENTAL_ARENA_API AElemental : public ACharacter
{
	GENERATED_BODY()

	class USpringArmComponent* CameraBoom;


	class UCameraComponent* PlayerCamera;

public:
	// Sets default values for this character's properties
	AElemental();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controls, meta = (AllowPrivateAccess = "true"))
	float BaseHorizontalRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controls, meta = (AllowPrivateAccess = "true"))
	float BaseVerticalRate;


	/*
		Going to implement this later after getting a few more of the super classes done as not much interaction is happening rn
	*/
// 	class UAbilitySystemComponent* AbilitySystemCompontent;
// 
// 	virtual UAbilitySystemComponent GetAbilitySystemCompontent() const override {
// 		return AbilitySystemCompontent;
// 	}

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Function called when forward/backwards is pressed*/
	void MoveForward(float Value);

	/** Function called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void LightAttack();

	void Wall();

public:	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return PlayerCamera;}
};
