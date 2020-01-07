
#include "DetailCustomizations/CGNodeBlackBoardDetails.h"
#include "Misc/Attribute.h"
#include "Layout/Margin.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "SlateOptMacros.h"
#include "Textures/SlateIcon.h"
#include "Framework/Commands/UIAction.h"
#include "Widgets/SWidget.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Input/SComboButton.h"
#include "ComboGraph/CGNode.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailPropertyRow.h"
#include "DetailCategoryBuilder.h"
#include "ComboGraph/Nodes/CGNode_BlackBoardBase.h"
#include "BehaviorTree/BlackboardData.h"
#include "ComboGraph/Nodes/CGNode_BlackBoard.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_NativeEnum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"

#include "IPropertyUtilities.h"

#define LOCTEXT_NAMESPACE "CGNodeBlackBoardDetails"

TSharedRef<IDetailCustomization> FCGNodeBlackBoardDetails::MakeInstance()
{
	return MakeShareable(new FCGNodeBlackBoardDetails);
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void FCGNodeBlackBoardDetails::CustomizeDetails(IDetailLayoutBuilder & DetailLayout)
{
	PropUtils = &(DetailLayout.GetPropertyUtilities().Get());
	//InitPropertyValues();

	TArray<TWeakObjectPtr<UObject> > EditedObjects;
	DetailLayout.GetObjectsBeingCustomized(EditedObjects);

	CacheBlackboardData(DetailLayout);

	const bool bIsEnabled = CachedBlackboardAsset.IsValid();
	TAttribute<bool> PropertyEditCheck = TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateSP(this, &FCGNodeBlackBoardDetails::IsEditingEnabled));

	/*IDetailCategoryBuilder& FlowCategory = DetailLayout.EditCategory("FlowControl");
	NotifyObserverProperty = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UBTDecorator_Blackboard, NotifyObserver));
	IDetailPropertyRow& AbortRow = FlowCategory.AddProperty(NotifyObserverProperty);
	AbortRow.IsEnabled(PropertyEditCheck);*/

	IDetailCategoryBuilder& BBCategory = DetailLayout.EditCategory("Blackboard");
	IDetailPropertyRow& KeySelectorRow = BBCategory.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UCGNode_BlackBoard, BlackboardKey)));
	KeySelectorRow.IsEnabled(bIsEnabled);

	//KeyIDProperty = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UCGNode_BlackBoard, BlackboardKey.SelectedKeyID), UCGNode_BlackBoardBase::StaticClass());
	KeyIDProperty = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UCGNode_BlackBoard, BlackboardKey.SelectedKeyID), UCGNode_BlackBoardBase::StaticClass());
	if (KeyIDProperty.IsValid())
	{
		FSimpleDelegate OnKeyChangedDelegate = FSimpleDelegate::CreateSP(this, &FCGNodeBlackBoardDetails::OnKeyIDChanged);
		KeyIDProperty->SetOnPropertyValueChanged(OnKeyChangedDelegate);
		OnKeyIDChanged();
	}

#if WITH_EDITORONLY_DATA

	IDetailPropertyRow& BasicOpRow = BBCategory.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UCGNode_BlackBoard, BasicOperation)));
	BasicOpRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FCGNodeBlackBoardDetails::GetBasicOpVisibility)));
	BasicOpRow.IsEnabled(PropertyEditCheck);

	IDetailPropertyRow& ArithmeticOpRow = BBCategory.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UCGNode_BlackBoard, ArithmeticOperation)));
	ArithmeticOpRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FCGNodeBlackBoardDetails::GetArithmeticOpVisibility)));
	ArithmeticOpRow.IsEnabled(PropertyEditCheck);

	IDetailPropertyRow& TextOpRow = BBCategory.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UCGNode_BlackBoard, TextOperation)));
	TextOpRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FCGNodeBlackBoardDetails::GetTextOpVisibility)));
	TextOpRow.IsEnabled(PropertyEditCheck);

