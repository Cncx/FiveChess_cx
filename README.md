>FiveChess_cx:
====================
* author:ThyChan
* email:cn_cx@foxmail.com
* type: C++/C


>>###五子棋简单的电脑算法：
--------------------
	每个空白点分四个方向搜索五点：
	把相连的同色棋子累加，遇到反色中断搜索并重置累计数
	累计结果为1表示有1个同色棋子，该点优先级加1
	累计结果为2表示有2个同色棋子，该点优先级加10
	累计结果为3表示有3个同色棋子，该点优先级加100
	累计结果为4表示有4个同色棋子，该点优先级加1000
	如果需要精确定位电脑棋子可用二维表实现。


>>###MICROSOFT 基础类库: FiveChess_cx 项目概述
===============================================================================

应用程序向导已为您创建了这个 FiveChess 应用程序。此应用程序不仅演示 Microsoft 基础类的基本使用方法，还可作为您编写应用程序的起点。
本文件概要介绍组成 FiveChess 应用程序的每个文件的内容。

>>>#####FiveChess.vcxproj
这是使用应用程序向导生成的 VC++ 项目的主项目文件。 
它包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

>>>#####FiveChess.vcxproj.filters
    这是使用“应用程序向导”生成的 VC++ 项目筛选器文件。
    它包含有关项目文件与筛选器之间的关联信息。在 IDE 中，通过这种关联，在特定节点下以分组形式显示具有相似扩展名的文件。例如，“.cpp”文件与“源文件”筛选器关联。

>>>#####FiveChess.h
这是应用程序的主要头文件。它包括其他项目特定的头文件(包括 Resource.h)，并声明 CFiveChessApp 应用程序类。

>>>#####FiveChess.cpp
这是包含应用程序类 CFiveChessApp 的主要应用程序源文件。

>>>#####FiveChess.rc
这是程序使用的所有 Microsoft Windows 资源的列表。它包括 RES 子目录中存储的图标、位图和光标。此文件可以直接在 Microsoft Visual C++ 中进行编辑。项目资源位于 2052 中。

>>>#####res\FiveChess.ico
这是用作应用程序图标的图标文件。此图标包括在主要资源文件 FiveChess.rc 中。

>>>#####res\FiveChess.rc2
此文件包含不在 Microsoft Visual C++ 中进行编辑的资源。您应该将不可由资源编辑器编辑的所有资源放在此文件中。


>>###应用程序向导创建一个对话框类:

>>>#####FiveChessDlg.h，FiveChessDlg.cpp - 对话框
这些文件包含 CFiveChessDlg 类。该类定义应用程序主对话框的行为。该对话框的模板位于 FiveChess.rc 中，该文件可以在 Microsoft Visual C++ 中进行编辑。


>>###其他标准文件:

>>>#####StdAfx.h，StdAfx.cpp
这些文件用于生成名为 FiveChess.pch 的预编译头 (PCH) 文件和名为 StdAfx.obj 的预编译类型文件。

>>>#####Resource.h
这是标准头文件，它定义新的资源 ID。
Microsoft Visual C++ 读取并更新此文件。


>>###其他注释:
应用程序向导使用“TODO:”指示应添加或自定义的源代码部分。
如果应用程序在共享的 DLL 中使用 MFC，则需要重新发布这些 MFC DLL；如果应用程序所用的语言与操作系统的当前区域设置不同，则还需要重新发布对应的本地化资源 MFC100XXX.DLL。有关这两个主题的更多信息，请参见 MSDN 文档中有关 Redistributing Visual C++ applications (重新发布 Visual C++ 应用程序)的章节。

