// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Personagem.generated.h"

UCLASS()
class SA_AFFLICTED_API APersonagem : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APersonagem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float GetLife();
	void SetLife(float Value);

	void AdicionarCorDisponivel();

	void Atirar();

	bool IsTemArma();

	void InicializarArma();

	int8 GetMonstrosMortos();

	void SetMonstrosMortos(int8 Value);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Items")
		class ALanternaDoJogador* LanternaEmUso;

	TSubclassOf<class UUserWidget> UserWidgetPause;

	class AArmaDoJogador* ArmaEmUso;

	void Move(float Value);

	void MoveSides(float Value);

	void MudarCor();

	void Pause();	

	float Life;

	int8 MonstrosMortos;

};
