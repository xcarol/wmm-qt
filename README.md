# Where's My Money

A Personal 'Fintech' Application  

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/f5be67cc1a014fcab81676691fcbd100)](https://app.codacy.com/gh/xcarol/wmm/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)  

## Development environment

For Ubuntu 23.10, install:

- build-essential
- cmake
- docker
- docker-compose
- freeglut3-dev

Download and install Qt from [https://www.qt.io/download-open-source](https://www.qt.io/download-open-source)  
Install Qt at _/home/xcarol/Qt_ and check the path declared at _CMakeLists.txt_ when setting _CMAKE_PREFIX_PATH_ to be correct.

### MySql Driver for Qt

At first run of the application the following error can raise up in the console and the application cannot connect to the MySql Server.  

``` Log
QMYSQL driver not loaded
QSqlDatabase: available drivers: QMIMER QODBC QPSQL QSQLITE QMARIADB QMYSQL
```

The problem is that _Qt_ cannot find _libmysqlclient*_ library.  
To know the needed version by the driver go to _Qt_ installation directory where drivers are located (p.e. ~/Qt/6.7.0/gcc_64/plugins/sqldrivers) and run the following command: `ldd libqsqlmysql.so` look at the _libmysqlclient_ version `libmysqlclient.so.21` in this case 21. If it says `libmysqlclient.so.21 => Not found` then run: `sudo apt install libmysqlclient21`. If doesn't says `Not found` go to pray because I have no idea.

## Vs Code

Install the following extensions

- ms-vscode.cpptools-extension-pack
- llvm-vs-code-extensions.vscode-clangd
- twxs.cmake
- ms-vscode.cmake-tools
- vadimcn.vscode-lldb

## Qt Creator

I olny used it to create de project and to edit the *.ui files with Qt Designer

## MySql Server

There is a _docker-compose.yml_ file to create a local mysql database.  
And a _mysqlserver.sh_ script to manage it.  

Acces database through [http://localhost:8080](http://localhost:8080)  
