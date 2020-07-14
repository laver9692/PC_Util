// Fill out your copyright notice in the Description page of Project Settings.


#include "ListDrawer.h"
#include <Components/TextBlock.h>

void UListDrawer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// �p��������
	Super::NativeTick(MyGeometry, InDeltaTime);

	// �e�L�X�g�u���b�N�̎擾
	auto tmp = GetWidgetFromName("TestText");
	UTextBlock* _pParam01_Text = Cast<UTextBlock>(tmp);
	// �e�L�X�g��������
	if (_pParam01_Text) {
		// ���l��\��
		float _Value = 3.1415926f;
		FString _Str = FString::Printf(TEXT("%.2f"), _Value);

		_pParam01_Text->SetText(FText::FromString(_Str));

		// �J���[�ݒ�
		_pParam01_Text->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.f, 0.f, 1.f)));
	}
}

void UListDrawer::NativeConstruct() {
	Super::NativeConstruct();
	//UTextBlock tmp = UTextBlock::UTextBlock();
	//tmp.Text = FText::FromString("TestTextTest");
	//tmpWidget = CreateWidget(this, tmp.StaticClass(), "TestTmpText");
}

FReply UListDrawer::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	UE_LOG(LogTemp, Display, TEXT("Called NativeOnKeyDown"));
	const auto key = InKeyEvent.GetKey();
	if (key == EKeys::Enter) {
		CreateTest();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UListDrawer::CreateTest()
{
	if (tmpWidget) {
		tmpWidget->RemoveFromParent();
		tmpWidget = nullptr;
	}
	auto ins = CreateWidget(this, UTextBlock::StaticClass(), "InstancedText");
	FRandomStream rand = FRandomStream::FRandomStream();
	float x = rand.FRandRange(0, 1920);
	float y = rand.FRandRange(0, 1080);
	ins->SetPositionInViewport(FVector2D(x, y));
	Cast<UTextBlock>(ins)->SetText(FText::FromString("It is Test Text"));
	ins->AddToViewport();
	tmpWidget = ins;
}
