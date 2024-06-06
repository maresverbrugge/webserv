<div align="center">
  <img src="img/WEBSERV.png" alt="logo" width="1000" height="auto" />
  <p>Writing our own HTTP server in C++.</p>
</div>

# About the project
A project made in accordance with the Webserv project which is part of the Codam Core Curriculum. This project was a collaboration between with [Marès Verbrugge](https://github.com/maresverbrugge), [Flen Huisman](https://github.com/fhuisman) and [Felicia Koolhoven](https://github.com/fkoolhoven). We created our own HTTP server in C++, following [HTTP 1.1](https://datatracker.ietf.org/doc/html/rfc2616) and taking nginx as an example. HTTP is one of the most used protocols on the internet. We tested our webserv in a browser (Firefox).

# Getting started
Start by cloning the repository:
```c
git clone https://github.com/maresverbrugge/webserv.git
```
Go into the minishell directory and compile by using make:
```c
make
```

# Usage

⚠️ Please note this project is made for Linux only! ⚠️

Webserv takes as input a configuration file, similar to an nginx configuration file. It contains directives such as host (e.g. localhost), port (e.g. 8080) and route (e.g. /pages). See the [config folder](https://github.com/maresverbrugge/webserv/tree/main/config) for examples or read the [subject](subject.pdf) to find the configuration file requirements. To use our default configuration:

```c
./webserv config/default.conf
```
You can then make request through the browser. For example:
```c
http://localhost:8081/
```

# Important features
You can find all the requirement we had to meet in the [subject](subject.pdf). Our webserv:
- Is compatible with at least Mozilla Firefox
- Can serve a fully static website
- Handles GET, POST and DELETE requests
- Uses [epoll](https://man7.org/linux/man-pages/man7/epoll.7.html) to efficiently manage requests
- Will provide accurate HTTP response status codes and error pages
- Allows you to upload and delete files on specified locations
- Can execute [CGI](https://www.geeksforgeeks.org/common-gateway-interface-cgi/), for example to generate an HTML page through a Python script

# Important learnings
This was the first group project we did in C++. We gained knowledge on:
- Object-oriented programming, designing classes
- I/O multiplexing using [epoll](https://man7.org/linux/man-pages/man7/epoll.7.html)
- HTTP requests and responses
- The inner workings of webservers
- Writing reliable parsers in C++
- Using Git and GitHub in a collaborative way
- Concurrent programming (to handle CGI)
- Modern C++ and smart pointers
- HTML, CSS and JavaScript (for our test [web pages](https://github.com/maresverbrugge/webserv/tree/main/root))
- Python (for our CGI scripts)

# Contact
See our profile pages for ways to contact us!
