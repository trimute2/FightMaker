#include "ComboGraph\ComboGraphTypes.h"
#include "AIModule\Classes\BehaviorTree\BehaviorTreeTypes.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_NativeEnum.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "VisualLogger/VisualLogger.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Name.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"

//----------------------------------------------------------------------//
// FFMBlackboardKeySelector
//----------------------------------------------------------------------//
void FFMBlackboardKeySelector::ResolveSelectedKey(const UBlackboardData& BlackboardAsset)
{
	if (SelectedKeyName.IsNone() == false || !bNoneIsAllowedValue)
	{
		if (SelectedKeyName.IsNone() && !bNoneIsAllowedValue)
		{
			InitSelection(BlackboardAsset);
		}

		SelectedKeyID = BlackboardAsset.GetKeyID(SelectedKeyName);
		SelectedKeyType = BlackboardAsset.GetKeyType(SelectedKeyID);
		/*UE_CLOG(IsSet() == false, LogBehaviorTree, Warning
			, TEXT("%s> Failed to find key \'%s\' in BB asset %s. BB Key Selector will be set to \'Invalid\'")
			, *UBehaviorTreeTypes::GetBTLoggingContext()
			, *SelectedKeyName.ToString()
			, *BlackboardAsset.GetFullName()
		);*/
	}
}

void FFMBlackboardKeySelector::InitSelection(const UBlackboardData& BlackboardAsset)
{
	for (const UBlackboardData* It = &BlackboardAsset; It; It = It->Parent)
	{
		for (int32 KeyIndex = 0; KeyIndex < It->Keys.Num(); KeyIndex++)
		{
			const FBlackboardEntry& EntryInfo = It->Keys[KeyIndex];
			if (EntryInfo.KeyType)
			{
				bool bFilterPassed = true;
				if (AllowedTypes.Num())
				{
					bFilterPassed = false;
					for (int32 FilterIndex = 0; FilterIndex < AllowedTypes.Num(); FilterIndex++)
					{
						if (EntryInfo.KeyType->IsAllowedByFilter(AllowedTypes[FilterIndex]))
						{
							bFilterPassed = true;
							break;
						}
					}
				}

				if (bFilterPassed)
				{
					SelectedKeyName = EntryInfo.EntryName;
					break;
				}
			}
		}
	}
}

void FFMBlackboardKeySelector::AddObjectFilter(UObject* Owner, FName PropertyName, TSubclassOf<UObject> AllowedClass)
{
	const FName FilterName = MakeUniqueObjectName(Owner, UBlackboardKeyType_Object::StaticClass(), *FString::Printf(TEXT("%s_Object"), *PropertyName.ToString()));
	UBlackboardKeyType_Object* FilterOb = NewObject<UBlackboardKeyType_Object>(Owner, FilterName);
	FilterOb->BaseClass = AllowedClass;
	AllowedTypes.Add(FilterOb);
}

void FFMBlackboardKeySelector::AddClassFilter(UObject* Owner, FName PropertyName, TSubclassOf<UClass> AllowedClass)
{
	const FName FilterName = MakeUniqueObjectName(Owner, UBlackboardKeyType_Class::StaticClass(), *FString::Printf(TEXT("%s_Class"), *PropertyName.ToString()));
	UBlackboardKeyType_Class* FilterOb = NewObject<UBlackboardKeyType_Class>(Owner, FilterName);
	FilterOb->BaseClass = AllowedClass;
	AllowedTypes.Add(FilterOb);
}

void FFMBlackboardKeySelector::AddEnumFilter(UObject* Owner, FName PropertyName, UEnum* AllowedEnum)
{
	const FName FilterName = MakeUniqueObjectName(Owner, UBlackboardKeyType_Enum::StaticClass(), *FString::Printf(TEXT("%s_Enum"), *PropertyName.ToString()));
	UBlackboardKeyType_Enum* FilterOb = NewObject<UBlackboardKeyType_Enum>(Owner, FilterName);
	FilterOb->EnumType = AllowedEnum;
	AllowedTypes.Add(FilterOb);
}

void FFMBlackboardKeySelector::AddNativeEnumFilter(UObject* Owner, FName PropertyName, const FString& AllowedEnumName)
{
	const FName FilterName = MakeUniqueObjectName(Owner, UBlackboardKeyType_NativeEnum::StaticClass(), *FString::Printf(TEXT("%s_NativeEnum"), *PropertyName.ToString()));
	UBlackboardKeyType_NativeEnum* FilterOb = NewObject<UBlackboardKeyType_NativeEnum>(Owner, FilterName);
	FilterOb->EnumName = AllowedEnumName;
	AllowedTypes.Add(FilterOb);
}

void FFMBlackboardKeySelector::AddIntFilter(UObject* Owner, FName PropertyName)
{
	const FString FilterName = PropertyName.ToString() + TEXT("_Int");
	AllowedTypes.Add(NewObject<UBlackboardKeyType_Int>(Owner, *FilterName));
}

void FFMBlackboardKeySelector::AddFloatFilter(UObject* Owner, FName PropertyName)
{
	const FString FilterName = PropertyName.ToString() + TEXT("_Float");
	AllowedTypes.Add(NewObject<UBlackboardKeyType_Float>(Owner, *FilterName));
}

void FFMBlackboardKeySelector::AddBoolFilter(UObject* Owner, FName PropertyName)
{
	const FString FilterName = PropertyName.ToString() + TEXT("_Bool");
	AllowedTypes.Add(NewObject<UBlackboardKeyType_Bool>(Owner, *FilterName));
}

void FFMBlackboardKeySelector::AddVectorFilter(UObject* Owner, FName PropertyName)
{
	const FString FilterName = PropertyName.ToString() + TEXT("_Vector");
	AllowedTypes.Add(NewObject<UBlackboardKeyType_Vector>(Owner, *FilterName));
}

void FFMBlackboardKeySelector::AddRotatorFilter(UObject* Owner, FName PropertyName)
{
	const FString FilterName = PropertyName.ToString() + TEXT("_Rotator");
	AllowedTypes.Add(NewObject<UBlackboardKeyType_Rotator>(Owner, *FilterName));
}

void FFMBlackboardKeySelector::AddStringFilter(UObject* Owner, FName PropertyName)
{
	const FString FilterName = PropertyName.ToString() + TEXT("_String");
	AllowedTypes.Add(NewObject<UBlackboardKeyType_String>(Owner, *FilterName));
}

void FFMBlackboardKeySelector::AddNameFilter(UObject* Owner, FName PropertyName)
{
	const FString FilterName = PropertyName.ToString() + TEXT("_Name");
	AllowedTypes.Add(NewObject<UBlackboardKeyType_Name>(Owner, *FilterName));
}