#include "hzpch.h"
#include "ImGuiConsole.h"

#include <imgui.h>
#include "examples\imgui_impl_glfw.h"
#include "examples\imgui_impl_opengl3.h"


namespace Hazel {

	float ImGuiConsole::ms_displayScale = 1.0f;
	uint16_t ImGuiConsole::ms_messageBufferCapacity = 200;
	uint16_t ImGuiConsole::ms_messageBufferSize = 0;
	uint16_t ImGuiConsole::ms_messageBufferBegin = 0;
	ImGuiConsole::Message::Level ImGuiConsole::ms_messageBufferRenderFilter = ImGuiConsole::Message::Level::Trace;
	std::vector<Ref<ImGuiConsole::Message>> ImGuiConsole::ms_messageBuffer(ms_messageBufferCapacity);
	bool ImGuiConsole::ms_allowScrollingToBottom = true;
	bool ImGuiConsole::ms_requestScrollToBottom = false;

	void ImGuiConsole::AddMessage(Ref<Message> message)
	{
		if (message->mLevel == Message::Level::Invalid)
			return;

		*(ms_messageBuffer.begin() + ms_messageBufferBegin) = message;
		if (++ms_messageBufferBegin == ms_messageBufferCapacity)
			ms_messageBufferBegin = 0;
		if (ms_messageBufferSize < ms_messageBufferCapacity)
			ms_messageBufferSize++;

		ms_requestScrollToBottom = ms_allowScrollingToBottom;
	}

	void ImGuiConsole::Clear()
	{
		for (auto msg = ms_messageBuffer.begin(); msg != ms_messageBuffer.end(); msg++)
			(*msg) = std::make_shared<Message>();
		ms_messageBufferBegin = 0;
	}

	void ImGuiConsole::OnImGuiRender(bool* show)
	{
		ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
		ImGui::Begin("Console", show);
		{
			ImGuiRendering::ImGuiRenderHeader();
			ImGui::Separator();
			ImGuiRendering::ImGuiRenderMessages();
		}
		ImGui::End();
	}


	void ImGuiConsole::ImGuiRendering::ImGuiRenderHeader()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		const float spacing = style.ItemInnerSpacing.x;

		// Text change level
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Display");

		ImGui::SameLine(0.0f, 2.0f * spacing);
		
		// Buttons to quickly change level left
		if (ImGui::ArrowButton("##MessageRenderFilter_L", ImGuiDir_Left))
		{
			ms_messageBufferRenderFilter = Message::GetLowerLevel(ms_messageBufferRenderFilter);
		}

		ImGui::SameLine(0.0f, spacing);

