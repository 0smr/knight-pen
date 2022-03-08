# Knight Pen
<img src="https://img.shields.io/badge/version-0.1.1-37c248"><br>

A simple screen annotation software.

<div align="center">
    <img src="extera/logo/logo-name-knight-pen.webp" width="400px">
</div>
<br>

<!-- # Preview -->

## Compiling

**Dependencies**

`Qt >= 5.15` - `GCC >= 8.0` - `qmake >= 3.1`.

    sudo apt-get install g++ build-essential qt5-default qttools5-dev qttools5-dev-tools qtdeclarative5-dev*


**Clone repository**

    git clone --recursive https://github.com/SMR76/knight-pen.git

**‌Build**

    cd knight-pen 
    mkdir build
    cd build 
    qmake CONFIG+=release ../src/knight-pen/
    make

## TO-DO
- [ ] Fix mask problem in custom <a href="./src/knight-pen/maskwindow.h">window</a>.
- [ ] Add other pen & brush tools.
- [ ] Capability to move selection.
- [ ] Capability to rotate selection.
- [ ] Capability to resize selection.
- [ ] Capability to translate selection.
- [ ] Capability to intersect shapes.
- [ ] Move InteractiveCanvas component to separate repository.
- [ ] Port to cmake.

## Dependencies
- [Qt](https://www.qt.io/) (LGPLV3) <sub>Core & GUI</sub>
- [QNanoPainter](https://github.com/QUItCoding/qnanopainter) (zlib license) <sub>Canvas</sub>