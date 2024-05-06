#include "PlayerController.h"

#include <iostream>
#include <stdexcept>

const int I_SUCCESS = 0;
const int I_USERINPUT_INIT_FAILED = -5;

std::shared_ptr<CWindow> pCurrWindow = nullptr;
std::shared_ptr<CCamera> pCamera = nullptr;
std::shared_ptr<CGameObject> pControlledGameObject = nullptr;

// Key Input
std::function<void(void)> pExitInput{ []() {} };
std::function<void(void)> pForwardInput{ []() {} };
std::function<void(void)> pBackwardInput{ []() {} };
std::function<void(void)> pRightInput{ []() {} };
std::function<void(void)> pLeftInput{ []() {} };
std::function<void(void)> pUpInput{ []() {} };
std::function<void(void)> pDownInput{ []() {} };
std::function<void(void)> pMouseMode{ []() {} };
std::function<void(void)> pSceneSwitch{ []() {} };

// Mouse Input
float fPitch{ 0.0f };
float fYaw{ -90.0f };
float fLastPosX{ 400.0f };
float fLastPosY{ 400.0f };
float fOffsetX{ 0.0f };
float fOffsetY{ 0.0f };
const float F_SENSE{ 0.1f };

void MouseInput(GLFWwindow* a_pWindow, double a_dXPos, double a_dYPos)
{
	fOffsetX = static_cast<float>(a_dXPos) - fLastPosX;
	fOffsetY = fLastPosY - static_cast<float>(a_dYPos); // reversed since y-coordinates range from bottom to top
	fLastPosX = static_cast<float>(a_dXPos);
	fLastPosY = static_cast<float>(a_dYPos);

	fOffsetX *= F_SENSE;
	fOffsetY *= F_SENSE;

	fYaw += fOffsetX;
	fPitch += fOffsetY;

	if (fPitch > 89.0f)
		fPitch = 89.0f;
	if (fPitch < -89.0f)
		fPitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	direction.y = sin(glm::radians(fPitch));
	direction.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	pCamera->CalcOrientation(direction);
}

void ScrollCallback(GLFWwindow* window, double a_dXOffset, double a_dYOffset) {
	// Adjust the float value based on the scroll input
	pCamera->AddSpeed(static_cast<float>(a_dYOffset * 0.5f));
}

int CPlayerController::Initialize(const std::shared_ptr<CWindow>& a_pWindow,
	const std::shared_ptr<CGameObject>& a_pGameObject, const float& a_fDeltaTime)
{
	m_fDeltaTime = a_fDeltaTime;
	pControlledGameObject = a_pGameObject;
	pCamera = pControlledGameObject->GetComponent<CCamera>();
	if (pCamera == nullptr)
		throw std::runtime_error("No Camera Component found!");
	pCurrWindow = a_pWindow;

	SetDefaultInputGO();

	glfwSetCursorPosCallback(pCurrWindow->GetWindow(), MouseInput);
	glfwSetScrollCallback(pCurrWindow->GetWindow(), ScrollCallback);
	return I_SUCCESS;
}


int CPlayerController::Update(const double& a_dDeltaTime)
{
	m_fDeltaTime = static_cast<float>(a_dDeltaTime);
	CheckKeys();
	return I_SUCCESS;
}

void CPlayerController::Finalize(void)
{
}

void CPlayerController::SetExitInput(const std::function<void(void)>& a_pExitInput)
{
	pExitInput = a_pExitInput;
}

void CPlayerController::SetForwardInput(const std::function<void(void)>& a_pForwardInput)
{
	pForwardInput = a_pForwardInput;
}

void CPlayerController::SetBackwardInput(const std::function<void(void)>& a_pBackwardInput)
{
	pBackwardInput = a_pBackwardInput;
}

void CPlayerController::SetRightInput(const std::function<void(void)>& a_pRightInput)
{
	pRightInput = a_pRightInput;
}

void CPlayerController::SetLeftInput(const std::function<void(void)>& a_pLeftInput)
{
	pLeftInput = a_pLeftInput;
}

void CPlayerController::SetUpInput(const std::function<void(void)>& a_pUpInput)
{
	pUpInput = a_pUpInput;
}

void CPlayerController::SetDownInput(const std::function<void(void)>& a_pDownInput)
{
	pDownInput = a_pDownInput;
}

void CPlayerController::SetMouseMode(const std::function<void(void)>& a_pMouseMode)
{
	pMouseMode = a_pMouseMode;
}

void CPlayerController::SetSceneSwitch(const std::function<void()>& a_pSceneSwitch)
{
	pSceneSwitch = a_pSceneSwitch;
}

void CPlayerController::SetDefaultInputGO()
{
	pExitInput = ([]() { pCurrWindow->SetWindowShouldClose(true); });
	pForwardInput = ([this]() { pControlledGameObject->AddPosition(glm::vec3(pCamera->GetSpeed() * m_fDeltaTime * pCamera->GetOrientation())); });
	pBackwardInput = ([this]() { pControlledGameObject->AddPosition(glm::vec3(pCamera->GetSpeed() * m_fDeltaTime * -pCamera->GetOrientation())); });
	pRightInput = ([this]() { pControlledGameObject->AddPosition(glm::vec3(pCamera->GetSpeed() * m_fDeltaTime * glm::normalize(glm::cross(pCamera->GetOrientation(), pCamera->GetUp())))); });
	pLeftInput = ([this]() { pControlledGameObject->AddPosition(glm::vec3(pCamera->GetSpeed() * m_fDeltaTime * -glm::normalize(glm::cross(pCamera->GetOrientation(), pCamera->GetUp())))); });
	pUpInput = ([this]() { pControlledGameObject->AddPosition(glm::vec3(pCamera->GetSpeed() * m_fDeltaTime * pCamera->GetUp())); });
	pDownInput = ([this]() { pControlledGameObject->AddPosition(glm::vec3(pCamera->GetSpeed() * m_fDeltaTime * -pCamera->GetUp())); });

	pMouseMode = ([this]()
	{
		m_bMouseOn = !m_bMouseOn;
		if (m_bMouseOn)
			glfwSetInputMode(pCurrWindow->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(pCurrWindow->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	});
}

void CPlayerController::CheckKeys(void)
{
	if (glfwGetKey(pCurrWindow->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		pExitInput();
	}
	if (glfwGetKey(pCurrWindow->GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		pMouseMode();
	}
	if (glfwGetKey(pCurrWindow->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		pForwardInput();
	}
	if (glfwGetKey(pCurrWindow->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		pBackwardInput();
	}
	if (glfwGetKey(pCurrWindow->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		pRightInput();
	}
	if (glfwGetKey(pCurrWindow->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		pLeftInput();
	}
	if (glfwGetKey(pCurrWindow->GetWindow(), GLFW_KEY_E) == GLFW_PRESS)
	{
		pUpInput();
	}
	if (glfwGetKey(pCurrWindow->GetWindow(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		pDownInput();
	}
	if (glfwGetKey(pCurrWindow->GetWindow(), GLFW_KEY_P) == GLFW_PRESS)
	{
		pSceneSwitch();
	}
}
