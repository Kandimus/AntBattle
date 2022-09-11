Python directory contains files necessary to run Broker and Displayer process, which are: 
Broker.py - broker process which receives data frames from Engine and (process if necessary) sends to all registered clients.
clients send registration request to Broker for data receival prior to receiving any data.

Displayer.py - client process which regiters for data receival to Broker and receives data frames to display them
Data frames are transmitted via UDP utilizing plan text json format.

Tester.py - process to test Broker (support for Displayer to be added )


