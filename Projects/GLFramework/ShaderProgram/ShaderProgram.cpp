#include "ShaderProgram.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath)
	: ref{ glCreateProgram() }
{
	Shader vertexShader(GL_VERTEX_SHADER, ShaderCode(vertexPath).c_str());
	Shader fragmentShader(GL_FRAGMENT_SHADER, ShaderCode(fragmentPath).c_str());

	glAttachShader(ref, vertexShader.Ref());
	glAttachShader(ref, fragmentShader.Ref());

	glLinkProgram(ref);

	GLint success;
	glGetProgramiv(ref, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		char infoLog[512];
		glGetProgramInfoLog(ref, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDetachShader(ref, vertexShader.Ref());
	glDetachShader(ref, fragmentShader.Ref());
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(ref);
}

void ShaderProgram::use()
{
	glUseProgram(ref);
}

void ShaderProgram::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ref, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ref, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ref, name.c_str()), value);
}

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(ref, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ref, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setMat4(const std::string& name, glm::mat4 value, bool transpose) const
{
	glUniformMatrix4fv(glGetUniformLocation(ref, name.c_str()), 1, transpose, glm::value_ptr(value));
}

std::string ShaderProgram::ShaderCode(const GLchar* path) const
{
	std::ifstream shaderFile(path);

	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();

	return shaderStream.str();
}

ShaderProgram::Shader::Shader(GLenum shaderType, const char* shaderCode)
	: ref{ glCreateShader(shaderType) }
{
	glShaderSource(ref, 1, &shaderCode, NULL);
	glCompileShader(ref);

	GLint success;
	glGetShaderiv(ref, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		char infoLog[512];
		glGetShaderInfoLog(ref, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
}

ShaderProgram::Shader::~Shader()
{
	glDeleteShader(ref);
}

GLuint ShaderProgram::Shader::Ref() const
{
	return ref;
}

// 함수형 쉐이더 로드
/*

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// 쉐이더들 생성
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// 버텍스 쉐이더 코드를 파일에서 읽기
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("파일 %s 를 읽을 수 없음. 정확한 디렉토리를 사용 중입니까 ? FAQ 를 우선 읽어보는 걸 잊지 마세요!\n", vertex_file_path);
		getchar();
		return 0;
	}

	// 프래그먼트 쉐이더 코드를 파일에서 읽기
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// 버텍스 쉐이더를 컴파일
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// 버텍스 쉐이더를 검사
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// 프래그먼트 쉐이더를 컴파일
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// 프래그먼트 쉐이더를 검사
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// 프로그램에 링크
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// 프로그램 검사
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLubyte* LoadBmp(const char* Path, int* Width, int* Height)
{
	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER* fh = NULL;
	BITMAPINFOHEADER* ih;
	BYTE* pRaster;
	hFile = CreateFileA(Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	FileSize = GetFileSize(hFile, NULL);
	fh = (BITMAPFILEHEADER*)malloc(FileSize);
	ReadFile(hFile, fh, FileSize, &dwRead, NULL);
	CloseHandle(hFile);
	int len = FileSize - fh->bfOffBits;
	pRaster = (GLubyte*)malloc(len);
	memcpy(pRaster, (BYTE*)fh + fh->bfOffBits, len); // RGB로 순서를 바꾼다.
	for (BYTE * p = pRaster; p < pRaster + len - 3; p += 3)
	{
		BYTE b = * p; *p = * (p + 2); *(p + 2) = b;
	}
	ih = (BITMAPINFOHEADER *)((PBYTE)fh + sizeof(BITMAPFILEHEADER));
	*Width = ih -> biWidth;
	*Height = ih -> biHeight;
	free(fh);

	return pRaster;
}

*/
//출처: https://202psj.tistory.com/1456?category=273755 [알레폰드의 IT 이모저모]
