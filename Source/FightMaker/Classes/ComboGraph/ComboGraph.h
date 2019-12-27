// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CGNode.h"
#include "CGNode_Branching.h"
#include "FMAction.h"
#include "ComboGraph.generated.h"

FIGHTMAKER_API DECLARE_LOG_CATEGORY_EXTERN(CGraphsystem, Log, All);

#if WITH_EDITOR
class UComboGraph;

/** class for interfacing with module*/
class IComboGraphModuleInterface {
public:
	virtual ~IComboGraphModuleInterface(){}

	virtual UEdGraph* CreateNewComboGraphGraph(UComboGraph* ComboGraph) = 0;

	virtual void SetupAssetNode(UEdGraph* CGGraph, class UCGNode* InComboNode, bool bSelectNewNode) = 0;

	virtual void UpdateBlackBoardAsset(UEdGraph* ComboGraph) = 0;

	virtual void CompileAssetNodesFromGraphNodes(UComboGraph* ComboGraph) = 0;

	virtual void RemoveNullNodes(UComboGraph* ComboGraph) = 0;
};
#endif

class UBlackboardData;

/**
 * 
 */
UCLASS(BlueprintType)
class FIGHTMAKER_API UComboGraph : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
#if WITH_EDITORONLY_DATA
	/** EdGraph based representation of the ComboGraph */
	UPROPERTY()
	class UEdGraph* ComboGraphGraph;

#endif

	UPROPERTY(EditAnywhere)
	UBlackboardData* BlackboardAsset;

	//MoveData evaluate(blackboard)
	FFMAction Evaluate(FFMAction& OutputAction, class UBlackboardComponent* blackboard, int FirstEvalIndex);

	

protected:

	UPROPERTY()
	UCGNode_Branching *RootNode; //the root node

	UPROPERTY()
	TArray<UCGNode*> BaseNodes; //The array of all nodes set in the graph

public:
#if WITH_EDITOR
	////////////////////////////////////////////////
	//used for creating newgraphs and stuff

	virtual void PostInitProperties() override;
	static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);

	void CreateGraph();

	void SetUpNode(UCGNode* CGNode, bool bSelectNewNode = true);

	void RemoveNodeFromBase(UCGNode* CGNode);

	////////////////////////////////////////////////
	//Used by editor to display and edit stuff

	class UEdGraph* GetGraph();

#endif
	virtual void PostLoad() override;

	template<class T>
	T* ConstructComboNode(TSubclassOf<UCGNode> ComboNodeClass = T::StaticClass(), bool bSelectNewNode = true) {
		T* CGNode = NewObject<T>(this, ComboNodeClass, NAME_None, RF_Transactional);
#if WITH_EDITOR
		BaseNodes.Add(CGNode);
		SetUpNode(CGNode, bSelectNewNode);
#endif
		return CGNode;
	}

#if WITH_EDITOR


	////////////////////////////////////////////////
	//Used To interface with editor module
	static TSharedPtr<IComboGraphModuleInterface> GetComboGraphModuleInterface();

	static void SetComboGraphModuleInterface(TSharedPtr<IComboGraphModuleInterface> InComboGraphModuleInterface);

	void CompileAssetNodesFromGraphNodes();

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	int FindNode(UCGNode* node);

	UCGNode_Branching* GetRootNode();

private:

	static TSharedPtr<IComboGraphModuleInterface> ComboGraphModuleInterface;

#endif
public:
	virtual void Serialize(FStructuredArchive::FRecord Record) override;

	void Refresh();
	
};
