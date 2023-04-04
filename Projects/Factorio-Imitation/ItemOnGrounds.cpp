#include "ItemOnGrounds.h"
#include "ItemOnGround.h"
#include "../GLFramework/VertexDataObject/VertexArrayObject.h"
#include "../GLFramework/VertexDataObject/VertexBufferObject.h"
#include "../GLFramework/Texture/Texture.h"
#include "Character.h"
#include "Inventory.h"

ItemOnGrounds::ItemOnGrounds()
	: allItemsImage{ new GLImage("Icons/AllItems", 8, 8, 0.25f, 0.25f) }
	, instancingShader{ new ShaderProgram("shader/InstancingVertexShader.glsl", "shader/StandardFragmentShader.glsl") }
	, itemsVAO{ new VAO() }
{
	instancingShader->use();
	
	// VertexShader uniform var
	instancingShader->setVec2("maxFrame", allItemsImage->GetMaxFrame());
	instancingShader->setMat4("model", glm::mat4());
	instancingShader->setVec2("offset", glm::vec2(0.0f, 0.0f));
	instancingShader->setVec2("margin", allItemsImage->GetMargin());
	instancingShader->setFloat("vertexScale", 1.0f);

	// FragmentShader uniform var
	instancingShader->setInt("material.diffuse", 0);
	instancingShader->setVec3("material.diffuseColor", glm::vec3(1.0f, 1.0f, 1.0f));
	instancingShader->setFloat("alpha", 1.0f);

	// 아래의 VBO들을 담는 Vertex Array Object를 생성한다.
	// tileQuadVBO Init (사각형 버텍스들을 저장하는 VBO)
	VBO* tileQuadVBO = new VBO();   // 사각형 버텍스들을 저장하는 VBO
	float vertices[] = {
		// positions       // texture coords
		-32 / 2, -32 / 2,  0.0f, 0.0f,
		 32 / 2, -32 / 2,  1.0f, 0.0f,
		 32 / 2,  32 / 2,  1.0f, 1.0f,
		 32 / 2,  32 / 2,  1.0f, 1.0f,
		-32 / 2,  32 / 2,  0.0f, 1.0f,
		-32 / 2, -32 / 2,  0.0f, 0.0f,
	};
	tileQuadVBO->SetData(sizeof(float) * 4 * 6, vertices, GL_STATIC_DRAW); // vec4(vec2 pos, vec2 texCoord) * 6
	itemsVAO->AddVBO(0, tileQuadVBO, 4);

	// currFrameVBO Init (각 인스턴스의 현재 프레임을 저장하는 VBO)
	VBO* currFrameVBO = new VBO();
	currFrameVBO->SetData(sizeof(glm::vec2), NULL, GL_DYNAMIC_DRAW); // vec2(currFrame.x, currFrame.y) * 1024
	itemsVAO->AddVBO(1, currFrameVBO, 2);
	itemsVAO->SetDivisor(1, 1);

	// offsetVBO Init (각 인스턴스의 위치 오프셋을 저장하는 VBO)
	VBO* offsetVBO = new VBO();
	offsetVBO->SetData(sizeof(glm::vec2), NULL, GL_DYNAMIC_DRAW); // vec2(currFrame.x, currFrame.y) * 1024
	itemsVAO->AddVBO(2, offsetVBO, 2);
	itemsVAO->SetDivisor(2, 1);
}

ItemOnGrounds::~ItemOnGrounds()
{
	SAFE_DELETE(allItemsImage);
	SAFE_DELETE(instancingShader);
	SAFE_DELETE(itemsVAO);
}

void ItemOnGrounds::Update(const FRECT& cameraFRect)
{
	if (items.empty())
		return;

	PickUp();

	itemsInScreen.clear();
	std::copy_if(items.begin(), items.end(), std::back_inserter(itemsInScreen), [&cameraFRect](ItemOnGround* item) {
		item->Update();	
		return PtInFRect(cameraFRect, item->GetPosition());
	});

	sort(itemsInScreen.begin(), itemsInScreen.end(), [](ItemOnGround* a, ItemOnGround* b) { return a->GetPosition() > b->GetPosition(); });
}

void ItemOnGrounds::Render()
{
	if (itemsInScreen.empty())
		return;

	instancingShader->use();

	glBindVertexArray(itemsVAO->GetID());

	std::vector<glm::vec2> infoData(itemsInScreen.size());
	for (int i = 0; i < infoData.size(); i++)
		infoData[i] = glm::vec2(itemsInScreen[i]->GetItemId() % 8, 7 - itemsInScreen[i]->GetItemId() / 8);
	itemsVAO->SetVBOData(1, sizeof(glm::vec2) * infoData.size(), &infoData[0], GL_DYNAMIC_DRAW);

	for (int i = 0; i < infoData.size(); i++)
		infoData[i] = itemsInScreen[i]->GetPosition();
	itemsVAO->SetVBOData(2, sizeof(glm::vec2) * infoData.size(), &infoData[0], GL_DYNAMIC_DRAW);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, allItemsImage->GetLpSourceTexture()->GetID());
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(infoData.size()));

	glBindVertexArray(0);
}

void ItemOnGrounds::AddItem(int itemId, float positionX, float positionY)
{
	items.push_back(new ItemOnGround(static_cast<ItemEnum>(itemId), positionX, positionY));
}

void ItemOnGrounds::DeleteItem(ItemOnGround* item)
{
	std::erase(items, item);
	SAFE_DELETE(item);
}

void ItemOnGrounds::PickUp()
{
	if (KeyManager::GetSingleton()->IsStayKeyDown('F') == false)
		return;

	Character* player{ EntityManager::GetSingleton()->GetLpPlayer() };
	bool pickedUp{ false };
	for (auto& item : itemsInScreen)
	{
		if (CheckRectCollision(player->GetPickUpFRect(), item->GetCollisionFRect()) == false)
			continue;

		pickedUp = true;
		player->GetLpInventory()->AddItem(ItemInfo(item->GetItemId(), 1));
		DeleteItem(item);
	}

	if (pickedUp)
		SoundManager::GetSingleton()->Play("PickedUpItem", 0.6f);
}
