#pragma once
#include "framework.h"

/// Holds all state information relevant to a character as loaded using FreeType
namespace Text {
	struct Character {
		unsigned int TextureID; // ID handle of the glyph texture
		glm::ivec2   Size;      // size of glyph
		glm::ivec2   Bearing;   // offset from baseline to left/top of glyph
		unsigned int Advance;   // horizontal offset to advance to next glyph
	};
}

class Shader;
class TextRenderer : public Singleton<TextRenderer>
{
private:
	// render state
	unsigned int VAO, VBO;
	// holds a list of pre-compiled Characters
	std::map<char, Text::Character> Characters;
	// shader used for text rendering
	Shader* TextShader;

public:
	virtual HRESULT Init();
	virtual void Release();
	
	// pre-compiles a list of characters from the given font
	void Load(std::string font, unsigned int fontSize);
	// renders a string of text using the precompiled list of characters
	void RenderText(std::string text, float x, float y, float scale = 1.0f, glm::vec3 color = glm::vec3(1.0f));
	void RenderOutText(std::string text, float x, float y, float scale = 1.0f, float outline = 0.2f, glm::vec3 color = glm::vec3(1.0f));
};

