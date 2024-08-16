//================== INTRO OF THIS FILE ===========================
//       **Usagi.h** is an interface for the client side.
//       **Usagi.h** defines all the difinitions the clients should probably use.
//       USAGI is now static linked to the client side.
//=================================================================


/*
	<Usagi.h> ---- Application.h ---- Window.h
			  |				     |--- Core.h
			  |				     |--- LayerStack.h
			  |					 |--- Event.h
			  |				     |--- ApplicationEvent.h
			  | 				 |--- ImGuiLayer.h
			  | 				 |--- Shader.h
			  | 				 |--- Buffer.h
			  | 				 |--- VertexArray.h
	          |--- Layer.h
			  |--- Log.h
			  |--- Keycodes.h
			  |--- MouseButtonCodes.h
			  |--- Input.h
			  |--- ImGuiLayer.h

*/ 




#pragma once

//@@ For use by Usagi applications
// --- Renderer ---------------------
#include "Usagi/Application.h"       
#include "Usagi/Layer.h"
#include "Usagi/Log.h"

#include "Usagi/Core/Timestep.h"

#include "Usagi/Keycodes.h"
#include "Usagi/MouseButtonCodes.h"
#include "Usagi/Input.h"

#include "Usagi/ImGui/ImGuiLayer.h"

#include "Usagi/Renderer/Renderer.h"
#include "Usagi/Renderer/RenderCommand.h"

#include "Usagi/Renderer/Buffer.h"
#include "Usagi/Renderer/Shader.h"
#include "Usagi/Renderer/VertexArray.h"

#include "Usagi/Renderer/OrthographicCamera.h"
// -----------------------------------


// ---Entry Point---------------------
#include "Usagi/EntryPoint.h"
// -----------------------------------