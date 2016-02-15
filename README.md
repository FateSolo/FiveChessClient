# FiveChessClient

大学生创新项目：网络智能五子棋博弈系统的研究与实现

项目说明：实现了C/S架构的五子棋游戏，支持双人对弈、人机对弈等功能

服务器端代码开源：https://github.com/FateSolo/FiveChessServer

客户端代码开源：https://github.com/FateSolo/FiveChessClient

本篇文章是对客户端程序的简要说明。

### 1. 项目开发环境：
* Windows 8.1
* Visual Studio 2013
* Qt Creator 3.3.0

### 2. 总体说明：
* 主要使用C++实现客户端逻辑功能

* 应用了Qt框架完成对图形界面、网络功能的编写

### 3. 运行环境部署：
这里以Windows 8.1为例

1、请确保系统中已经安装了Visual Studio或者MinGW

2、前往http://www.qt.io/download-open-source/#section-2下载对应的Qt

3、安装Github客户端（如果可以从其他地方获取到源文件，例如大创光盘，那么请忽略第3、4步）

4、使用Github客户端克隆FiveChessClient(请先将github.com/FateSolo下的FiveChessClient项目Fork到你自己的账号)，
或者使用Git Bash克隆(注意将name替换成你的用户名)

    	$ git clone git@github.com:name/FiveChessClient.git

5、打开Qt Creator，导入FiveChessClient，运行

6、在FiveChessClient的父目录中找到构建后的文件夹，进去拷贝出FiveChessClient.exe，存放到单独的文件夹中，这里命名为FiveChess

7、从FiveChessClient文件夹中将config.ini和music文件夹拷贝到FiveChess文件夹中

8、前往Qt的安装目录 Qt\Qt5.4.0\5.4\msvc2013_64\bin，拷贝出icudt53.dll、icuin53.dll、icuuc53.dll、Qt5Core.dll、Qt5Gui.dll、Qt5Multimedia.dll、Qt5Network.dll、Qt5Widgets.dll到FiveChess中

9、前往Qt\Qt5.4.0\5.4\msvc2013_64\plugins，拷贝出imageformats、mediaservice、platforms文件夹到FiveChess中

10、上网下载或从本地找到msvcp120.dll、msvcr120.dll拷贝至FiveChess中

11、打开FiveChess文件夹中的config.ini文件，修改ip为你刚才部署的服务器的ip地址

12、运行FiveChessClient.exe即可

------

作者 [@FateSolo]<br>
2016 年 2月 6日
