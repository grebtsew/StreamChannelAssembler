#ifndef STREAM_H
#define STREAM_H

#include <iostream>

class Stream {
public:
    virtual void play() = 0; // Pure virtual function for play (needs to be overridden)
    virtual void init() = 0; // Pure virtual function for init (needs to be overridden)
    virtual ~Stream() {}     // Virtual destructor for proper cleanup in derived classes
};

// Derived class Image
class Image : public Stream {
public:
    void play() override;
    void init() override;
};

// Derived class Video
class Video : public Stream {
public:
    void play() override;
    void init() override;
};


#endif // STREAM_H
