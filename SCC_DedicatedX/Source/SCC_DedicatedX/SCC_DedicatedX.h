// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#pragma region GeneralLogging

SCC_DEDICATEDX_API DECLARE_LOG_CATEGORY_EXTERN(LogDX, Log, All);

#define DXCheckF(Expression, Format, ...)\
{\
	if((Expression) == false)\
	{\
		UE_LOG(LogDX, Error, TEXT("[%s(%d)] " Format), *FString(__FUNCTION__), __LINE__, ##__VA_ARGS__);\
		check(Expression);\
	}\
}

extern TAutoConsoleVariable<bool> CVarDebugGeneral;

#pragma endregion

#pragma region NetLogging

SCC_DEDICATEDX_API DECLARE_LOG_CATEGORY_EXTERN(LogDXNet, Log, All);

#define NETMODE_TCHAR ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("Client%02d"), UE::GetPlayInEditorID()) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("StandAlone") : TEXT("Server")))
#define FUNCTION_TCHAR (ANSI_TO_TCHAR(__FUNCTION__))
#define DX_LOG_NET(LogCategory, Verbosity, Format, ...) UE_LOG(LogCategory, Verbosity, TEXT("[%s] %s %s"), NETMODE_TCHAR, FUNCTION_TCHAR, *FString::Printf(Format, ##__VA_ARGS__))

#pragma endregion

#include "CoreMinimal.h"


