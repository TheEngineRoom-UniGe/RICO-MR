#include "ROSInterface.h"

void IRosInterface::Connect(const FString& InIp, const int32& InPort)
{
  UE_LOG(LogROS, Log, TEXT("Connecting to %s:%d"), *InIp, InPort);
  ROSHandler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(InIp, InPort));
  ROSHandler->Connect();
}

void IRosInterface::Disconnect()
{
  if (ROSHandler.IsValid())
    {
      ROSHandler->Disconnect();
    }
}
