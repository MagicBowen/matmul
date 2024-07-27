## TODOs

- [x]: 支持一个对象注册多个DFX代理接口；
- [x]: 支持如果类没有注册，则依赖获取的代理返回原类；
- [x]: 支持用户注册多种不同的proxy链条；
- [x]: 支持const方法中对module的using；
- [ ]: 接口确定下来，评审一次；
- [ ]: 尽早合入DFX对框架的影响部分；

## Usage

Download this project template, and rename folder name to your project name.

```sh
$ chmod a+x ./ccup.sh

# start project from docker env
# make sure docker is pre-installed on the system.
./ccup.sh -e

# update depends and execute cmake generating
./ccup.sh -u

# build
./ccup.sh -b

# update & build
./ccup.sh -ub

# run tests
./ccup.sh -t

# build & test
./ccup.sh -bt

# update & build & test
./ccup.sh -ubt

# run executable
./ccup.sh -r

# install
./ccup.sh -i

# build & install
./ccup.sh -bi

# update & build & install
./ccup.sh -ubi

# clean build
./ccup.sh -c

# clean all
./ccup.sh -C

# help
./ccup.sh -h
```
