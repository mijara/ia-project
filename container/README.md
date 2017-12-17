# docker-binutils - a Docker container for running binutils such as gprof

# DOCKER HUB

https://registry.hub.docker.com/u/mcandre/docker-binutils/

# EXAMPLE

```
$ docker run -it -v $(pwd):/mnt mcandre/docker-binutils bash
root@b2b639fe64c0:/# cd /mnt
root@b2b639fe64c0:/mnt# mkdir -p bin/
root@b2b639fe64c0:/mnt# gcc -o bin/hello -pg --static hello.c
root@b2b639fe64c0:/mnt# bin/hello
root@b2b639fe64c0:/mnt# gprof bin/hello gmon.out | head
gprof bin/hello gmon.out | head
Flat profile:

Each sample counts as 0.01 seconds.
 no time accumulated

  %   cumulative   self              self     total
 time   seconds   seconds    calls  Ts/call  Ts/call  name
  0.00      0.00     0.00        1     0.00     0.00  main

 %         the percentage of the total running time of the
root@b2b639fe64c0:/mnt# exit
$ ls bin/
hello
```

# REQUIREMENTS

* [Docker](https://www.docker.com/)

## Optional

* [make](http://www.gnu.org/software/make/)
* [Node.js](https://nodejs.org/en/) (for dockerlint)

## Debian/Ubuntu

```
$ sudo apt-get install docker.io build-essential
```

## RedHat/Fedora/CentOS

```
$ sudo yum install docker-io
```

## non-Linux

* [VirtualBox](https://www.virtualbox.org/)
* [Docker Toolbox](https://www.docker.com/toolbox)

### Mac OS X

* [Xcode](http://itunes.apple.com/us/app/xcode/id497799835?ls=1&mt=12)
* [Homebrew](http://brew.sh/)
* [brew-cask](http://caskroom.io/)

```
$ brew cask install dockertoolbox
```

### Windows

* [Chocolatey](https://chocolatey.org/)

```
> chocolatey install virtualbox make
```

* [DockerToolbox-1.8.2c.exe](https://github.com/docker/toolbox/releases/download/v1.8.2c/DockerToolbox-1.8.2c.exe)
