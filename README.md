## Raytracing
A personal repo to keep track of [Ray Tracing in One Weekend book](https://raytracing.github.io/books/RayTracingInOneWeekend.html).

The source code contains source code from the book, there can be some alterations. The code also contains detail explaination of everything possible, and is intended for notes purposes.
> If you are doing the book, dont just copy the whole code. Please understand it first

### Compiling
- First clone the repo
  ```
  git clone https://github.com/chirag-droid/raytracing
  ```
- Then to compile
  ```
  cmake -B build -G <build system>
  ```
  This will build the project for your build system in build directory
- Then to run
  ```
  cd build
  <build system> run
  ```
  This will automatically compile(if needed) and run the code and produce the image.ppm file
