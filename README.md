# 欧拉操作实现扫成

#### 环境

windows，VS2019，Debug，x64

OpenGL 3.3 Core

glfw + glad

glfw：[Download | GLFW](https://www.glfw.org/download.html)

glad：[https://glad.dav1d.de](https://glad.dav1d.de/)

#### 第三方库

poly2tri：将带内环的面三角化

#### 相关链接

glfw：[Download | GLFW](https://www.glfw.org/download.html)

glad：[https://glad.dav1d.de]

poly2tri：[greenm01/poly2tri: Automatically exported from code.google.com/p/poly2tri (github.com)](https://github.com/greenm01/poly2tri)

（这些第三方库的h文件和lib文件都已经放在third_libraries的Include和Libs文件中，不需要再进行下载）

#### 使用说明

使用Visual Studio2019直接打开hw_test.sln文件

把third_libraries文件夹中的Include和Libs的路径分别添加到外部包含目录和库目录中，在附加依赖项中添加：glfw3.lib、opengl32.lib、Project1.lib（由于poly2tri只给出了.h和.c文件，所以我生成了poly2tri的静态库文件Project1）

#### 操作说明

模型是带有两个通孔的长方体。默认为着色模型，按空格键可在着色模型和线框模型之间进行切换

（做了简单的变换和透视，如果发现模型有点扭曲，那不是error，只是单纯的透视效果）

#### 结果

<video src="E:\8_学校\1_课程文件\3_三维CAD建模\作业\mine\结果\结果.mp4"></video>