		ImGui::PushItemWidth(ImGui::CalcTextSize("Critical").x * 1.36f);
		if (ImGui::BeginCombo("##MessageRenderFilter", Message::GetLevelName(ms_messageBufferRenderFilter)))
		{
			for (auto lvl = Message::s_mLevels.begin(); lvl != Message::s_mLevels.end(); lvl++)
			{
				bool selected = (ms_messageBufferRenderFilter == *lvl);
				if (ImGui::Selectable(Message::GetLevelName(*lvl), selected))
					ms_messageBufferRenderFilter = *lvl;
				if (!selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		ImGui::SameLine(0.0f, spacing);
		
		// Buttons for changing level
		if (ImGui::ArrowButton("##MessageRenderFilder_R", ImGuiDir_Right))
			ms_messageBufferRenderFilter = Message::GetHigherLevel(ms_messageBufferRenderFilter);
		
		ImGui::SameLine(0.0f, spacing);

		// Button for settings
		ImGui::SameLine(0.0f, ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Settings").x - style.WindowPadding.x / 2.0f);
		if (ImGui::Button("Settings"))
			ImGui::OpenPopup("SettingsPopup");
		if (ImGui::BeginPopup("SettingsPopup"))
		{
			ImGuiRenderSettings();
			ImGui::EndPopup();
		}
	}

	void ImGuiConsole::ImGuiRendering::ImGuiRenderSettings()
	{
		const float maxWidth = ImGui::CalcTextSize("Scroll to bottom").x * 1.1f;
		const float spacing = ImGui::GetStyle().ItemInnerSpacing.x + ImGui::CalcTextSize(" ").x;

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Scroll to bottom");
		ImGui::SameLine(0.0f, spacing + maxWidth - ImGui::CalcTextSize("Scroll to bottom").x);
		ImGui::Checkbox("##ScrollToBottom", &ms_allowScrollingToBottom);

		ImGui::SameLine(0.0f, spacing);

		// Button for clearing console
		ImGui::SameLine(0.0f, ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Clear console").x + 1.0f);
		if (ImGui::Button("Clear console"))
			ImGuiConsole::Clear();

		// Slider for font scale
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Display scale");
		ImGui::SameLine(0.0f, spacing + maxWidth - ImGui::CalcTextSize("Display scale").x);
		ImGui::PushItemWidth(maxWidth * 1.25f / 1.1f);
		ImGui::SliderFloat("##DisplayScale", &ms_displayScale, 0.5f, 4.0f, "%.1f");
		ImGui::PopItemWidth();
	}

	void ImGuiConsole::ImGuiRendering::ImGuiRenderMessages()
	{
		ImGui::BeginChild("ScrollRegion", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
		{
			ImGui::SetWindowFontScale(ms_displayScale);

			auto messageStart = ms_messageBuffer.begin() + ms_messageBufferBegin;
			if (*messageStart)
				for (auto msg = messageStart; msg != ms_messageBuffer.end(); msg++)
					(*msg)->OnImGuiRender();

			if (ms_messageBufferBegin != 0)
				for (auto msg = ms_messageBuffer.begin(); msg != messageStart; msg++)
					(*msg)->OnImGuiRender();

			if (ms_requestScrollToBottom && ImGui::GetScrollMaxY() > 0)
			{
				ImGui::SetScrollY(ImGui::GetScrollMaxY());
				ms_requestScrollToBottom = false;
			}
		}
		ImGui::EndChild();
	}

	std::vector<ImGuiConsole::Message::Level> ImGuiConsole::Message::s_mLevels{
		ImGuiConsole::Message::Level::Trace,
		ImGuiConsole::Message::Level::Info,
		ImGuiConsole::Message::Level::Warn,
		ImGuiConsole::Message::Level::Error,
		ImGuiConsole::Message::Level::Critical,
		ImGuiConsole::Message::Level::Off
	};

	ImGuiConsole::Message::Message(const std::string message, Level level)
		: mMessage(message), mLevel(level)
	{
	}

	void ImGuiConsole::Message::OnImGuiRender()
	{
		if (mLevel != Level::Invalid && mLevel >= ImGuiConsole::ms_messageBufferRenderFilter)
		{
			Color color = GetRenderColor(mLevel);
			ImGui::PushStyleColor(ImGuiCol_Text, { color.r, color.g, color.b, color.a });
			ImGui::TextUnformatted(mMessage.data());
			ImGui::PopStyleColor();
		}
	}

	ImGuiConsole::Message::Level ImGuiConsole::Message::GetLowerLevel(Level level)
	{
		switch (level)
		{
		case ImGuiConsole::Message::Level::Off: return ImGuiConsole::Message::Level::Critical;
		case ImGuiConsole::Message::Level::Critical: return ImGuiConsole::Message::Level::Error;
		case ImGuiConsole::Message::Level::Error: return ImGuiConsole::Message::Level::Warn;
		case ImGuiConsole::Message::Level::Warn: // return ImGuiConsole::Message::Level::Debug;
		case ImGuiConsole::Message::Level::Debug: return ImGuiConsole::Message::Level::Info;
		case ImGuiConsole::Message::Level::Info:
		case ImGuiConsole::Message::Level::Trace: return ImGuiConsole::Message::Level::Trace;
		}
		return ImGuiConsole::Message::Level::Invalid;
	}

	ImGuiConsole::Message::Level ImGuiConsole::Message::GetHigherLevel(Level level)
	{
		switch (level)
		{
		case ImGuiConsole::Message::Level::Trace: return ImGuiConsole::Message::Level::Info;
		case ImGuiConsole::Message::Level::Info: //return ImGuiConsole::Message::Level::Debug;
		case ImGuiConsole::Message::Level::Debug: return ImGuiConsole::Message::Level::Warn;
		case ImGuiConsole::Message::Level::Warn: return ImGuiConsole::Message::Level::Error;
		case ImGuiConsole::Message::Level::Error: return ImGuiConsole::Message::Level::Critical;
		case ImGuiConsole::Message::Level::Critical:
		case ImGuiConsole::Message::Level::Off: return ImGuiConsole::Message::Level::Off;
		}
		return ImGuiConsole::Message::Level::Invalid;
	}

	const char* ImGuiConsole::Message::GetLevelName(Level level)
	{
		switch (level)
		{
		case ImGuiConsole::Message::Level::Trace: return "Trace";
		case ImGuiConsole::Message::Level::Info: return "Info";
		case ImGuiConsole::Message::Level::Debug: return "Debug";
		case ImGuiConsole::Message::Level::Warn: return "Warning";
		case ImGuiConsole::Message::Level::Error: return "Error";
		case ImGuiConsole::Message::Level::Critical: return "Critical";
		case ImGuiConsole::Message::Level::Off: return "None";
		}
		return "Unknown name";
	}

	ImGuiConsole::Message::Color ImGuiConsole::Message::GetRenderColor(Level level)
	{
		switch (level)
		{
		case ImGuiConsole::Message::Level::Trace: return { 0.75f, 0.75f, 0.75f, 1.00f }; // White-ish gray
		case ImGuiConsole::Message::Level::Info: return { 0.00f, 0.50f, 0.00f, 1.00f }; // Green
		case ImGuiConsole::Message::Level::Debug: return { 0.00f, 0.50f, 0.50f, 1.00f }; // Cyan
		case ImGuiConsole::Message::Level::Warn: return { 1.00f, 1.00f, 0.00f, 1.00f }; // Yellow
		case ImGuiConsole::Message::Level::Error: return { 1.00f, 0.00f, 0.00f, 1.00f }; // Red
		case ImGuiConsole::Message::Level::Critical: return { 1.00f, 1.00f, 1.00f, 1.00f }; // White-white
		}
		return { 1.00f, 1.00f, 1.00f, 1.00f };
	}

}

