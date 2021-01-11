#include "BurnerInserter.h"

HRESULT BurnerInserter::Init(int x, int y, DIRECTION direction, bool temp)
{
	itemId = ItemEnum::BURNER_INSERTER;
	Structure::Init(x, y, direction, temp);

	platformImage = new GLImage();
	platformImage->Init("Entity/BurnerInserter-platform", 4, 1);

	handBaseImage = new GLImage();
	handBaseImage->Init("Entity/BurnerInserter-handBase");
	handBaseScale = { 0.4f, 0.4f };
	handBaseImage->SetScale(handBaseScale);
	handBaseImage->SetOffset(glm::vec2(0.0f, 60.0f));
	handBaseAngle = 0.0f;

	handOpenImage = new GLImage();
	handOpenImage->Init("Entity/BurnerInserter-handOpen");
	handOpenImage->SetScale(glm::vec2(0.4f, 0.4f));
	handOpenImage->SetOffset({ 0, 82 });

	return S_OK;
}

void BurnerInserter::Release()
{
	SAFE_RELEASE(handOpenImage);
	SAFE_RELEASE(handBaseImage);
	SAFE_RELEASE(platformImage);
}

void BurnerInserter::Update()
{
	Structure::Update();

	float destAngle;

	// 핸드베이스 업데이트
	handBaseImage->SetAngle(glm::radians(handBaseAngle));
	float scaleY = ((cos(glm::radians(handBaseAngle)) + 1.0f) / 4.0f + 0.4f);
	handBaseScale.y = 0.4f * scaleY;
	handBaseImage->SetScale(handBaseScale);

	
	float destAngles[4] = { 0.0f, -60.0f, -180.0f, -300.0f };
	destAngle = destAngles[direction];

	if (destAngle == 0.0f && handBaseAngle < 0)
		handBaseAngle += 360.0f;
	handBaseAngle = Lerp(handBaseAngle, destAngle, 0.05f);

	// 핸드 오픈 업데이트
	float handOpenDestAngles[4] = { 0.0f, -120.0f, -180.0f, -240.0f };
	destAngle = handOpenDestAngles[direction];

	float handOpenScaleY = ((cos(glm::radians(handBaseAngle + 180)) + 1.0f) / 4.0f + 0.4f);
	if (destAngle == 0.0f && handOpenAngle < 0)
		handOpenAngle += 360.0f;
	handOpenAngle = Lerp(handOpenAngle, destAngle, 0.05f);

	handOpenImage->SetScale(glm::vec2(0.4f, 0.4f * handOpenScaleY));
	handOpenImage->SetAngle(glm::radians(handOpenAngle));
}

void BurnerInserter::FirstRender(Shader* lpShader)
{
}

void BurnerInserter::Render(Shader* lpShader)
{
	platformImage->Render(lpShader, position.x, position.y, OPPOSITE_DIR(direction), 0);
	
}

void BurnerInserter::LateRender(Shader* lpShader)
{
	handOpenImage->Render(lpShader, 
		position.x - sin(glm::radians(handBaseAngle)) * 120 * handBaseScale.y ,
		position.y + cos(glm::radians(handBaseAngle)) * 120 * handBaseScale.y + 10);
	handBaseImage->Render(lpShader, position.x, position.y + 10);
}
