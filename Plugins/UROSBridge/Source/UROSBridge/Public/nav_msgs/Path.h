#pragma once
#include "ROSBridgeMsg.h"

#include <std_msgs/Header.h>
#include <geometry_msgs/PoseStamped.h>

namespace nav_msgs
{
	class Path : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		TArray<geometry_msgs::PoseStamped> Poses;

	public:

		Path()
		{
			MsgType = "nav_msgs/Path";
		}

		Path(const std_msgs::Header& InHeader, const TArray<geometry_msgs::PoseStamped>& InPoses) :
			Header(InHeader), Poses(InPoses)
		{
			MsgType = "nav_msgs/Path";
		}

		~Path() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		TArray<geometry_msgs::PoseStamped> GetPoses() const
		{
			return Poses;
		}

		void SetHeader(const std_msgs::Header& InHeaders)
		{
			Header = InHeaders;
		}

		void SetPoses(const TArray<geometry_msgs::PoseStamped>& InPoses)
		{
			Poses = InPoses;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));
			TArray<TSharedPtr<FJsonValue>> PosesPtrArr;

			Poses.Empty();
			PosesPtrArr = JsonObject->GetArrayField(TEXT("poses"));
			for (auto& ptr : PosesPtrArr)
				Poses.Add(geometry_msgs::PoseStamped::GetFromJson(ptr->AsObject()));
		}

		static Path GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Path Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			FString PosesString = "[ ";
			for (auto& value : Poses)
				PosesString += value.ToString() + TEXT(", ");
			PosesString += " ]";

			return TEXT("Path { header = ") + Header.ToString() +
				TEXT(", poses = ") + PosesString +
				TEXT(" } ");
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			TArray<TSharedPtr<FJsonValue>> PosesPtrArray;
			for (auto& pose : Poses)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(pose.ToJsonObject()));
				PosesPtrArray.Add(Ptr);
			}

			Object->SetObjectField(TEXT("header"), Header.ToJsonObject());
			Object->SetArrayField(TEXT("poses"), PosesPtrArray);

			return Object;
		}

		virtual FString ToYamlString() const override
		{
			FString OutputString;
			TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
			FJsonSerializer::Serialize(ToJsonObject().ToSharedRef(), Writer);
			return OutputString;
		}
	};
} // namespace nav_msgs