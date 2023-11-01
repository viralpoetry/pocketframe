# PocketFrame
Digital picture frame from the PocketBook Basic Touch e-reader. See the [blog](https://www.malgregator.com/post/pocketframe/) for more information.

<img src='https://github.com/viralpoetry/pocketframe/raw/main/pocketframe_final.jpeg'/>

## Usage

Building from source requires Docker or Docker compatible software and the internet connection for pulling dependencies.

1) Build binary from source by running `./build.sh`
2) Copy `pocketframe.app` binary from the `build/` directory to the device `Applications` directory.
3) Create folder `My pictures/PocketFrame/` and copy desired `jpeg` pictures to it.

Alternatively, modify hardcoded settings in the source code (directory, time interval, debug switch).

### Useful tips

600x800 picture resolution is the actual fullscreen, although SDK `Stretch` function can resize photos of other dimensions as well.

To prevent turning device off, disable power saving features.

  * Settings / Saving Power -> Lock Device after = Off
  * Settings / Saving Power -> Power off after = Off
