#sensitizer
##Introduction

sensitizer is a simple little tool that forces a program to behave as if all filesystems are case-sensitive. It's handy for testing that programs behave correctly without actually having to run OS X on a case-sensitive filesystem. 

##Requirements

* OS X Mavericks
* The ability to run programs as root

##Usage

    make
    sudo ./sensitizer /bin/ls /system

## License

sensitizer is released under the zlib license. To see the exact details, look at the `LICENSE` file.
