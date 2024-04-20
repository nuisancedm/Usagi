# Usagi
## 编译
Sandbox通过包含Usagi提供的头文件来获取类，函数原型，常量定义， Sandbox获取的来自Usagi的头文件，就是Usagi库提供的接口，使得Sandbox直到该如何调用Usagi的功能，这些头文件只包含了接口的声明，而不包含具体实现。  


接口的具体实现包含在了Usagi编译成的DLL中，当sandbox运行时，会加载这个dll来使用Usagi实现的功能，这种机制允许sandbox在编译时只知道接口，在运行时通过链接到dll来使用实际的功能实现。

## 事件
Usagi的每一个事件都是单独的类，记录了该事件自身的相关信息。