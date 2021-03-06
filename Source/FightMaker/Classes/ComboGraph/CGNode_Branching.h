// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboGraph/CGNode.h"
#include "CGNode_Branching.generated.h"

/**
 * 
 */
UCLASS(meta=(HiddenNode))
class FIGHTMAKER_API UCGNode_Branching : public UCGNode
{
	GENERATED_BODY()
	
public:

	virtual void EvaluateNode(FFMAction& ActionOutput, class UBlackboardComponent* blackboard) override;

};
