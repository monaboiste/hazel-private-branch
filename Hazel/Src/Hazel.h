#pragma once

// For use by Hazel applications

#include "Hazel\Core\Application.h"
#include "Hazel\Core\Log.h"
#include "Hazel\Core\Timestep.h"
#include "Hazel\Core\Layer.h"
#include "Hazel\ImGui\ImGuiLayer.h"

#include "Hazel\Core\Input.h"
#include "Hazel\Core\KeyCodes.h"
#include "Hazel\Core\MouseButtonCodes.h"

// ---Renderer------------------------
#include "Hazel\Renderer\Renderer2D.h"
#include "Hazel\Renderer\Renderer.h"
#include "Hazel\Renderer\RenderCommand.h"

#include "Hazel\Renderer\Buffer.h"
#include "Hazel\Renderer\FrameBuffer.h"
#include "Hazel\Renderer\Shader.h"
#include "Hazel\Renderer\Texture.h"
#include "Hazel\Renderer\SubTexture2D.h"
#include "Hazel\Renderer\VertexArray.h"

#include "Hazel\Renderer\OrthographicCameraController.h"
#include "Hazel\Renderer\OrthographicCamera.h"
#include "Hazel\Renderer\EditorCamera.h"

// ---Scene--------------------------
#include "Hazel\Scene\Scene.h"
#include "Hazel\Scene\SceneSerializer.h"
#include "Hazel\Scene\Component.h"
#include "Hazel\Scene\Entity.h"
#include "Hazel\Scene\ScriptableEntity.h"

// Utils
#include "Hazel\Utils\PlatformUtils.h"

// ---ImGui Console and Debug---------
#include "Hazel\ImGui\ImGuiConsole.h"
#include "Hazel\Debug\Instrumentor.h"