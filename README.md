# zlab
zlab（easy lab）就是一个简单的实验室，包含了一些实用的c++库。你可以自由地引用到自己的项目里，轻松愉快地使用。

## How to Use
举个栗子：你想要解析或者构造一个json文件，你可以使用zlab中的zjson

在代码中，你需要包含zjson的头文件
```
#include "zjson.h"
```

在编译时，你需要加入zjson的头文件路径。另外由于zjson是对jsoncpp进行了二次封装，所以还需要加入zjson的静态库和路径
```
g++ -o demo_zjson.out *.cpp -I./ -I../../inc -L../../lib -lzjson
```
如果想了解更详细的，请参考demo

## List
目前为止，zlab提供了以下功能，更多功能持续更新中...
* zhashtable.h   基于多阶hash的hash表
* zjson.h  json文件的读写接口
* zload.h  读取文件到vector中
* zsession.h  会话管理，提供增、删、查、改接口和超时删除接口
* ztimer.h  基于select的定时器
* zutil.h  通用工具

## License
In summary, zlab is licensed under the MIT license.

## Author
* 主页：<http://www.o-k.la>
* 邮箱：izuzi@foxmail.com