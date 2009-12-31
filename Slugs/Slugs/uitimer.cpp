#include "uitimer.h"

UITimer::UITimer(const std::string& name, int group, ImageResource* backgroundImage, FontResource* font) : UIWidget(name, group)
{

	background.SetImage(backgroundImage);
	background.Center();

	fontResource = font;

}

void UITimer::Render()
{

	Renderer* renderer = Renderer::Get();

	// Get turn time remaining for the current player
	float time = Max(Game::Get()->GetCurrentPlayer()->GetTurnTimeRemaining(), 0.0f);

	// Render the background
	renderer->Render(background);

	// Render the timer
	sprintf_s(buffer, 16, "%.0f", time);
	renderer->RenderTextShadowed((float)bounds.center.x - 2, (float)bounds.center.y - 3, fontResource, buffer, 32.0f, (time <= 10.0f ? Color::red : Color::white), Color::black, FontFlag_Bold|FontFlag_Centered);

}

void UITimer::SetPosition(const Vec2i& position)
{

	background.SetPosition(ConvertVector(position));

	UIWidget::SetPosition(position);

}