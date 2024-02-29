// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"

#include "Kismet/GameplayStatics.h" // MDJ: Needed to do "UGameplayStatics::ProjectWorldToScreen(...)"
#include "Blueprint/WidgetLayoutLibrary.h" // MDJ: Needed to do "float Scale = UWidgetLayoutLibrary::GetViewportScale(this);"
#include "Components/SizeBox.h" // MDJ: needed to do "ParentSizeBox->SetRenderTranslation(ScreenPosition);"


// MDJ: this is the 'tick' equivalent of widgets
void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// MDJ: If AttachedActor is no longer valid, immediately return -- this is explained Lecture 14.1 @22:00
	//		because we set AttachedActor as UPROPERTY the pointer is automatically cleared once UE garbagecollection runs (approx every 60s)
	//		so once that happens our "AttachedActor->GetActorLocation()" crashes the code -- thus do IsValid check below
	if (!IsValid(AttachedActor)) // MDJ: !IsValid is little better than straight != nullptr because it also checks pending kill (CTRL+click to see for yourself)
	{
		RemoveFromParent(); // MDJ: basically the same as remove from viewport

		// MDJ: Adding Logging so that we do not silently remove from parent -- so that if we do not set Actor we can actually see this part of code was reached
		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing Health Widget."));
		return;
	}

	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		// MDJ: Correct for viewport scale
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale; // MDJ: Divide ScreenPosition over viewport scale to get proper screen position adjusted for UMG

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}
