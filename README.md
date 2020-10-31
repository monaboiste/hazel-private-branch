[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://github.com/monaboiste/hazel-private-branch/blob/master/LICENSE)

<p align="center">
	<img src="branding/hazel_logo.png" width="400">
</p>

Hazel is primarily an early-stage interactive application and rendering engine for Windows. Repository is basically one-to-one copy of [TheCherno's](https://github.com/TheCherno/Hazel) with little tweaks there and there.
Everything inside this repository is being created based off YouTube videos, found at [thecherno.com/engine](https://thecherno.com/engine).  
Purpose of this project is understanding computer graphic and learning how to write my own game engine.

## Getting Started
Visual Studio 2017 or 2019 is recommended, Hazel is officially untested on other development environments whilst we focus on a Windows build.

Clone the repository with `git clone --recursive https://github.com/monaboiste/hazel-private-branch.git`.

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.
  
In order to generate VS solution run `win-genproj.bat` inside scripts folder.