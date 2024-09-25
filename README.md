![favicon](web/favicon/favicon-32x32.png)

# CloudShell
A lightweight, simple to use and (linux-nerd-)intuitive way to manage your files in the cloud. 

CloudShell works by emulating a console that lets you list, upload, download, move and do so much more with your files. 

![example help](img/help.png)

## Work in progress
This project is nowhere near finished. I am currently preparing for a new job that involves lots of c++ and thus I found it to be a good project. 

## Dependencies
This project uses [Crow](https://github.com/CrowCpp/Crow). If you want to compile it yourself, put the `crow/include` into `./include`. You also need [Asio](https://think-async.com/Asio/), because its a requirement of Crow. 
This project uses [nlohmann/json](https://github.com/nlohmann/json) for de-/serializing objects. 

## Contributions
If you have Ideas and the time to make them work, feel free, to open a PR. Every PR is welcome. 


## Todo-List
- add classes to manage files
    - add json-meta files to stored files, so that we can delete and still have a backup
    - make sure that the stored data survives a shutdown/reboot
- implement most of the commands
    - add actual file upload
    - add actual file download
    - filter with emulated grep
- rename main.cpp to server.cpp
- create better logo (its kinda ok)
- sanitize user input
    - I am quite sure that you can break it all with simple HTML
- maybe rebrand the commands to be more in line with upload and download (or maybe use scp/wget/etc. as names)
- add light mode? 
