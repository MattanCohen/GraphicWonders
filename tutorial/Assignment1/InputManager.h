#pragma once   //maybe should be static class
#include "igl/opengl/glfw/Display.h"
#include "igl/opengl/glfw/renderer.h"
#include "Assignment1.h"
#include "imgui/imgui.h"


	void glfw_mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		if (action == GLFW_PRESS)
		{
			Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
			Assignment1* scn = (Assignment1*)rndr->GetScene();
			double x2, y2;
			
			glfwGetCursorPos(window, &x2, &y2);
			rndr->UpdatePress(x2, y2);
			if (rndr->Picking((int)x2, (int)y2))
			{
				rndr->UpdatePosition(x2, y2);
				if(button == GLFW_MOUSE_BUTTON_LEFT)
					rndr->Pressed();
			}
			else
			{
				rndr->UnPick(2);
			}
		
		}
		else
		{
			Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
			rndr->UnPick(2);
		}

	}
	
	void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Assignment1* scn = (Assignment1*)rndr->GetScene();
		/*
		if (rndr->IsPicked())
		{
			rndr->UpdateZpos((int)yoffset);
			rndr->MouseProccessing(GLFW_MOUSE_BUTTON_MIDDLE);
		}
		else
		{
			rndr->MoveCamera(0, rndr->zTranslate, (float)yoffset);
		}
		*/
		bool append = yoffset < 0;
		scn->WhenRotate(append);
	}
	
	void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Assignment1* scn = (Assignment1*)rndr->GetScene();

		float xold = rndr->getXOld();
		float yold = rndr->getYOld();
		float xrel = xpos - xold;
		float yrel = ypos - yold;

		rndr->UpdatePosition((float)xpos,(float)ypos);

		if (rndr->CheckViewport(xpos,ypos, 0))
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{

				rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				if (xrel > 0)
					scn->WhenTranslate(true, true);
				else
					scn->WhenTranslate(true, false);
				if (yrel > 0)
					scn->WhenTranslate(false, true);
				else
					scn->WhenTranslate(false, false);
				rndr->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && rndr->IsPicked() && rndr->IsMany())
					rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);

		}
		/*
		scn -> AddShader("../shaders/exampleShader");
		Shader s();
		for (int i = 0; i < scn->shaders.size(); i++) {
			scn->shaders.at(i)->SetUniform1f("x", xpos);
			scn->shaders.at(i)->SetUniform1f("y", ypos);
		}
		*/
	}

	void glfw_window_size_callback(GLFWwindow* window, int width, int height)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);

        rndr->resize(window,width,height);
		
	}

	int coeff_to_change = 0;
	
	bool Dpressed = false;
	void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Assignment1* scn = (Assignment1*)rndr->GetScene();
		//rndr->FreeShapes(2);
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			switch (key)
			{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
				
			case GLFW_KEY_SPACE:
				scn->switchColor();
				if (scn->IsActive())
					scn->Deactivate();
				else
					scn->Activate();
				break;

			case GLFW_KEY_UP:
				//rndr->MoveCamera(0, scn->xRotate, 0.05f);
				scn->changeCoeffs(coeff_to_change, true);
				break;
			case GLFW_KEY_DOWN:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
				//rndr->MoveCamera(0, scn->xRotate, -0.05f);
				scn->changeCoeffs(coeff_to_change, false);
				break;
			case GLFW_KEY_LEFT:
				//rndr->MoveCamera(0, scn->yRotate, 0.05f);
				scn->changeIterationNum(false);
				break;
			case GLFW_KEY_RIGHT:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
				//rndr->MoveCamera(0, scn->yRotate, -0.05f);
				scn->changeIterationNum(true);
				break;
			case GLFW_KEY_U:
				//rndr->MoveCamera(0, scn->yTranslate, 0.25f);
				break;
			case GLFW_KEY_D:
				if (Dpressed) {
					scn->resetCoeffs();
					Dpressed = false;
				}
				else
					Dpressed = true;
				//rndr->MoveCamera(0, scn->yTranslate, -0.25f);
				break;
			case GLFW_KEY_L:
				//rndr->MoveCamera(0, scn->xTranslate, -0.25f);
				break;
			
			case GLFW_KEY_R:
				//rndr->MoveCamera(0, scn->xTranslate, 0.25f);
				break;
			
			case GLFW_KEY_B:
				rndr->MoveCamera(0, scn->zTranslate, 0.5f);
				break;
			case GLFW_KEY_F:
				rndr->MoveCamera(0, scn->zTranslate, -0.5f);
				break;
			case GLFW_KEY_1:
				coeff_to_change = 0;
				break;
			case GLFW_KEY_2:
				coeff_to_change = 1;
				break;
			case GLFW_KEY_3:
				coeff_to_change = 2;
				break;
			case GLFW_KEY_4:
				coeff_to_change = 3;
				break;

			default:
				break;

			}
			if (key != GLFW_KEY_D && Dpressed)
				Dpressed = false;
		}
	}


void Init(Display& display, igl::opengl::glfw::imgui::ImGuiMenu *menu)
{
    display.AddKeyCallBack(glfw_key_callback);
    display.AddMouseCallBacks(glfw_mouse_callback, glfw_scroll_callback, glfw_cursor_position_callback);
    display.AddResizeCallBack(glfw_window_size_callback);
    if(menu)
        menu->init(&display);
}
