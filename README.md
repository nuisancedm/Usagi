# Usagi
## DAIRY
#### 4/8/2024 Add Entry point 
* 新增Core.h, 用宏封装了__declspec(dllexport)和__declspec(dllimport)
* 新增Usagi.h, 供外部应用使用，包含了所有应用程序应该引入的头文件
* 新增Application.h，Application类由外部应用集成并实现部分函数
* 新增EntryPoint.h，作为头文件main函数，将会被外部应用引入

####  4/11/2024 Add Logging system
我们想要在控制台输出信息，程序运行的信息，shader加载的信息等等。
当程序运行出错的时候，我们想要程序告诉我们。
通过gitsubmodule的方式使用spdlog作为usagi的logging system
打开CMD
```C++
git submodule add https://github.com/gabime/spdlog Usagi/vendor/spdlog
```
我们为usagi和sandbox同时引入include文件夹
下一步我们会重新封装spdlog，我们希望修改API，用我们自己的方式调用log，比如Usagi::log等。我们创建自己的Log类。

* 新建Log.h Log.cpp, 封装了spdlog 并用定义了log宏


