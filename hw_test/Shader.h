#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

class Shader {
public:
	unsigned int ID;	//程序ID

	Shader();
	//构造器读取并创建着色器
	Shader(const char* vertexPath, const char* fragmentPath);
	//使用/激活程序
	void use();
	//uniform工具函数
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	void setMat4(const std::string& name, glm::mat4);

};




#endif
