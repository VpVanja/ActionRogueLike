// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"


#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "Components/CapsuleComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComp"));
	
}




// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}



// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("PrimaryAttack"), IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("PrimaryInteract"), IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction(TEXT("BlackHoleAbility"), IE_Pressed, this, &ASCharacter::BlackHoleAbility);
	PlayerInputComponent->BindAction(TEXT("DashAbility"), IE_Pressed, this, &ASCharacter::DashAbility);
}



void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(), Value);
}


inline void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value);
	//FVector RightVector1 = ControlRot.Vector().RightVector;// This is an alternative way to get the right vector. and it works.
}



void ASCharacter::PrimaryAttack()
{
	if (DidTimerFire)
	{
		PlayAnimMontage(AttackAnim);
		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
                	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
		DidTimerFire = false;
	}
}



void ASCharacter::PrimaryAttack_TimeElapsed()// This is a timer function. it is called after 0.2 seconds of the PrimaryAttack function. so it matches the animation.
{
	CameraTrace();
	
	FTransform SpawnTM = GetProjectileTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 
	SpawnParams.Instigator = this;
	
	SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	DidTimerFire = true;

	GetMesh()->IgnoreActorWhenMoving(SpawnedProjectile, true);
	GetCapsuleComponent()->IgnoreActorWhenMoving(SpawnedProjectile, true);
}


void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryIneract();
	}
	
}



void ASCharacter::CameraTrace()
{
	FVector Start = CameraComp->GetComponentLocation();
	FVector End = Start + CameraComp->GetForwardVector() * 10000.0f;
	FHitResult HitResult;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Vehicle);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Visibility);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	
	bool bBlockingHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, CollisionQueryParams); // this is the way we will probably use in the future and set the projectile to ignore this channel.
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectQueryParams, CollisionQueryParams);
	if (bBlockingHit)
	{
		ProjectileDestination = HitResult.ImpactPoint;
	}
	else
	{
		ProjectileDestination = End;
	}
}




void ASCharacter::BlackHoleAbility()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAbility, this, &ASCharacter::BlackHoleAbility_TimeElapsed, 0.2f);
}

void ASCharacter::BlackHoleAbility_TimeElapsed()
{
	CameraTrace();
	
	FTransform SpawnTM = GetProjectileTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 
	SpawnParams.Instigator = this;
	GetWorld()->SpawnActor<AActor>(BlackHoleClass, SpawnTM, SpawnParams);
}



FTransform ASCharacter::GetProjectileTransform()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector SpawnVector = ProjectileDestination - HandLocation;

	FRotator ProjectileRotation =  SpawnVector.Rotation();
	
	FTransform SpawnTM = FTransform(ProjectileRotation, HandLocation);
	return SpawnTM;
}



void ASCharacter::DashAbility()
{
	CameraTrace();
	PlayAnimMontage(AttackAnim);
	FTransform SpawnTM = GetProjectileTransform();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 
	SpawnParams.Instigator = this;
	
	SpawnedTeleporter = GetWorld()->SpawnActor<AActor>(TeleportProjectileClass, SpawnTM, SpawnParams);
	
	GetCapsuleComponent()->IgnoreActorWhenMoving(SpawnedTeleporter, true);
}



