#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCS, Log, All);

#define CS_LOG(Format, ...) \
UE_LOG(LogCS, Warning, TEXT(Format), ##__VA_ARGS__)

#define CS_LOG_WARNING(Format, ...) \
UE_LOG(LogCS, Warning, TEXT(Format), ##__VA_ARGS__)

#define CS_LOG_ERROR(Format, ...) \
UE_LOG(LogCS, Error, TEXT(Format), ##__VA_ARGS__)