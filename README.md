# NeoVehicle Infotainment System

A modern, feature-rich automotive infotainment interface built with Qt/QML, showcasing advanced UI development skills and automotive software design patterns.

## 🚗 Overview

NeoVehicle is a sophisticated car infotainment system that demonstrates professional-grade Qt/QML development. The application features a clean, intuitive interface with real-time vehicle data management, interactive controls, and modern automotive UI design principles.

## ✨ Features

- **Real-time Vehicle Control**: Lock/unlock functionality with visual feedback
- **Interactive Dashboard**: Dynamic temperature monitoring and user management
- **Modern UI Design**: Clean, automotive-inspired interface with responsive layouts
- **Navigation Integration**: Built-in map functionality using OpenStreetMap
- **Modular Architecture**: Well-structured component system for scalability
- **Property System**: Qt's property system for seamless data binding

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
   cd NeoVehicle-Infotainment
   ```

2. **Build the project**
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. **Run the application**
   ```bash
   ./NeoVehicle
   ```

## 🏗️ Project Structure

```
NeoVehicle/
├── CMakeLists.txt          # Build configuration
├── main.cpp                # Application entry point
├── qml.qrc                 # Resource file
├── controllers/            # Backend logic
│   ├── headers/
│   │   └── system.h       # System controller header
│   └── src/
│       └── system.cpp     # System controller implementation
├── ui/                    # QML components
│   ├── BottomBar/         # Bottom navigation bar
│   ├── LeftScreen/        # Left dashboard panel
│   └── RightScreen/       # Right panel with navigation
├── images/                # UI assets
└── README.md
```

## 🎯 Architecture Highlights

- **System Controller**: Centralized vehicle state management with Qt's property system
- **Component Modularity**: Reusable QML components with clean interfaces
- **Signal-Slot Pattern**: Reactive programming for real-time updates
- **Resource Management**: Efficient asset loading through Qt's resource system

## 🔧 Development Features

- **Property Binding**: Automatic UI updates when vehicle state changes
- **Event System**: Robust event handling for user interactions
- **Memory Management**: RAII principles and Qt's object model
- **Type Safety**: Strong typing with Qt's meta-object system

## 🎨 UI Components

- **BottomBar**: Navigation and system controls
- **LeftScreen**: Primary dashboard with vehicle status
- **RightScreen**: Map integration and secondary controls
- **Interactive Elements**: Lock/unlock controls, temperature display

## 📚 Learning Outcomes

This project demonstrates proficiency in:
- Qt/QML application development
- C++/QML integration techniques  
- Automotive UI/UX design principles
- Component-based architecture
- Build system configuration (CMake)
- Version control and professional documentation

## 🔄 Future Enhancements

- Media playback integration
- Climate control interface
- Vehicle diagnostics dashboard
- Bluetooth connectivity
- Voice command integration
- Multi-language support

## 🤝 Contributing

This is a portfolio project showcasing Qt/QML development skills. Suggestions and feedback are welcome!

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

---

*Built with Qt/QML - Showcasing modern automotive software development*