# 3D in desmos
<img width="728" height="585" alt="image" src="https://github.com/user-attachments/assets/f4fabe1f-c983-4d13-90a1-a6743565a283" />

https://www.desmos.com/calculator/hk0lywrb4a?lang=en

## How to use
First of all, you need to compile the project and generate the desmos graph that contains the 3D model
```
gcc -O3 src/main.c -o 3desmos
./3desmos models/apple.obj apple.json
```
In order to import the graph you need to install this extension
https://github.com/hyrodium/desmos-text-io

Then you can create new desmos graph and import apple.json using the extension
