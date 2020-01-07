#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "UObject/WeakObjectPtr.h"
#include "Layout/Visibility.h"
#include "IDetailCustomization.h"

class IDetailLayoutBuilder;
class IPropertyHandle;
class SWidget;
class UBlackboardData;

class FCGNodeBlackBoardDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;

	bool IsEditingEnabled() const;

private:
	void InitPropertyValues();

	void CacheBlackboardData(IDetailLayoutBuilder& DetailLayout);

	EVisibility GetIntValueVisibility() const;
	EVisibility GetFloatValueVisibility() const;
	EVisibility GetStringValueVisibility() const;
	EVisibility GetEnumValueVisibility() const;
	EVisibility GetBasicOpVisibility() const;
	EVisibility GetArithmeticOpVisibility() const;
	EVisibility GetTextOpVisibility() const;

	void OnKeyIDChanged();

	void OnEnumValueComboChange(int32 Index);
	TSharedRef<SWidget> OnGetEnumValueContent() const;
	FText GetCurrentEnumValueDesc() const;

	TSharedPtr<IPropertyHandle> IntValueProperty;
	TSharedPtr<IPropertyHandle> KeyIDProperty;
	TSharedPtr<IPropertyHandle> NotifyObserverProperty;

	TSharedPtr<IPropertyHandle> ModeProperty;

	/** cached type of property selected by KeyName */
	TSubclassOf<class UBlackboardKeyType> CachedKeyType;
	/** cached custom object type of property selected by KeyName */
	UEnum* CachedCustomObjectType;
	/** cached operation for key type */
	uint8 CachedOperationType;

	TArray<FString> EnumPropValues;

	TWeakObjectPtr<UBlackboardData> CachedBlackboardAsset;
	class IPropertyUtilities* PropUtils;
};