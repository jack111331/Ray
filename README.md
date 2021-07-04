# Ray
== A robust shading pipeline construction rendering engine ==
## Clone
- Be sure to clone this project with following command to include all the submodule this project will use
```shell
git clone --recursive https://github.com/jack111331/Ray
```

## How to build?
### [GLAD](http://glad.dav1d.de/)
- Because each GPU driver offer different OpenGL function address to use function, we need to load it on run-time, the GLAD perfectly resolve this by providing an online service to let you choose your OpenGL version and your GPU driver specific extensions.
- Follow [The tutorial](https://learnopengl-cn.github.io/01%20Getting%20started/02%20Creating%20a%20window/#glad) here may give you more help.
## Reference
- [GLSL Path Tracer](https://github.com/knightcrawler25/GLSL-PathTracer)