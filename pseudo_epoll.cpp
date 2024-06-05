// PSEUDO CODE EPOLL:
// try to cast events[i].data.ptr to Server class / Client class / CGI class / Signal class
// to find out on what kind of socket the EPOLL-event is happening.

// if EPOLLIN on server socket:
    // accept connection with accept(), create client class
    // add new connection to epoll
// if EPOLLIN on connection socket (client class):
    // use read() to read data
    // parse request
    // proces request: make response or error
    // set FLAG = ready_to_write
// if EPOLLOUT on connection socket && FLAG == ready_to_write:
    // use send() to write response back to client
    // after response, remove connection (client class) from epoll
    // after response, close connection (client class)
// if EPOLLHUP on connection socket (client class):
    // remove connection (client class) from epoll
    // close connection (client class)

// end of program:
// remove all servers from epoll:
// epoll_ctl(_socketFD, EPOLL_CTL_DEL, server->getSocketFD(), &event_list[i]);




// TO TEST:
// std::cout << "epoll_return = " << epoll_return << std::endl;
// std::cout << "i = " << i << std::endl;
// std::cout << "event_list[i].data.ptr->_socketFD: " << ready_listDataPtr->getSocketFD() << std::endl;
// if (client != NULL)
// {
// 	std::cout << "This fd is a client\n";
// 	if (client->getReadyForFlag() == READ)
// 		std::cout << "Client->getReadyForFlag() == READ\n";
// 	else
// 		std::cout << "Client->getReadyForFlag() == WRITE\n";
// }
// std::cout << "event_list[i].events = " << event_list[i].events << std::endl;
// if (event_list[i].events == 1)
// 	std::cout << "This means EPOLLIN\n";
// else if (event_list[i].events == 4)
// 	std::cout << "This means EPOLLOUT\n";
// else if (event_list[i].events == 5)
// 	std::cout << "This means EPOLLIN | EPOLLOUT\n";
// else
// 	std::cout << RED BOLD "attention for this epoll event mrazzle \n";
// END OF TEST
