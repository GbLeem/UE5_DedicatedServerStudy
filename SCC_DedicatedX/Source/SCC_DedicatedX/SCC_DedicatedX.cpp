// Copyright Epic Games, Inc. All Rights Reserved.

#include "SCC_DedicatedX.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, SCC_DedicatedX, "SCC_DedicatedX" );

#pragma region GeneralLogging

DEFINE_LOG_CATEGORY(LogDX);

TAutoConsoleVariable<bool>CVarDebugGeneral(TEXT("DX.Debug.General"), false, TEXT("일반적인 디버깅 정보 띄우기\n"), ECVF_Cheat);

#pragma endregion

#pragma region NetLogging

DEFINE_LOG_CATEGORY(LogDXNet);

#pragma endregion