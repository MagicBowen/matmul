## TODOs

- [x]: 完成 MMad 及其测试
- [ ]: 端到端打通基本流程和测试
- [ ]: 支持不同的迭代策略
- [ ]: 支持不同的分块策略
- [ ]: 支持不同的缓存策略
- [ ]: 支持MDL
- [ ]: 支持不同的移出策略
- [ ]: 支持不同的格式转化
- [ ]: 支持不同的量化策略

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
