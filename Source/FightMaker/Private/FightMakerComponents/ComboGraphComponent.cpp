// Fill out your copyright notice in the Description page of Project Settings.


#include "FightMakerComponents/ComboGraphComponent.h"

// Sets default values for this component's properties
UComboGraphComponent::UComboGraphComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	// ...
}

void UComboGraphComponent::InitializeComponent()
{
	Super::InitializeComponent();

	AActor* Owner = GetOwner();
	if (Owner) {
		BlackboardComp = Owner->FindComponentByClass<UBlackboardComponent>();
	}
}

bool UComboGraphComponent::InitializeComboGraph(UComboGraph & NewAsset)
{
	if (&NewAsset == ComboGraphAsset) {
		return false;
	}
	//might brak this up into more than one function like behavior tree component
	if (NewAsset.BlackboardAsset && BlackboardComp && !BlackboardComp->IsCompatibleWith(NewAsset.BlackboardAsset)) {
		return false;
	}

	ComboGraphAsset = &NewAsset;

	ComboGraphAsset->Refresh();

	/*if (BlackboardComp) {
		BlackboardComp->InitializeBlackboard(*ComboGraphAsset->BlackboardAsset);
	}*/

	return false;
}

// Called when the game starts
void UComboGraphComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UComboGraphComponent::TestGraph()
{
	if (ComboGraphAsset && BlackboardComp) {
		FFMAction response;
		response = ComboGraphAsset->Evaluate(BlackboardComp);
		if (response.ConfirmAction.IsBound()) {
			response.ConfirmAction.Broadcast(response.ActionInfo,BlackboardComp,GetOwner());
		}
	}
}

// Called every frame
void UComboGraphComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

