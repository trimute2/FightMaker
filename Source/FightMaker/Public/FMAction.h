#pragma once

#include "Animation/AnimMontage.h"
#include "FMAction.generated.h"

class UCGNode;

USTRUCT(BlueprintType)
struct FFMActionInfo {
	GENERATED_USTRUCT_BODY();

	FFMActionInfo() : AnimSpeed(1.0f), Priority(0)
	{}

	UPROPERTY(EditInstanceOnly, Category = Animation)
	UAnimMontage* ActionMontage;

	UPROPERTY(EditInstanceOnly, Category = Animation)
	float AnimSpeed;

	UPROPERTY(EditInstanceOnly, Category = Properties)
	bool bCanCancel;

	UPROPERTY(EditInstanceOnly, Category = Properties)
	int Priority;

	UPROPERTY(EditInstanceOnly, Category = Properties)
	float AttackDamage;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FActionConditionResponseDelegate,struct FFMActionInfo, ActionInfo, class UBlackboardComponent*, BlackBoardInfo, class AActor*, Actor);

USTRUCT(BlueprintType)
struct FFMAction {
	GENERATED_USTRUCT_BODY()

	FFMAction() : NextIndex(-1)
	{}

	UPROPERTY(EditInstanceOnly)
	FFMActionInfo ActionInfo; //the info that describes the attack

	UPROPERTY()
	bool UseableResponse;

	UPROPERTY()
	bool FirstEvaluation;

	UPROPERTY(BlueprintAssignable)
	FActionConditionResponseDelegate RegisterAction; //delegates called to register the attack

	UPROPERTY(BlueprintAssignable)
	FActionConditionResponseDelegate ConfirmAction;  //delegates called to confirm the attack and consume player resources (stamina, meter etc)

	UPROPERTY(BlueprintAssignable)
	FActionConditionResponseDelegate RollbackAction; //delegate called to rollback register

	UPROPERTY()
	UCGNode * EvaluatedNext;

	UPROPERTY()
	int NextIndex;
};

