// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CGNode.h"
#include "CGNode_Branching.h"
#include "FMAction.h"
#include "ComboGraph.generated.h"

FIGHTMAKER_API DECLARE_LOG_CATEGORY_EXTERN(CGraphsystem, Log, All);
/*
might not need this keeping this here since i already typed it out andmight need it later
enum EInuputEvents
{
	NOInput = 0,
	I01 = 0x0001,
	I02 = 0x0002,
	I03 = 0x0004,
	I04 = 0x0008,
	I05 = 0x0010,
	I06 = 0x0020,
	I07 = 0x0040,
	I08 = 0x0080,
	I09 = 0x0100,
	I10 = 0x0200,
	I11 = 0x0400,
	I12 = 0x0800,
	I13 = 0x1000,
	I14 = 0x2000,
	I15 = 0x4000,
	I16 = 0x8000
};*/

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


	UPROPERTY()
	UCGNode_Branching *RootNode; //the root node

protected:

	//TODO: decouple root nodes array from the functionality of root nodes
	UPROPERTY()
	TArray<class UCGNode*> RootNodes;

	////////////////////////////////////////////////
	// the arrays that we should be keeping

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

	void MakeNodeRoot(class UCGNode* newRoot);

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

private:

	static TSharedPtr<IComboGraphModuleInterface> ComboGraphModuleInterface;

#endif
public:
	virtual void Serialize(FStructuredArchive::FRecord Record) override;

	void Refresh();
	
};
