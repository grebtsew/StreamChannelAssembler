# StreamChannelAssembler
 Create and publish streams from a folder of images, videos and livefeeds. Just place your arbitrary images and videos in the `./data/contents/` folder and run the application. By utilizing gStreamer and OpenCV the images will be livestreamed to a channel or locally. JSON files with links to webcam or other live feeds can also be used. The stream can then be sent to various media servers. The current implementation is only a MVP (Minimum Viable Product) and more features will be added further on. Check out the `./data/config.json` file to change settings.

![license](https://img.shields.io/github/license/grebtsew/StreamChannelAssembler)
![size](https://img.shields.io/github/repo-size/grebtsew/StreamChannelAssembler)
![commit](https://img.shields.io/github/last-commit/grebtsew/StreamChannelAssembler)


# Motivation
To be fair, I am quite rusty in `C++`, which motivates the decision to create a new project in `C++`. The focus with the project is to create clean and good practice code towards high performing computing. Video streaming is a computer resource heavy task which fit perfectly. The application is strongly coupled to gStreamer and OpenCV which are great libraries to learn when renewing one's code language skills.

# Running
Firstly install dependencies as in the `Dockerfile`.
Then compile, build and run the application by running the following command:
```bash
cmake -S . -B build && cmake --build build && ./build/StreamChannelAssembler
```

# Docker
This repository has been dockerized. Currently this is under construction.

```bash
docker-compose up --build
```
The built project is volumed to the local folder `./build`.
The data folder is also volumed so as to share the content with the docker container easily.

# Testing

Unit Tests are written using `Catch2`, they are linked to github actions. Tests will be built with the rest of the project.

# Examples

These are some examples of streams that can be added to the content folder except for videos and images.

```json
{
    "name": "Test Video Example",
    "type": "video",
    "target": "videotestsrc ! videoconvert ! appsink",
    "length": 20, // how many seconds the live stream should play
    "start": 0 // where in the stream we should start
}
```

```json
{
    "name": "Local Webcam Example",
    "type": "video",
    "target": 0,
    "length": 20,
}
```
These JSON objects can be seen as defining the input to the application.



# Supported Data Types
This list show supported datatypes in a table.

| FileType | ContentType | Explaination | Status |
|----------|----------|----------|----------|
| .png   | Image   | Std. Type | ![Pass](https://img.shields.io/badge/-Pass-brightgreen) |
| .jpg   | Image   | Std. Type  | ![Pass](https://img.shields.io/badge/-Pass-brightgreen) |
| .jpeg   | Image   | Std. Type | ![Pass](https://img.shields.io/badge/-Pass-brightgreen) |
| .json   | GStreamer   | Define a gStreamer command to appsink. See gstreamer_example.json   | ![Pass](https://img.shields.io/badge/-Untested-yellow) |
| .json   | FFMPEG   | Define a FFMPEG command to app. See ffmpeg_example.json   | ![Pass](https://img.shields.io/badge/-Untested-yellow) |
| .json   | Video   | Define a file or command as video. See webcam_example.json   | ![Pass](https://img.shields.io/badge/-Pass-brightgreen) |
| .json   | Audio   | Define a file or command as audio. See radio_example.json  | ![Pass](https://img.shields.io/badge/-Untested-yellow) |
| .mp4   | Video   | Std. Type  | ![Pass](https://img.shields.io/badge/-Untested-yellow) |
| .avi   | Video   | Std. Type | ![Pass](https://img.shields.io/badge/-Untested-yellow) |
| .mov   | Video   | Std. Type | ![Pass](https://img.shields.io/badge/-Untested-yellow) |
| .gif   | Video   | Gif:s are seen as video | ![Pass](https://img.shields.io/badge/-Untested-yellow) |
| .mkv   | Video   | Std. Type | ![Pass](https://img.shields.io/badge/-Untested-yellow) |
| .mp3   | Audio   | Std. Type | ![Pass](https://img.shields.io/badge/-Untested-yellow) |

How the formats are handled by the media processors are illustrated in the figure below.
// TODO:

# Known issues

This is a list of known issues that hopefully will be solved in near future:

* pure gstreamer commands can't run
* readme need more examples and helpful tips of how to run and so on.
* demo added to readme
* fix performance
* add scheduler
* add a couple of playlist orders, such as random
* fix local view in a manageable windowform
* add test rtsp, hls and webrtc stream commands
* no animations
* no overlays


# Some major sources

* https://stackoverflow.com/questions/37339184/how-to-write-opencv-mat-to-gstreamer-pipeline
* https://chat.openai.com/
* https://github.com/bluenviron/mediamtx
* https://github.com/nlohmann/json



# License

This project is under the MIT license, read more [here](/LICENSE).

Copyright (c) 2023 Grebtsew
