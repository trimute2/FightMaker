#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "UnrealClient.h"
#include "IPropertyTypeCustomization.h"

class IPropertyHandle;
class UBlackboardData;

//I Just copy pasted this from Black board selector becuase Unreal only has blackboard selector work with Behavior tree
class FFMBlackboardSelectorDetails : public IPropertyTypeCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

private:

	void CacheBlackboardData();
	const class UBlackboardData* FindBlackboardAsset(UObject* InObj);

	void InitKeyFromProperty();
	void OnKeyComboChange(int32 Index);
	TSharedRef<SWidget> OnGetKeyContent() const;
	FText GetCurrentKeyDesc() const;
	bool IsEditingEnabled() const;

	TSharedPtr<IPropertyHandle> MyStructProperty;
	TSharedPtr<IPropertyHandle> MyKeyNameProperty;
	TSharedPtr<IPropertyHandle> MyKeyIDProperty;
	TSharedPtr<IPropertyHandle> MyKeyClassProperty;

	/** cached names of keys */
	TArray<FName> KeyValues;

	bool bNoneIsAllowedValue;

	/** cached blackboard asset */
	TWeakObjectPtr<class UBlackboardData> CachedBlackboardAsset;

	/** property utils */
	class IPropertyUtilities* PropUtils;
};