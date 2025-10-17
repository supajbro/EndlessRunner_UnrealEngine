#include "DistanceWidget.h"
#include "Components/TextBlock.h"

void UDistanceWidget::UpdateDistanceDisplay(float Distance)
{
    if (DistanceText)
    {
        DistanceText->SetText(FText::AsNumber(FMath::FloorToInt(Distance)));
    }
}
