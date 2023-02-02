# SNEZE: Simple Not Easy Zestful Engine
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/MIT)
[![Conduct](https://img.shields.io/badge/Conduct-Covenat%202.0-yellow.svg?style=for-the-badge)](https://www.contributor-covenant.org/version/2/0/code_of_conduct/)
[![C++](https://img.shields.io/badge/20-yellow.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white&labelColor=gray)](https://en.cppreference.com/w/cpp/20)

SNEZE is a high-level, entity component system based 2D game engine that provides solutions for common game problems. The goal is to make game development fast and easy, while also being zestful.

## Features (ready)
- None (just starting the project!)

## Features (under development)
- Simple to use however is not easy to master.
- Code centric, no editor for game logic, just code. 
- Entity Component System to centralize data and provide fast performance.
- High-level Engine allowing developers to focus more on game logic instead of engine logic.
- Solutions for Common Game Problems, such as handling state, transitions, and resolution changes.
- Patterns for Basic Game Scenes such as maps, menus, tiled, scrolling, or UI.
- Already made common effects for any game, blinks, fade in/out, ink.
- Common game systems such sound or controller support.
- Multi-platform : Windows, Mac, Linux.

## Background
After several years using different game engines I always end create a lot of common code with features that are usually too high-level for those engines to support but are required for most of the games that I do. 

This could be as simple flashing a sprite in red color when a player hit an enemy, transition between game states such fade-in-out or to allow the game window to be resized and keeping the game scaling correctly, even if the aspect ratio change.

This engine is trying to step up that effort to allow to focus on the game logic and solve common problems, however keep it configurable some this could be customized by the game.

## Getting Started
To get started, simply follow the instructions in the [Getting Started](GETTING_STARTED.MD) guide.

## Dependencies

SNEZE is powered by these incredible projects:

- [EnTT](https://github.com/skypjack/entt) : A fast and reliable entity component system.
- [raylib](https://www.raylib.com/) : A simple and easy-to-use library to enjoy videogames programming.
- [spdlog](https://github.com/gabime/spdlog) : Fast C++ logging library.

## Contributions
SNEZE is an open source project and contributions are welcome. If you'd like to contribute, please follow the guidelines in the [Contributing](CONTRIBUTING.md) file and make a pull request.

## License
```
Copyright (c) 2022 Juan Medina

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```