// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"


#include "AITypes.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "Components/CapsuleComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"


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

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

	HandSocketName = TEXT("Muzzle_01");
	TimeToHitParamName = TEXT("TimeToHit");
	HealOrDamageSwitchParamName = TEXT("HealDamageSwitch");
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
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

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryIneract();
	}
}


void ASCharacter::PrimaryAttack()
{
	if (DidTimerFire)
	{
		PlayAnimMontage(AttackAnim);
		if (CastVfx)
		{
			UGameplayStatics::SpawnEmitterAttached(CastVfx, GetMesh(), HandSocketName);
		}
		
		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
                	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
		DidTimerFire = false;
	}
}

void ASCharacter::PrimaryAttack_TimeElapsed()// This is a timer function. it is called after 0.2 seconds of the PrimaryAttack function. so it matches the animation.
{
	SpawnProjectile(ProjectileClass);
}



void ASCharacter::BlackHoleAbility()
{
	if (DidTimerFire)
	{
		PlayAnimMontage(AttackAnim);
		if (CastVfx)
		{
			UGameplayStatics::SpawnEmitterAttached(CastVfx, GetMesh(), HandSocketName);
		}
		GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAbility, this, &ASCharacter::BlackHoleAbility_TimeElapsed, 0.2f);
		DidTimerFire = false;
	}
	
}


void ASCharacter::BlackHoleAbility_TimeElapsed()
{
	SpawnProjectile(BlackHoleClass);
}


void ASCharacter::DashAbility()
{
	if (DidTimerFire)
	{
		PlayAnimMontage(AttackAnim);
		if (CastVfx)
		{
			UGameplayStatics::SpawnEmitterAttached(CastVfx, GetMesh(), HandSocketName);
		}
        	GetWorldTimerManager().SetTimer(TimerHandle_DashAbility, this, &ASCharacter::DashAbility_TimeElapsed, 0.2f);
		DidTimerFire = false;
	}
	
}

void ASCharacter::DashAbility_TimeElapsed()
{
	SpawnProjectile(TeleportProjectileClass);
}


void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	DidTimerFire = true;
	FTransform SpawnTM = GetProjectileTransformFromTraceEnd(CameraTrace());

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 
	SpawnParams.Instigator = this;

	SpawnedTeleporter = GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	
	GetCapsuleComponent()->IgnoreActorWhenMoving(SpawnedTeleporter, true);
}

FVector ASCharacter::CameraTrace()
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

	FVector TraceEnd = Start + CameraComp->GetForwardVector() * 10000.0f;
	
	bool bBlockingHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, CollisionQueryParams); // this is the way we will probably use in the future and set the projectile to ignore this channel.
	if (bBlockingHit)
	{
		TraceEnd = HitResult.ImpactPoint;
	}

	return TraceEnd;
	
}

FTransform ASCharacter::GetProjectileTransformFromTraceEnd(FVector Destination)
{
	FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

	FVector SpawnVector = Destination - HandLocation; //here we will remove the memeber variable and use the trace end location. Which will be the impact point. so we need to move this code to the CameraTrace function. and rename the function

	FRotator ProjectileRotation =  SpawnVector.Rotation();
	
	FTransform SpawnTM = FTransform(ProjectileRotation, HandLocation);
	return SpawnTM;
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(HealOrDamageSwitchParamName, 1.0f);
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
	}
	if (Delta > 0.0f && NewHealth > 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(HealOrDamageSwitchParamName, 0.0f);
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
	}
	
	if (NewHealth <= 0 && Delta <= 0)
	{
		
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			DisableInput(PC);
		}
		
	}
}

