#include "DistanceWidget.h"
#include "Components/TextBlock.h"

void UDistanceWidget::UpdateDistanceDisplay(float Distance)
{
    if (DistanceText)
    {
        FFormatOrderedArguments Args;
        Args.Add(FText::AsNumber(FMath::FloorToInt(Distance)));

        FText FormattedText = FText::Format(NSLOCTEXT("YourNamespace", "DistanceTextFormat", "{0} Meters"), Args);

        DistanceText->SetText(FormattedText);
    }
}
