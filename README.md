# 3D in desmos
<img width="908" height="602" alt="image" src="https://github.com/user-attachments/assets/2c37ebe0-808b-47dc-ba1a-caa5112d2c2f" />
<img width="930" height="676" alt="image" src="https://github.com/user-attachments/assets/4557bad1-1cba-46fa-a5e7-df82c6499e94" />

## How to use
First of all, you need to compile the project and generate the desmos graph that contains the 3D model
```
gcc -O3 src/main.c -o 3desmos
./3desmos models/apple.obj apple.json
```
In order to import the graph you need to install this extension
https://github.com/hyrodium/desmos-text-io

Then you can create new desmos graph and import apple.json using the extension
