#pragma once

#include "Hazel\Core\Core.h"

#include <string>
#include <vector>

namespace Hazel
{

	class ImGuiConsole
	{
	public:
		class Message
		{
		public:
			enum class Level : int8_t
			{
				Invalid		= -1,
				Trace		= 0,
				Debug		= 1,
				Info		= 2,
				Warn		= 3,
				Error		= 4,
				Critical	= 5,
				Off			= 6			/* Display nothing */
			};

		private:
			struct Color { float r, g, b, a; };

		public:
			Message(const std::string message = "", Level level = Level::Invalid);
			~Message() = default;

			void OnImGuiRender();

			static Level GetLowerLevel(Level level);
			static Level GetHigherLevel(Level level);
			static const char* GetLevelName(Level level);

			static Color GetRenderColor(Level level);
		
		public:
			const Level mLevel;
			const std::string mMessage;
			static std::vector<Level> s_mLevels;
		};

	public:
		~ImGuiConsole() = default;

		static void AddMessage(Ref<Message> message);
		static void Clear();
		static void OnImGuiRender(bool* show);

	protected:
		ImGuiConsole() = default;

	private:
		struct ImGuiRendering
		{
			static void ImGuiRenderHeader();
			static void ImGuiRenderSettings();
			static void ImGuiRenderMessages();
		};

	private:
		static float ms_displayScale;
		static uint16_t ms_messageBufferCapacity;
		static uint16_t ms_messageBufferSize;
		static uint16_t ms_messageBufferBegin;
		static Message::Level ms_messageBufferRenderFilter;
		static std::vector<Ref<Message>> ms_messageBuffer;
		static bool ms_allowScrollingToBottom;
		static bool ms_requestScrollToBottom;
	};

}