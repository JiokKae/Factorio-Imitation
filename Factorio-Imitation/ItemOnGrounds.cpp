#include "ItemOnGrounds.h"
#include "ItemOnGround.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Texture.h"
#include "Character.h"
#include "Inventory.h"

HRESULT ItemOnGrounds::Init()
{
    allItemsImage = new GLImage();
    allItemsImage->Init("Icons/AllItems", 8, 8, 0.25f, 0.25f);

    instancingShader = new Shader("InstancingVertexShader.glsl", "StandardFragmentShader.glsl");
    instancingShader->use();

    // VertexShader uniform var
    instancingShader->setVec2("maxFrame", allItemsImage->GetMaxFrame());
    instancingShader->setMat4("model", glm::mat4());
    instancingShader->setVec2("offset", glm::vec2(0.0f, 0.0f));
    instancingShader->setFloat("vertexScale", 1.0f);

    // FragmentShader uniform var
    instancingShader->setInt("material.diffuse", 0);
    instancingShader->setVec3("material.diffuseColor", glm::vec3(1.0f, 1.0f, 1.0f));
    instancingShader->setFloat("alpha", 1.0f);

    // 아래의 VBO들을 담는 Vertex Array Object를 생성한다.
    itemsVAO = new VAO();

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

	return S_OK;
}

void ItemOnGrounds::Release()
{
    SAFE_DELETE(itemsVAO);
    SAFE_DELETE(instancingShader);
    SAFE_RELEASE(allItemsImage);
}

void ItemOnGrounds::Update(FRECT cameraFRect)
{
    // 아이템 줍기 처리
    if (KeyManager::GetSingleton()->IsStayKeyDown('F'))
    {
        Character* player = EntityManager::GetSingleton()->GetLpPlayer();
        FRECT playerRect = player->GetPickUpFRect();
        bool pickedUp = false;
        for (it = vecItems.begin(); it != vecItems.end();)
        {
            if (CheckRectCollision(playerRect, (*it)->GetCollisionFRect()))
            {
                pickedUp = true;
                player->GetLpInventory()->AddItem(new ItemInfo((*it)->GetItemEnum(), 1));
                (*it)->Release();
                delete (*it);
                it = vecItems.erase(it);
                if (it == vecItems.end())
                    break;
            }
            else
                ++it;
        }
        if (pickedUp)
            SoundManager::GetSingleton()->Play("PickedUpItem", 0.6f);
    }

    vecItemsInScreen.clear();
    for (it = vecItems.begin(); it != vecItems.end(); ++it)
    {
        (*it)->Update();
        if (PtInFRect(cameraFRect, (*it)->GetPosition()))
        {
            vecItemsInScreen.push_back(*it);
        }
    }
    sort(vecItems.begin(), vecItems.end(), [](ItemOnGround* a, ItemOnGround* b) { return a->GetPosition() > b->GetPosition(); });
}

void ItemOnGrounds::Render()
{
    int renderItemSize = vecItemsInScreen.size();
    glm::vec2* infoDatas = new glm::vec2[renderItemSize]();
    
    instancingShader->use();
    instancingShader->setVec2("margin", allItemsImage->GetMargin());

    glBindVertexArray(itemsVAO->GetID());

    for (int i = 0; i < renderItemSize; i++)
        infoDatas[i] = glm::vec2(vecItemsInScreen[i]->GetItemEnum() % 8, 7 - vecItemsInScreen[i]->GetItemEnum() / 8);
    itemsVAO->SetVBOData(1, sizeof(glm::vec2) * renderItemSize, &infoDatas[0], GL_DYNAMIC_DRAW);

    for (int i = 0; i < renderItemSize; i++)
        infoDatas[i] = vecItemsInScreen[i]->GetPosition();
    itemsVAO->SetVBOData(2, sizeof(glm::vec2) * renderItemSize, &infoDatas[0], GL_DYNAMIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, allItemsImage->GetLpSourceTexture()->GetID());
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, renderItemSize);

    delete[] infoDatas;
    glBindVertexArray(0);
}

void ItemOnGrounds::AddItem(ItemOnGround* item)
{
    vecItems.push_back(item);
}

void ItemOnGrounds::DeleteItem(ItemOnGround* item)
{
    for (it = vecItems.begin(); it != vecItems.end();)
    {
        if (*it == item)
        {
            (*it)->Release();
            delete (*it);
            vecItems.erase(it);
            break;
        }
        else
            ++it;
    }
}
