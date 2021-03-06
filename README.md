# Native Example Bundle

This is a simple SDL2 game, based on the classic Atari Indy800 title,
from 1975.

## Installing

You will need to enable Atari Homebrew on your VCS. Once you have done
so, go to

  https://atari-vcs:3030/

and select `Upload`. Select the file `native-indy800-example_0.1.0.zip`
built from this repository. You can then launch this game, like any
Homebrew title, from the same interface.

## Gameplay

Use your Atari controller to drive around the track, scoring points
as you go. 

Using/ an Atari modern controller, the controls are:
- Left trigger: Accelerate
- Right trigger: Brake
- Left stick: Steer

Using an Atari classic controller, the controls are:
- A: Accelerate
- B: Brake
- Stick: Steer

On the classic controller, You can steer both by using the digital
stick conventionally, or by twisting it like a paddle.

Press (A) and then (Menu) to start a game.

## Building


You can create the bundle by running

    mkdir build
    cd build
    make_bundle.sh ../native-indy800-example.yaml

in any suitable Linux-like environment with the
[bundle-gen](https://github.com/atari-vcs/bundle-gen) script
[`make-bundle.sh`](https://github.com/atari-vcs/bundle-gen/blob/main/make-bundle.sh)
installed in your PATH, and Docker installed on your machine.

## License

This example is made available under either an
[Apache-2.0](https://opensource.org/licenses/Apache-2.0) or an [MIT
license](https://opensource.org/licenses/MIT). You should be remember
that the libraries it depends on may be less permissively licensed.
