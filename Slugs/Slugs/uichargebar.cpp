#include "uichargebar.h"
#include "game.h"

UIChargebar::UIChargebar(const std::string& name, int group, ImageResource* backgroundResource, ImageResource* fillResource) : UIWidget(name, group)
{

	background.SetImage(backgroundResource);
	background.Center();
	fill.SetImage(fillResource);
	fill.SetCenter(0, fill.GetHeight() / 2);

}

void UIChargebar::Render()
{

	Renderer* renderer = Renderer::Get();

	Player* currentPlayer = Game::Get()->GetCurrentPlayer();

	if ((currentPlayer) && (currentPlayer->GetCurrentSlug()))
	{

		float charge = Clamp(currentPlayer->GetCurrentSlug()->GetPower(), 0.0f, 1.0f);

		if (charge > 0.0f)
		{
		
			renderer->Render(background);

			fill.SetWidth(background.GetWidth() * charge);
			renderer->Render(fill);

		}

	}

}

void UIChargebar::SetPosition(const Vec2i& position)
{

	Vec2f p = ConvertVector(position);

	background.SetPosition(p);
	fill.SetPosition(Vec2f(p.x - background.GetWidth() / 2, p.y));

	UIWidget::SetPosition(position);

}