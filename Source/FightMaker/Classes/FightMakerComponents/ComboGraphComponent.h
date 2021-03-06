// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComboGraph/ComboGraph.h"
#include "FMAction.h"
#include "ComboGraphComponent.generated.h"

DECLARE_DELEGATE_OneParam(FActionResponseDelegate, FFMActionInfo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIGHTMAKER_API UComboGraphComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComboGraphComponent();

	/** BEGIN UActorComponent overrides */
	virtual void InitializeComponent() override;
	//virtual void UninitializeComponent() override;
	/** END UActorComponent overrides */

	UPROPERTY(transient)
	UComboGraph* ComboGraphAsset;

	UPROPERTY(transient)
	UBlackboardComponent* BlackboardComp;

	bool InitializeComboGraph(UComboGraph& NewAsset);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FFMAction CurrentAction;

public:	

	FActionResponseDelegate OnSetAction;

	void TestGraph();

	void EmptyCurrentAction(UAnimMontage* Montage, bool bInterrupted);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
