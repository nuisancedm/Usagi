# Usagi Notes

## 1 Before Rendering

### 1.1 Compile

we have **USAGI** as an engine we build.  
we have **SANDBOX** as a client who gose to use USAGI to create something.  

**USAGI** provides *usagi.h*, which is a headfile includes all definitions of classes or functions the client (**SANDBOX**) shoule be #included by the client. in the other way of saying, *usagi.h* is the USAGI interface for client to use.

however *usagi.h* are just definitions, it doesn't have implementation.  
**USAGI** will be dynamic linked to the the client, client could get the implementation from the dll file.

### 1.2 pipeline

the Client (**SANDBOX**) is going to create an Usagi Application.  
this Usagi Application will create the Window for the whole project to run.  
the Window will deal with all event.  

### 1.3 class Application

Client should create an Usagi Application by define the this function.

``` C++

Usagi::Application* Usagi::CreateApplication()

```

Client could get access to Usagi API only after the Usagi Application is created.  

**the role of the Application:**  

* owner of the Window.
* manager of the layerstack.
* place where the run loop happens.

### 1.4 Event System

events is Usagi are currently blocking , meaning when an event occurs it immediatetly gets dispatched and must be dealt with right then an there. for the future, a better strategy might be to buffer events in an event bus and process them during the "event" part of the update stage.  

each event happened will be dealt as an instace of a event class. the instance will hold some infomation and sone functions related to the event.

#### 1.4.0 event process pipeline

* Application hold a WindowsWindow.
* Application will define a function (OnEvent) and pass the function to WindowsWindow. WindowsWindow get the function, save it in m_Data. WindowsWindow now has a native glfwwindow and m_Data.
* WindowsWindow bind the m_Data to glfwwindow by using glfwSetWindowUserPointer.
now glfwwindow could get access to m_Data and get the OnEvent function.  
* user click or type, glfwwindow will get to know first and call glfw callback.
* in glfwcallback, glfwwindow create the event and call OnEvent in m_Data.

#### 1.4.1 class Event

it is a pure virtual base class (or interface) of all kind of other event. all other event class must be derived from it.  

it defines that Class EventDispatcher is a friend class of all Event class.

MEMBERS:

``` C++
EventType GetEventType()                  // return a enum of the event type 
const char* GetName()                     // return the const char* string of the event type 
std::string ToString()                    // wrap of GetName(), return std::string  
int GetCategoryFlags()                    // return a enum integer of the Event Category
bool IsInCategory(EventCategory category) // check if the event is in the given category
bool Handled = false;                     // if the event has been handled
```

#### 1.4.2 class EventDispatcher

every event will have its own EventDispatcher. in the other way an EventDispatcher will hold an event.  
EventDispatcher will be instanlized with a reference to an event, then it will call EventDispatcher.Dispatch(func) to give the event a function to deal with it.  

EventDispatcher.Dispatch(func) will check if the function given is used to deal with the event it holds.

MEMBERS:

``` C++
private:
    Event& m_Event;                // the event this dispatcher holds
public:
    bool Dispatch(EventFn<T> func) // use an function to deal with the event
```

### 1.5 class Window

Usagi provide a basic window interface in *window.h*  
according to different platform, Usagi provides different implementation.

#### 1.5.1 WindowsWindow
WindowsWindow is a wrap of GLFWwindow

TODO...

### 1.6 Input Polling

TODO... 

## Rendering

### Native Render API












