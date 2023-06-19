#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>

#include "KafkaConnectionParams.generated.h"

/** Structure that defines a row of DH params */
USTRUCT(BlueprintType)
struct FKafkaConnectionParams : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KafkaConnection")
		FString Servers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KafkaConnection")
		FString UserName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KafkaConnection")
		FString Password;

	FKafkaConnectionParams() {
		Servers = "";
		UserName = "";
		Password = "";
	}
};
