#include "stream.h"



class Stream {
public:
    virtual void play() = 0; // Pure virtual function for play (needs to be overridden)
    virtual void init() = 0; // Pure virtual function for init (needs to be overridden)
    virtual ~Stream() {}     // Virtual destructor for proper cleanup in derived classes
};

// Derived class Image
class Image : public Stream {


public:
    void play() override {

    }

    void init() override {
        std::cout << "Initializing the image stream." << std::endl;
        // Your code for initializing the image stream
    }
};

// Derived class Video
class Video : public Stream {
public:
    void play() override {
        std::cout << "Playing the video." << std::endl;
        // Your code for playing the video
    }

    void init() override {
        std::cout << "Initializing the video stream." << std::endl;
        // Your code for initializing the video stream
    }
};