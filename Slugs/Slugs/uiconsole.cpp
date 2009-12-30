#include "uiconsole.h"
#include "uimanager.h"

/*
	class UIConsole
*/

const float UIConsole::OPENCLOSE_TIMER		= 1.0f;				
const float UIConsole::OPEN_SIZE			= 200.0f;					

void UIConsole::SetOpen(bool open)
{

	if (open)
		state = UIConsoleState_Opening;
	else
		state = UIConsoleState_Closing;

	// Handle the cases where the state changes while the console is already opening or closing
	if (timer < 0.0f)
		timer = OPENCLOSE_TIMER;
	else
		timer = OPENCLOSE_TIMER - timer;

}

UIConsole::UIConsole(const std::string& widgetName, int maxOutput, int historySize) : UIWidget(widgetName, UIGroup_System, true, true)
{

	const Color BACKGROUND_COLOR = Color(0, 0, 0, 128);

	this->maxLines = maxOutput;
	maxHistory = historySize;

	text = "";
	historyIt = history.begin();

	state = UIConsoleState_Closed;

	background.CreateBox(Vec2f(0.0f, 0.0f), Vec2f((float)Renderer::Get()->GetWidth(), OPEN_SIZE), BACKGROUND_COLOR);

	timer = 0.0f;
	border = 5;

	bottom = 0.0f;

}

void UIConsole::Register(const std::string& name, ConsoleCommand* command)
{

	ASSERT(text != "");
	ASSERT(command != NULL);
	ASSERT(Find(name) == NULL);

	// Register the new command
	commands.push_back(std::pair<std::string, ConsoleCommand*>(name, command));

}

ConsoleCommand* UIConsole::Find(const std::string& name)
{

	for (unsigned int i = 0; i < commands.size(); ++ i)
	{

		if (commands[i].first == name)
			return commands[i].second;

	}

	return NULL;

}

void UIConsole::Execute()
{

	if (text != "")
	{

		Execute(text);

		AddToHistory(text);
		text = "";

	}

}

void UIConsole::Execute(const std::string& command, bool silent, bool addToHistory)
{

	if (command != "")
	{

		std::string name;
		std::vector<std::string> args = SplitCommand(name, command, ' ');

		ConsoleCommand* c = Find(name);

		if (c != NULL)
		{

			bool result = c->Call(args);

			if (!result)
				Print(Localizer::Get()->Localize("console_invalidargs").c_str(), name.c_str());

		}
		else
			Print(Localizer::Get()->Localize("console_badcommand").c_str(), name.c_str());

		if (addToHistory)
			AddToHistory(command);

		historyIt = history.begin();

	}

}

void UIConsole::Clear()
{

	text = "";
	output.clear();

}

void UIConsole::ClearHistory()
{

	history.clear();

}

void UIConsole::Help()
{

#ifdef _DEBUG

	for (unsigned int i = 0; i < commands.size(); ++ i)
		Print("[%s] %s", commands[i].first, commands[i].second->GetDescription());

#endif

}

void UIConsole::Print(const std::string& text)
{

	output.push_back(text);

}

void UIConsole::Print(const char* format, ...)
{

	if (output.size() == maxLines)
		output.pop_back();

	va_list args;
	va_start(args, format);

	vsprintf_s(buffer, MAX_COMMAND_LENGTH, format, args);

	output.push_front(buffer);

	va_end(args);

}

void UIConsole::AddToHistory(const std::string& command)
{

	ASSERT(command != "");

	if (history.size() == maxHistory)
		history.pop_back();

	history.push_front(command);

}

void UIConsole::Update(float elapsedTime)
{

	if (state == UIConsoleState_Opening)
	{

		timer -= elapsedTime;

		if (timer <= 0.0f)
		{

			timer = 0.0f;
			state = UIConsoleState_Open;

		}

		// Hook input
		owner->SetInputHook(this);

		bottom = OPEN_SIZE * (OPENCLOSE_TIMER - timer);
		SetPosition(Vec2i(0, -(int)OPEN_SIZE + RoundDownToInt(bottom)));

	}
	else if (state == UIConsoleState_Closing)
	{

		timer -= elapsedTime;

		if (timer <= 0.0f)
		{

			timer = 0.0f;
			state = UIConsoleState_Closed;

			// Release input hook
			owner->SetInputHook(NULL);

		}

		bottom = OPEN_SIZE * timer;
		SetPosition(Vec2i(0, -(int)OPEN_SIZE + RoundDownToInt(bottom)));

	}

}

void UIConsole::Render()
{

	if (state != UIConsoleState_Closed)
	{

		Renderer* renderer = Renderer::Get();

		// Render background
		renderer->Render(background);

		const float fontSize = 12.0f;
		const float lineSpacing = 4.0f;
		float lineY = bottom - fontSize - border;

		float step = lineSpacing + fontSize;

		// Render the current command line
		renderer->RenderText(border, lineY, NULL, text, fontSize, Color::white, FontFlag_Bold);
		
		std::list<std::string>::iterator i = output.begin();

		while (i != output.end())
		{

			lineY -= step;

			if (lineY < -step)
				break;

			renderer->RenderText(border, lineY, NULL, *i, fontSize, Color::gray);

			i ++;

		}

		lineY -= step;

	}

}

void UIConsole::SetPosition(const Vec2i& position)
{

	background.SetPosition(ConvertVector(position));

	UIWidget::SetPosition(position);

}

void UIConsole::Toggle()
{

	switch (state)
	{

	case UIConsoleState_Open:
	case UIConsoleState_Opening:

		SetOpen(false);

		break;

	case UIConsoleState_Closed:
	case UIConsoleState_Closing:

		SetOpen(true);

		break;

	}

}

bool UIConsole::KeyUp(sf::Key::Code key, bool shift, bool control, bool alt)
{

	if ((key >= 97) && (key <= 122))
	{

		// Letters
		if (shift)
			text += (char)(key - 32);
		else
			text += (char)key;

	}
	else if ((key >= 48) && (key <= 57))
	{

		// Numbers
		text += (char)key;

	}
	else if (key == sf::Key::Space)
	{
		
		// Space
		text += ' ';

	}
	else if (key == sf::Key::Back)
	{

		// Backspace
		text = text.substr(0, text.size() - 1);

	}
	else if (key == sf::Key::Return)
	{

		// Return
		Execute();

	}

	return true;

}

bool UIConsole::IsOpen()
{

	return ((state == UIConsoleState_Open) || (state == UIConsoleState_Opening));

}