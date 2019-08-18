// ©Justin Camden 2019, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HorusFileHelperLibrary.generated.h"

/**
 * Library of functions to assist with file management
 */

UCLASS()
class HORUS_API UHorusFileHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Returns the base directory of the current project by looking at FApp::GetProjectName().
	 * This is usually a subdirectory of the installation
	 * root directory and can be overridden on the command line to allow self
	 * contained mod support.
	 *
	 * @return base directory
	 */
	UFUNCTION(BlueprintCallable, Category = HorusFileHelperLibrary)
	static FString ProjectDir();

	/**
	* Returns the saved directory of the current game by looking at FApp::GetProjectName().
	*
	* @return saved directory
	*/
	UFUNCTION(BlueprintCallable, Category = HorusFileHelperLibrary)
	static FString ProjectSavedDir();

	
	/**
	* Returns the directory the root configuration files are located.
	*
	* @return root config directory
	*/
	UFUNCTION(BlueprintCallable, Category = HorusFileHelperLibrary)
	static FString ProjectConfigDir();

	/**
	* Returns the content directory of the current game by looking at FApp::GetProjectName().
	*
	* @return content directory
	*/
	UFUNCTION(BlueprintCallable, Category = HorusFileHelperLibrary)
	static FString ProjectContentDir();

	/**
	* Returns the root directory for user-specific game files.
	*
	* @return game user directory
	*/
	UFUNCTION(BlueprintCallable, Category = HorusFileHelperLibrary)
	static FString ProjectUserDir();

	/**
	 * Write the FString to a file.
	 * Supports all combination of ANSI/Unicode files and platforms.
	 */
	UFUNCTION(BlueprintCallable, Category = HorusFileHelperLibrary)
	static bool SaveStringToFile(const FString& String, const FString& Filename);

	/*
	* Saves the inputted string array to the save path with the inputted file name.
	* Returns true if successfully saved.
	*/
	UFUNCTION(BlueprintCallable, Category = HorusFileHelperLibrary)
	static bool SaveStringArrayToFile(const TArray<FString>& Lines, const FString& Filename);
	
	/*
	* Hashes the specified file contents and returns the result as a hexadecimal string. 
	* Returns INVALID HASH if not valid.
	*/
	UFUNCTION(BlueprintCallable, Category = HorusFileHelperLibrary)
	static FString HashFileToHexString(const FString& Filename);
};
