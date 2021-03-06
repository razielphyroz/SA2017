// Fill out your copyright notice in the Description page of Project Settings.

#include "SA_Afflicted.h"
#include "Personagem.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "LanternaDoJogador.h"
#include "Projectile.h"
#include "ArmaDoJogador.h"
#include "MyHUD.h"

// Sets default values
APersonagem::APersonagem()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(CameraBoom);
	GetCapsuleComponent()->SetWorldScale3D(FVector(2.0f, 1.0f, 1.0f));
	GetCharacterMovement()->MaxWalkSpeed = 800.0f;

	ConstructorHelpers::FClassFinder<UUserWidget> LoadWidget
		 (TEXT("WidgetBlueprint'/Game/Widgets/Pause.Pause_C'"));
	if (LoadWidget.Succeeded()) {
		UserWidgetPause = LoadWidget.Class;
	}

	//ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	//if (SkeletalMesh.Succeeded()) {
	//	GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
	//}

	//GetMesh()->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
	//GetMesh()->SetWorldLocation(FVector(0.0f, 0.0f, -80.0f));
	//GetMesh()->SetWorldScale3D(FVector(0.9f, 0.9f, 0.9f));
	//GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	//ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimObj(TEXT("AnimBlueprint'/Game/Animations/WalkAnimation.WalkAnimation'"));
	//if (AnimObj.Succeeded()) {
	//	GetMesh()->SetAnimInstanceClass(AnimObj.Object->GetAnimBlueprintGeneratedClass());
	//}

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	Life = 100.0f;

}

// Called when the game starts or when spawned
void APersonagem::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParameters;
		LanternaEmUso = World->SpawnActor<ALanternaDoJogador>(GetActorLocation(), GetActorRotation(), SpawnParameters);
	}

	AMyHUD * hud = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (hud) {
		hud->AtivarTexto(0);
	}
}

// Called every frame
void APersonagem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LanternaEmUso->SetActorLocation(FVector(GetActorLocation().X,GetActorLocation().Y,GetActorLocation().Z));
	FRotator Rotation = GetActorRotation();
	Rotation.Pitch -= 2.0f;
	LanternaEmUso->SetActorRotation(Rotation);
	if (Life <= 0) {
		//Ja Elvis------------------------------------------------------------------------------------------
	}
}

// Called to bind functionality to input
void APersonagem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move", this, &APersonagem::Move);
	PlayerInputComponent->BindAxis("Sides", this, &APersonagem::MoveSides);
	PlayerInputComponent->BindAxis("Turn", this, &APersonagem::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &APersonagem::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APersonagem::Pause);
	PlayerInputComponent->BindAction("CorLant", IE_Pressed, this, &APersonagem::MudarCor);
	PlayerInputComponent->BindAction("Atirar", IE_Pressed, this, &APersonagem::Atirar);
}


void APersonagem::Move(float Value)
{
	if (Controller != NULL && Value != 0.0f) {
		FRotator Rotation = Controller->GetControlRotation();
		if (GetCharacterMovement()->IsMovingOnGround() ||
			GetCharacterMovement()->IsFalling()) {
			Rotation.Pitch = 0.0f;
		}
		const FVector Direction = FRotationMatrix(Rotation).
			GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APersonagem::MoveSides(float Value)
{
	if (Controller != NULL && Value != 0.0f) {
		FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void APersonagem::MudarCor()
{
	if (LanternaEmUso != nullptr) {
		LanternaEmUso->MudarCorDaLuz();
	}
}

void APersonagem::Pause() {
	UWorld* World = GetWorld();
	if (World) {
		APlayerController* PlayerController =
			UGameplayStatics::GetPlayerController(World, 0);
		if (PlayerController && UserWidgetPause != NULL) {
			PlayerController->SetPause(true);
			UUserWidget* UserW =
				UWidgetBlueprintLibrary::Create(
					World, UserWidgetPause, PlayerController);
			if (UserW) {
				UserW->AddToViewport();
				PlayerController->bShowMouseCursor = true;
			}
		}
	}
} 		  

float APersonagem::GetLife()
{
	return Life;
}

void APersonagem::SetLife(float Value)
{
	Life = Value;
}

void APersonagem::AdicionarCorDisponivel()
{
	if (LanternaEmUso != nullptr) {
		LanternaEmUso->AdicionarCorDisponivel();
	}
}

void APersonagem::Atirar() 
{
	if (ArmaEmUso != nullptr) {
		UWorld* World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParameters;
			FRotator Rotation = GetActorRotation();
			Rotation.Pitch -= 5.0f;
			AProjectile* Projectile = World->SpawnActor<AProjectile>(GetActorLocation(), Rotation, SpawnParameters);
		}
	}
}

bool APersonagem::IsTemArma() {
	if (ArmaEmUso != nullptr) {
		return true;
	} else {
		return false;
	}
}

void APersonagem::InicializarArma() {
	UWorld* World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParameters;
		ArmaEmUso = World->SpawnActor<AArmaDoJogador>(GetActorLocation(), GetActorRotation(), SpawnParameters);
	}
}

int8 APersonagem::GetMonstrosMortos() {
	return MonstrosMortos;
}

void APersonagem::SetMonstrosMortos(int8 Value) {
	MonstrosMortos = Value;

}