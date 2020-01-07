#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree\Blackboard\BlackboardKeyType.h"
#include "ComboGraphTypes.generated.h"

namespace FFMBlackboard
{
	const FName KeySelf = TEXT("SelfActor");

	typedef uint8 FKey;

	const FKey InvalidKey = FKey(-1);
}

class FCGNodeBlackBoardDetails;

USTRUCT(BlueprintType)
struct FIGHTMAKER_API FFMBlackboardKeySelector
{
	GENERATED_USTRUCT_BODY()

	FFMBlackboardKeySelector() : SelectedKeyID(FFMBlackboard::InvalidKey), bNoneIsAllowedValue(false)
	{}

	/** array of allowed types with additional properties (e.g. uobject's base class)
	  * EditAnywhere is required for FFMBlackboardSelectorDetails::CacheBlackboardData()*/
	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Blackboard)
	TArray<UBlackboardKeyType*> AllowedTypes;

	/** name of selected key */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Blackboard)
	FName SelectedKeyName;

	/** class of selected key  */
	UPROPERTY(transient, EditInstanceOnly, BlueprintReadWrite, Category = Blackboard)
	TSubclassOf<UBlackboardKeyType> SelectedKeyType;

protected:
	/** ID of selected key */
	UPROPERTY(transient, EditInstanceOnly, BlueprintReadWrite, Category = Blackboard)
	uint8 SelectedKeyID;
	// SelectedKeyId type should be FFMBlackboard::FKey, but typedefs are not supported by UHT
	static_assert(sizeof(uint8) == sizeof(FFMBlackboard::FKey), "FFMBlackboardKeySelector::SelectedKeyId should be of FFMBlackboard::FKey-compatible type.");

	// Requires BlueprintReadWrite so that blueprint creators (using MakeBlackboardKeySelector) can specify whether or not None is Allowed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blackboard, Meta = (Tooltip = ""))
	uint32 bNoneIsAllowedValue:1;

	/** find initial selection. Called when None is not a valid option for this key selector */
	void InitSelection(const UBlackboardData& BlackboardAsset);

public:
	/** find ID and class of selected key */
	void ResolveSelectedKey(const UBlackboardData& BlackboardAsset);
		
	void AllowNoneAsValue(bool bAllow) { bNoneIsAllowedValue = bAllow; }

	FORCEINLINE FFMBlackboard::FKey GetSelectedKeyID() const { return SelectedKeyID; }

	/** helper functions for setting basic filters */
	void AddObjectFilter(UObject* Owner, FName PropertyName, TSubclassOf<UObject> AllowedClass);
	void AddClassFilter(UObject* Owner, FName PropertyName, TSubclassOf<UClass> AllowedClass);
	void AddEnumFilter(UObject* Owner, FName PropertyName, UEnum* AllowedEnum);
	void AddNativeEnumFilter(UObject* Owner, FName PropertyName, const FString& AllowedEnumName);
	void AddIntFilter(UObject* Owner, FName PropertyName);
	void AddFloatFilter(UObject* Owner, FName PropertyName);
	void AddBoolFilter(UObject* Owner, FName PropertyName);
	void AddVectorFilter(UObject* Owner, FName PropertyName);
	void AddRotatorFilter(UObject* Owner, FName PropertyName);
	void AddStringFilter(UObject* Owner, FName PropertyName);
	void AddNameFilter(UObject* Owner, FName PropertyName);

	FORCEINLINE bool IsNone() const { return bNoneIsAllowedValue && SelectedKeyID == FFMBlackboard::InvalidKey; }
	FORCEINLINE bool IsSet() const { return SelectedKeyID != FFMBlackboard::InvalidKey; }
	FORCEINLINE bool NeedsResolving() const { return SelectedKeyID == FFMBlackboard::InvalidKey && SelectedKeyName.IsNone() == false; }
	FORCEINLINE void InvalidateResolvedKey() { SelectedKeyID = FFMBlackboard::InvalidKey; }

	friend FCGNodeBlackBoardDetails;
};

/*
USTRUCT(BlueprintType)
struct AIMODULE_API FBlackboardKeySelector
{
	GENERATED_USTRUCT_BODY()

	FBlackboardKeySelector() : SelectedKeyID(FBlackboard::InvalidKey), bNoneIsAllowedValue(false)
	{}

	/** array of allowed types with additional properties (e.g. uobject's base class)
	  * EditAnywhere is required for FBlackboardSelectorDetails::CacheBlackboardData() 
UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Blackboard)
TArray<UBlackboardKeyType*> AllowedTypes;

/** name of selected key 
UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Blackboard)
FName SelectedKeyName;

/** class of selected key  
UPROPERTY(transient, EditInstanceOnly, BlueprintReadWrite, Category = Blackboard)
TSubclassOf<UBlackboardKeyType> SelectedKeyType;

protected:
	/** ID of selected key 
	UPROPERTY(transient, EditInstanceOnly, BlueprintReadWrite, Category = Blackboard)
		uint8 SelectedKeyID;
	// SelectedKeyId type should be FBlackboard::FKey, but typedefs are not supported by UHT
	static_assert(sizeof(uint8) == sizeof(FBlackboard::FKey), "FBlackboardKeySelector::SelectedKeyId should be of FBlackboard::FKey-compatible type.");

	// Requires BlueprintReadWrite so that blueprint creators (using MakeBlackboardKeySelector) can specify whether or not None is Allowed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blackboard, Meta = (Tooltip = ""))
		uint32 bNoneIsAllowedValue : 1;

	/** find initial selection. Called when None is not a valid option for this key selector 
	void InitSelection(const UBlackboardData& BlackboardAsset);

public:
	/** find ID and class of selected key 
	void ResolveSelectedKey(const UBlackboardData& BlackboardAsset);

	void AllowNoneAsValue(bool bAllow) { bNoneIsAllowedValue = bAllow; }

	FORCEINLINE FBlackboard::FKey GetSelectedKeyID() const { return SelectedKeyID; }

	/** helper functions for setting basic filters 
	void AddObjectFilter(UObject* Owner, FName PropertyName, TSubclassOf<UObject> AllowedClass);
	void AddClassFilter(UObject* Owner, FName PropertyName, TSubclassOf<UClass> AllowedClass);
	void AddEnumFilter(UObject* Owner, FName PropertyName, UEnum* AllowedEnum);
	void AddNativeEnumFilter(UObject* Owner, FName PropertyName, const FString& AllowedEnumName);
	void AddIntFilter(UObject* Owner, FName PropertyName);
	void AddFloatFilter(UObject* Owner, FName PropertyName);
	void AddBoolFilter(UObject* Owner, FName PropertyName);
	void AddVectorFilter(UObject* Owner, FName PropertyName);
	void AddRotatorFilter(UObject* Owner, FName PropertyName);
	void AddStringFilter(UObject* Owner, FName PropertyName);
	void AddNameFilter(UObject* Owner, FName PropertyName);

	FORCEINLINE bool IsNone() const { return bNoneIsAllowedValue && SelectedKeyID == FBlackboard::InvalidKey; }
	FORCEINLINE bool IsSet() const { return SelectedKeyID != FBlackboard::InvalidKey; }
	FORCEINLINE bool NeedsResolving() const { return SelectedKeyID == FBlackboard::InvalidKey && SelectedKeyName.IsNone() == false; }
	FORCEINLINE void InvalidateResolvedKey() { SelectedKeyID = FBlackboard::InvalidKey; }

	friend FBlackboardDecoratorDetails;
};
*/