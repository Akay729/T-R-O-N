// Fill out your copyright notice in the Description page of Project Settings.


#include "TronGameMode.h"

#include "Tron/Player/TronPlayerController.h"

ATronGameMode::ATronGameMode()
{
	PlayerControllerClass = ATronPlayerController::StaticClass();
}
