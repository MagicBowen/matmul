## TODOs

- [x]: 支持一个对象注册多个DFX代理接口；
- [x]: 支持如果类没有注册，则依赖获取的代理返回原类；
- [ ]: 支持如果类的接口没有注册代理接口，则依赖获取返回原类接口；
- [ ]: 支持静态static接口，以及对静态static接口的代理；
- [ ]: 支持用户注册多种不同的Proxy链条；

- [ ]: 接口确定下来，评审一次；
- [ ]: 尽早确定DFX对框架的影响，尽早合入，后续变更不易；

## DFX设计约束

- 用户注册DFX代理的时候，可以提供类名和接口名（不用提供接口参数类型和返回值类型）
- Client类型声明依赖的时候，只能指定类名
- Client类使用依赖类型的时候，决定接口名称和参数，框架背后自行推导选择代理类的接口；
- Client类使用依赖类型的时候，如果发现对应的类型没有进行注册，则使用原类型原接口；
- Client类使用依赖类型的接口时候，如果发现对应的类型注册了，但是接口没有进行注册，则使用原类型原接口；

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
