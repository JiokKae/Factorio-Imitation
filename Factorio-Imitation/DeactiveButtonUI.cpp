#include "DeactiveButtonUI.h"
#include "GLImage.h"

HRESULT DeactiveButtonUI::Init()
{
	image = new GLImage();
	image->Init("UI/DeactiveButtonUI", 3, 1);

	onMouse = false;
	return S_OK;
}

void DeactiveButtonUI::Release()
{
}

void DeactiveButtonUI::Update()
{
	if (active)
	{
		if (PtInFRect(GetFrect(), { g_ptMouse.x, g_ptMouse.y }))
		{
			onMouse = true;
			
			if (isMouseDown)
			{
				if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
				{
					SoundManager::GetSingleton()->Play("GUI-ToolButton", 0.6f);
					UIManager::GetSingleton()->DeactiveUI();
				}
			}

			if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
			{
				isMouseDown = true;
			}
			else
				isMouseDown = false;


		}
		else
		{
			onMouse = false;
			isMouseDown = false;
		}
	}
}

void DeactiveButtonUI::Render(Shader* lpShader)
{
	if (active)
	{
		image->Render(lpShader, GetPosition().x, GetPosition().y, onMouse + isMouseDown, 0);
	}
}
