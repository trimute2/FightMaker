// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraphFactory.h"
#include "ComboGraph/ComboGraph.h"


UComboGraphFactory::UComboGraphFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UComboGraph::StaticClass();
}

UObject* UComboGraphFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UComboGraph>(InParent, Class, Name, Flags);
}