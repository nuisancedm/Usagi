# Usagi
## 编译
Sandbox通过包含Usagi提供的头文件来获取类，函数原型，常量定义， Sandbox获取的来自Usagi的头文件，就是Usagi库提供的接口，使得Sandbox直到该如何调用Usagi的功能，这些头文件只包含了接口的声明，而不包含具体实现。  

接口的具体实现包含在了Usagi编译成的DLL中，当sandbox运行时，会加载这个dll来使用Usagi实现的功能，这种机制允许sandbox在编译时只知道接口，在运行时通过链接到dll来使用实际的功能实现。

## pipeline
客户端 创建 Application。  
Applicaiton 创建 Window。

## Application类
由应用创建的一个Usagi Application，创建了Application，应用才能获取并使用Usagi的API。  
可以直接创建Application，也可以用Application的派生类来创建Application。

### 属性
* **m_Window**：application创建的window的智能指针。
* **m_Running**：render loop的循环变量。
* **m_LayerStack**: LayerStack类的实例，栈存储所有的layer。
* **s_Instance**：静态属性，Application实例的指针，应用只能创建一个application。

### 方法
* **构造函数**：为私有属性赋值，创建Window，为Window设置回调函数。 
* **Run()**：render loop所在函数，负责更新window和所有的layer
* **OnEvent(Event& e)**：它是Application的非静态成员函数，但是它会被传给window作为window的回调函数。该函数中首先创建一个事件派发器，由派发器根据传入的事件类型决定使用哪个函数处理事件。然后遍历所有的layer，找到第一个能处理这个事件的layer让他处理。

## Layer类
### 方法
* OnAttach()：Layer被Push进stack调用。
* OnDetach()：Layer被Pop出stack调用。
* OnUpdate(): Layer再render loop中调用。
* OnEvent(): 事件冒泡到本layer时调用。