#endif // WITH_EDITORONLY_DATA

	IntValueProperty = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UCGNode_BlackBoard, IntValue));
	IDetailPropertyRow& IntValueRow = BBCategory.AddProperty(IntValueProperty);
	IntValueRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FCGNodeBlackBoardDetails::GetIntValueVisibility)));
	IntValueRow.IsEnabled(PropertyEditCheck);

	IDetailPropertyRow& FloatValueRow = BBCategory.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UCGNode_BlackBoard, FloatValue)));
	FloatValueRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FCGNodeBlackBoardDetails::GetFloatValueVisibility)));
	FloatValueRow.IsEnabled(PropertyEditCheck);

	IDetailPropertyRow& StringValueRow = BBCategory.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UCGNode_BlackBoard, StringValue)));
	StringValueRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FCGNodeBlackBoardDetails::GetStringValueVisibility)));
	StringValueRow.IsEnabled(PropertyEditCheck);

	IDetailPropertyRow& EnumValueRow = BBCategory.AddProperty(IntValueProperty);
	EnumValueRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FCGNodeBlackBoardDetails::GetEnumValueVisibility)));
	EnumValueRow.IsEnabled(PropertyEditCheck);
	EnumValueRow.CustomWidget()
		.NameContent()
		[
			IntValueProperty->CreatePropertyNameWidget()
		]
	.ValueContent()
		[
			SNew(SComboButton)
			.OnGetMenuContent(this, &FCGNodeBlackBoardDetails::OnGetEnumValueContent)
		.ContentPadding(FMargin(2.0f, 2.0f))
		.ButtonContent()
		[
			SNew(STextBlock)
			.Text(this, &FCGNodeBlackBoardDetails::GetCurrentEnumValueDesc)
		.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool FCGNodeBlackBoardDetails::IsEditingEnabled() const
{
	return PropUtils->IsPropertyEditingEnabled();
}

void FCGNodeBlackBoardDetails::InitPropertyValues()
{
	uint8 ByteValue;
	ModeProperty->GetValue(ByteValue);
	//OnAbortModeChange(ByteValue);
}

void FCGNodeBlackBoardDetails::CacheBlackboardData(IDetailLayoutBuilder & DetailLayout)
{
	TArray<TWeakObjectPtr<UObject> > MyOuters;
	DetailLayout.GetObjectsBeingCustomized(MyOuters);

	CachedBlackboardAsset.Reset();
	for (int32 i = 0; i < MyOuters.Num(); i++)
	{
		UCGNode* NodeOb = Cast<UCGNode>(MyOuters[i].Get());
		if (NodeOb)
		{
			CachedBlackboardAsset = NodeOb->GetBlackboardAsset();
			break;
		}
	}
}


void FCGNodeBlackBoardDetails::OnKeyIDChanged()
{
	CachedOperationType = EBlackboardKeyOperation::Basic;
	CachedKeyType = NULL;

	UBlackboardData* Blackboard = CachedBlackboardAsset.Get();
	if (Blackboard == NULL)
	{
		return;
	}

	uint8 KeyID;
	FPropertyAccess::Result Result = KeyIDProperty->GetValue(KeyID);
	UProperty *Test = KeyIDProperty->GetProperty();
	if (Result == FPropertyAccess::Success && Test->IsA<UByteProperty>())
	{
		const FBlackboardEntry* KeyEntry = Blackboard->GetKey(KeyID);
		if (KeyEntry && KeyEntry->KeyType)
		{
			CachedKeyType = KeyEntry->KeyType->GetClass();
			CachedOperationType = KeyEntry->KeyType->GetTestOperation();
		}
		else
		{
			CachedKeyType = nullptr;
			CachedOperationType = 0;
		}
	}

	// special handling of enum type: cache all names for combo box (display names)
	UEnum* SelectedEnumType = NULL;
	if (CachedKeyType == UBlackboardKeyType_Enum::StaticClass())
	{
		const FBlackboardEntry* EntryInfo = Blackboard->GetKey(KeyID);
		SelectedEnumType = ((UBlackboardKeyType_Enum*)(EntryInfo->KeyType))->EnumType;
	}
	else if (CachedKeyType == UBlackboardKeyType_NativeEnum::StaticClass())
	{
		const FBlackboardEntry* EntryInfo = Blackboard->GetKey(KeyID);
		SelectedEnumType = ((UBlackboardKeyType_NativeEnum*)(EntryInfo->KeyType))->EnumType;
	}

	if (SelectedEnumType)
	{
		CachedCustomObjectType = SelectedEnumType;
		EnumPropValues.Reset();

		if (CachedCustomObjectType)
		{
			for (int32 i = 0; i < CachedCustomObjectType->NumEnums() - 1; i++)
			{
				FString DisplayedName = CachedCustomObjectType->GetDisplayNameTextByIndex(i).ToString();
				EnumPropValues.Add(DisplayedName);
			}
		}
	}
}

TSharedRef<SWidget> FCGNodeBlackBoardDetails::OnGetEnumValueContent() const
{
	FMenuBuilder MenuBuilder(true, NULL);

	for (int32 i = 0; i < EnumPropValues.Num(); i++)
	{
		FUIAction ItemAction(FExecuteAction::CreateSP(const_cast<FCGNodeBlackBoardDetails*>(this), &FCGNodeBlackBoardDetails::OnEnumValueComboChange, i));
		MenuBuilder.AddMenuEntry(FText::FromString(EnumPropValues[i]), TAttribute<FText>(), FSlateIcon(), ItemAction);
	}

	return MenuBuilder.MakeWidget();
}

FText FCGNodeBlackBoardDetails::GetCurrentEnumValueDesc() const
{
	FPropertyAccess::Result Result = FPropertyAccess::Fail;
	int32 CurrentIntValue = INDEX_NONE;

	if (CachedCustomObjectType)
	{
		Result = IntValueProperty->GetValue(CurrentIntValue);
	}

	return (Result == FPropertyAccess::Success && EnumPropValues.IsValidIndex(CurrentIntValue))
		? FText::FromString(EnumPropValues[CurrentIntValue])
		: FText::GetEmpty();
}

void FCGNodeBlackBoardDetails::OnEnumValueComboChange(int32 Index)
{
	IntValueProperty->SetValue(Index);
}

EVisibility FCGNodeBlackBoardDetails::GetIntValueVisibility() const
{
	return (CachedKeyType == UBlackboardKeyType_Int::StaticClass()) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility FCGNodeBlackBoardDetails::GetFloatValueVisibility() const
{
	return (CachedKeyType == UBlackboardKeyType_Float::StaticClass()) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility FCGNodeBlackBoardDetails::GetStringValueVisibility() const
{
	return (CachedOperationType == EBlackboardKeyOperation::Text) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility FCGNodeBlackBoardDetails::GetEnumValueVisibility() const
{
	if (CachedKeyType == UBlackboardKeyType_Enum::StaticClass() ||
		CachedKeyType == UBlackboardKeyType_NativeEnum::StaticClass())
	{
		return EVisibility::Visible;
	}

	return EVisibility::Collapsed;
}

EVisibility FCGNodeBlackBoardDetails::GetBasicOpVisibility() const
{
	return (CachedOperationType == EBlackboardKeyOperation::Basic) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility FCGNodeBlackBoardDetails::GetArithmeticOpVisibility() const
{
	return (CachedOperationType == EBlackboardKeyOperation::Arithmetic) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility FCGNodeBlackBoardDetails::GetTextOpVisibility() const
{
	return (CachedOperationType == EBlackboardKeyOperation::Text) ? EVisibility::Visible : EVisibility::Collapsed;
}

#undef LOCTEXT_NAMESPACE