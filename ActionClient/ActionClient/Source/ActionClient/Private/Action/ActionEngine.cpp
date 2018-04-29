// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionClient.h"
#include "ActionEngine.h"
#include "NetworkManager.h"
#include "InputManager.h"
#include "ActionTiming.h"
#include "ActionHelper.h"
#include "ActionWorld.h"
#include "ActionPlayerController.h"
#include "ActionPawn.h"


// Sets default values for this component's properties
UActionEngine::UActionEngine( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	// ...

	ip = TEXT( "127.0.0.1" );
	port = 45000;
	player_name = TEXT( "ActionClientTestPlayerName" );

}

// Called when the game starts
void UActionEngine::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//int32 testFloatPrint = 88;
	//A_LOG_N( "testFloatPrint : ", testFloatPrint );
	//A_LOG_N( "( float )testFloatPrint : ", ( float )testFloatPrint );

	////A_SCREENMSG_F
	//A_SCREENMSG_F( "testFloatPrint : ", testFloatPrint );
	//A_SCREENMSG_F( "( float )testFloatPrint : ", ( float )testFloatPrint );

	ActionWorld::StaticInit();
	InputManager::StaticInit();
	NetworkManager::StaticInit( ip, port, player_name );


	//NetworkManager::sInstance->SetGameObjectRegistryUObj( NewObject<UGameObjectRegistryUObj>( this ), GetWorld() );

	//NetworkManager::sInstance->GetReplicationManagerClient()->SetDefaultCharacterClasses( DefaultCharacterClasses );
	NetworkManager::sInstance->GetGameObjectRegistryUObj()->SetDefaultCharacterClasses( DefaultCharacterClasses );
}

void UActionEngine::UpdateLocalPlayer()
{
	AActionPlayerController* const FirstPC = Cast<AActionPlayerController>( UGameplayStatics::GetPlayerController( GetWorld(), 0 ) );
	if ( FirstPC != nullptr && ( FirstPC->GetPawn() ) )
	{
		AActionPawn *p = Cast<AActionPawn>( FirstPC->GetPawn() );
		if ( p )
		{
			p->SimulateMovementAfterReplay();
		}
	}
}


// Called every frame
void UActionEngine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	ActionTiming::sInstance.Update();

	InputManager::sInstance->Update();

	ActionWorld::sInstance->Update();

	NetworkManager::sInstance->ProcessIncomingPackets();

	//UpdateLocalPlayer();

	NetworkManager::sInstance->SendOutgoingPackets();
}