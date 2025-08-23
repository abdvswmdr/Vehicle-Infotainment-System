# Vehicle Infotainment System

A modern, feature-rich automotive infotainment interface built with Qt/QML, showcasing advanced UI development skills and automotive software design patterns.

## 🚗 Overview

VehicleSys is a sophisticated car infotainment system that demonstrates professional-grade Qt/QML development. The application features a clean, intuitive interface with real-time vehicle data management, interactive controls, and modern automotive UI design principles.

<img width="1030" height="826" alt="image" src="https://github.com/user-attachments/assets/db96e3ba-e5ea-4e8a-8cc5-13212c242d64" />


![Vehicle Infotainment System Demo](images/region_rec_20250823_093651_medium_reddit.gif)

## ✨ Features

- **Real-time Vehicle Control**: Lock/unlock functionality with visual feedback
- **Dual-Zone Climate Control**: Independent driver and passenger HVAC systems (50-90°F)
- **Intelligent Audio Management**: Dynamic volume control with 4-state visual feedback
- **Interactive Dashboard**: Temperature monitoring, clock display, and user profile
- **Navigation Interface**: Search box with map integration using MapboxGL
- **Quick Access Controls**: Central navigation icons for music, map, phone, and video
- **Modern UI Design**: Clean, automotive-inspired interface with responsive layouts
- **Modular Architecture**: Reusable component system with clean separation of concerns
- **Property System**: Qt's property system for real-time data binding and updates

## 🛠️ Technical Stack

- **Framework**: Qt 5.15+ with QML
- **Language**: C++ (backend) / QML (frontend)  
- **Build System**: CMake 3.20+
- **Architecture**: Model-View-Controller (MVC) pattern
- **Components**: QtQuick, QtLocation, QtPositioning

## 📋 Prerequisites

- Qt 5.15 or higher
- CMake 3.20+
- C++17 compatible compiler
- Git

### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install qt5-default qtdeclarative5-dev qtpositioning5-dev qtlocation5-dev cmake build-essential
```

## 🚀 Building & Running

1. **Clone the repository**
   ```bash
   git clone [repository-url]
   cd Vehicle-Infotainment
   ```

2. **Build the project**
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. **Run the application**
   ```bash
   ./VehicleSys
   ```

## 🏗️ Project Structure

```
VehicleSys/
├── CMakeLists.txt          # Build configuration
├── main.cpp                # Application entry point
├── qml.qrc                 # Resource file
├── controllers/            # Backend logic
│   ├── headers/
│   │   ├── system.h       # Core system controller
│   │   ├── hvachandler.h  # Climate control management
│   │   └── audiocontroller.h # Volume control system
│   └── src/
│       ├── system.cpp     # System controller implementation
│       ├── hvachandler.cpp # HVAC logic implementation
│       └── audiocontroller.cpp # Audio control implementation
├── ui/                    # QML components
│   ├── BottomBar/         # Interactive bottom controls
│   │   ├── BottomBar.qml  # Main bottom interface
│   │   ├── HVACComponent.qml # Temperature controls
│   │   └── VolumeControlComponent.qml # Audio controls
│   ├── LeftScreen/        # Left dashboard panel
│   └── RightScreen/       # Map and navigation interface
│       ├── RightScreen.qml # Main right panel
│       └── NavigationSearchBox.qml # Search functionality
├── images/                # UI assets
└── README.md
```

## 🎯 Architecture Highlights

- **Multi-Controller Backend**: Specialized C++ controllers for system, HVAC, and audio management
- **Component Modularity**: Reusable QML components with configurable property bindings
- **Signal-Slot Pattern**: Reactive programming for real-time UI updates
- **Event-Driven Design**: Mouse area interactions with immediate visual feedback
- **Resource Management**: Efficient asset loading through Qt's resource system
- **Property Binding**: Automatic UI synchronization with backend data changes

## 🔧 Development Features

- **Advanced Property System**: Q_PROPERTY macros for seamless C++/QML data binding
- **Interactive Controls**: Temperature increment/decrement with boundary checking
- **Dynamic UI States**: Volume icons that change based on audio level
- **Timer-Based Feedback**: Temporary UI state changes with automatic restoration
- **Event System**: Comprehensive mouse area handling for user interactions
- **Memory Management**: RAII principles and Qt's parent-child object model
- **Type Safety**: Strong typing with Qt's meta-object system

## 🎨 UI Components

### Interactive Bottom Bar
- **HVACComponent**: Dual-zone temperature controls with +/- buttons and live display
- **VolumeControlComponent**: Smart audio control with 4-state icon system
- **Navigation Icons**: Quick access buttons for music, map, phone, and video apps

### Right Screen Panel
- **NavigationSearchBox**: Interactive search with dynamic placeholder text
- **Status Bar**: Lock control, real-time clock, temperature, recording indicator, user profile
- **Map Integration**: MapboxGL rendering with navigation capabilities

### System Elements
- **Dynamic Icons**: Context-aware visual feedback (lock states, volume levels)
- **Responsive Layout**: Components that scale with screen dimensions
- **Real-time Updates**: Live data binding with automatic UI refresh

## 📚 Learning Outcomes

This project demonstrates proficiency in:
- **Advanced Qt/QML Development**: Complex application architecture with multiple interacting components
- **C++/QML Integration**: Seamless data flow using property system, signals, and invokable methods  
- **Interactive UI Design**: Touch-friendly automotive interface with immediate feedback
- **Component-Based Architecture**: Reusable, configurable QML components with clean interfaces
- **Event-Driven Programming**: Responsive user interaction handling with state management
- **Build System Mastery**: CMake configuration with automatic MOC/UIC processing
- **Software Engineering**: Clean code practices, documentation, and version control


## 🔄 Future Enhancements
<!--
- **Media Playback**: Full music player with playlist management
- **Advanced Climate**: Automatic climate zones with sensors
- **Vehicle Diagnostics**: Real-time performance monitoring dashboard  
- **Connectivity**: Bluetooth device pairing and management
- **Voice Commands**: Speech recognition for hands-free control
- **Customization**: User profiles with personalized settings
- **Animation System**: Smooth transitions and micro-interactions
-->
## 🤝 Contributing

This is a **personal learning project** designed to enhance C++ and Qt/QML development skills. The project demonstrates software engineering practices and automotive UI development techniques. Feedback and suggestions for improvements are welcome!

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

---

*Built with Qt/QML - Showcasing modern automotive software development*
