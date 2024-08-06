# Breakout

<p align="center">
  <img src="https://github.com/user-attachments/assets/2816e2d7-233e-441a-800a-34bca7a08b0d" alt="Image">
</p>


## About 

This is an implementation of the classic game breakout for the ChipKIT PIC32 microcontroller. The hardware specific code has been adjusted to run via WebAssembly. 

The objective of the game is to break all the blocks at the top of the screen and in doing so collecting a high score. Just like in Pong there is a paddle that the player controls to bounce the ball and the trajectory of the ball is determined by which angle the ball hits the paddle.

## How to run

For a live version of the game, refer to (link)

### Prerequisites


The WebAssembly port of this project was made with the [emscripten](https://emscripten.org/) toolchain and all its dependencies.  

> For the ChipKIT PIC32 implementation of this project, refer to commit [50f46fe](https://github.com/skvarre/projectbreakout/tree/50f46fe1ae42a529c88e8b7a9916700b7a2b7f83)

### Build

> Note: Building the project is not necessary if you only want to test it. In that case, continue to the next section. 

- To build the project, use the `Makefile` in the `src` directory.

```sh
make
```
- To clean up the compiled files, run:
```sh
make clean
```


### Run

To run this project, you'll need to start a localhost server. Here's an example in Python 3.X:

```python
python -m http.server 8000
```

Then navigate to https://localhost:8000



## Contributors

This was made as a school project in 2020 by Robin Nordmark and Tim Olsén. It was ported in 2024 to WebAssembly by Tim Olsén. 

