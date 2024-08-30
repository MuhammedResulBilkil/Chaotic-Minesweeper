#pragma once
UENUM()
enum ECellType
{
	ECT_Empty UMETA(DisplayName = "Empty"),
	ECT_Mine UMETA(DisplayName = "Mine"),
	ECT_Revealed UMETA(DisplayName = "Revealed"),
};