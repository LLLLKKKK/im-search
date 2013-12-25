im-search
=========

Well, repo for the course chuantiyantao .....

## Introduction

A very very simple image search engine .. for food

## Dependency

Make sure you have these stuff first ...

1. [Scons](www.scons.org) 
2. [protobuf](https://code.google.com/p/protobuf/)
3. [RCFProto](http://www.deltavsoft.com/RCFProto.html)
4. [Django](https://www.djangoproject.com/)
5. [OpenCV](http://opencv.org/)
 
## Build

1. Install protobuf and RCFProto, Scons .. and so on
2. Start SearchService: scons service/ && ./SearchServer
3. Start Django: python manager.py runserver
4. 127.0.0.1:8000, have fun...
