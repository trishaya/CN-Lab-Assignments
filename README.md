# CN-Lab-Assignments
# CN Lab – Assignment 1 
# Exercise: 
Write a program to run TCP client and server socket programs where client first 
says “Hi” and in response server says “Hello”. 
# SS
<img width="1620" height="614" alt="1" src="https://github.com/user-attachments/assets/a7e00de3-ae7b-4fbf-aba7-11376eaed8be" />  
# CN Lab – Assignment 2 
# Exercise: 
Write a program using TCP socket to implement the following:  
i. Server maintains records of fruits in the format: fruit-name, quantity Last-sold, 
(server timestamp),  
ii. Multiple client purchase the fruits one at a time, 
iii. The fruit quantity is updated each time any fruit is sold,  
iv. Send regret message to a client if therequested quantityof the fruit is not 
available.  
v. Display the customer ids <IP, port> who has done transactions already. This 
list should be updated in the server every time a transaction occurs. 
vi. The total number of unique customers who did some transaction will be 
displayed to the customer every time. 
# SS
<img width="1064" height="397" alt="image" src="https://github.com/user-attachments/assets/7ce9998a-7a7f-4b3a-88e6-23fc7e8643c0" />

# CN Lab – Assignment 3  
# Exercise: 
Redo assignment 2 using UDP socket.  
Steps/ Hints: (if any) 
Same as given in Assignment 1. 
# SS
   <img width="1099" height="397" alt="image" src="https://github.com/user-attachments/assets/7a344fb7-1f68-499a-b57d-a8877a413ff7" />

# Assignment 4: 
# Exercise: 
Install wireshark in a VM (Virtual Machine) environment. Draw a time diagram to 
show the steps in the protocols recorded in the captured file (saved in the .pcap file 
of wireshark) during a PING operation. List the L2, L3, L4 header fields that can be 
extracted from the .pcap file.  
Steps/ Hints: (if any) 
1. Install wireshark in your linux vm with mininet, 
2. Start capturing at any interface, 
3. Ping any two hosts, 
4. Analyze. 
# SS
   <img width="1721" height="658" alt="image" src="https://github.com/user-attachments/assets/c6078acc-5787-4f83-9df8-b500331743a8" />
<img width="1596" height="656" alt="image" src="https://github.com/user-attachments/assets/5e463219-8ee1-485f-8397-9d6f0beb1a48" />

# Assignment 5 
# Exercise: 
Learn and use maximum number of packet generation tools.  
Steps/ Hints: (if any) 
1. Install Iperf, D-ITG etc. and send traffic among mininet hosts. 
# SS
<img width="486" height="663" alt="image" src="https://github.com/user-attachments/assets/c0e9ce2b-1cc3-4e86-8799-fd9c11bad6c4" />
<img width="560" height="756" alt="image" src="https://github.com/user-attachments/assets/740393f4-ad35-4526-b5ad-806148441feb" />


# Assignment 6 
# Exercise: 
Develop a simple C based network simulator to analyze TCP traffic.  
Steps/ Hints: (if any) 
1. Use TCP/IP based C libraries including socket to listen to the incoming packets at 
the Ethernet port. 
2. Extract header and data of the incoming packets. 
# SS
<img width="671" height="698" alt="image" src="https://github.com/user-attachments/assets/2e848d9b-4351-43ab-ab78-904446bd6f70" />

# Assignment 7: 
# Exercise: 
Write UDP client server socket program where client sends one/two number(s) 
(integer or floating point) to server and a scientific calculator operation (like 
sin,cos,*,/, inv etc.) and server responds with the result after evaluating the value of 
operation as sent by the client. Server will maintain a scientific calculator. Detect in 
the mininet hosts with wireshark if there is any packet loss? Show it to the TA. 
# SS
<img width="495" height="510" alt="image" src="https://github.com/user-attachments/assets/5a808d5e-a685-4414-817f-68a895365c7b" />

# Assignment 8: 
# Exercise: 
Write a program in C using thread library and TCP sockets to build a chat server 
which enable clients communicating to each other through the chat server. Message 
logs must be maintained in the server in a text file. Each client will see the 
conversations in real time. Clients must handled by a server thread. (Keep it like a 
group chatbox) 
# SS
<img width="876" height="669" alt="image" src="https://github.com/user-attachments/assets/738f16f0-8c7b-4ee8-8753-83b138c5615b" />

# Assignment 9:  
# Exercise: 
Write a client server socket program in TCP for uploading and downloading files 
between two different hosts. Also calculate the transfer time in both the cases. 
# SS
<img width="1028" height="352" alt="image" src="https://github.com/user-attachments/assets/6ea6f240-5b3e-4534-b84b-5c9bf40bbb41" />

# Assignment 10: 
# Exercise: 
Write two C programs using raw socket to send i. TCP packet where TCP payload 
will contain your roll number. ii. ICMP time stamp messages towards a target IP.  
# SS
<img width="1845" height="442" alt="image" src="https://github.com/user-attachments/assets/9ee327af-ac85-45ce-9efd-0e5c9ea9de2e" />

# Assignment 11: 
# Exercise: 
Write a RAW socket program to generate TCP SYN flood based DDoS attack 
towards an IP address. Take four mininet hosts as agent devices.  
# SS
<img width="1167" height="323" alt="image" src="https://github.com/user-attachments/assets/fee3eeea-6499-405f-9b49-15e24ff77deb" />

# Assignment 12:  
# Exercise: 
Do the same attack as given in assignment no. 11 with ICMP packets using RAW 
socket.
# SS
<img width="1526" height="717" alt="image" src="https://github.com/user-attachments/assets/32ebb9da-8313-469f-8cd3-919bed1a4670" />

# Assignment 13: 
# Exercise: 
Create a binary tree topology with 7 switches in mininet. Capture packets at the root 
switch. Write a C program to extract the headers and draw a time diagram to show 
the protocols displayed in the captured file (save the .pcap/.pcapng file of 
wireshark/tshark) during a PING operation. List the L2, L3, L4 protocols that can be 
extracted from the .pcap/.pcapng file. 
# SS
<img width="1853" height="415" alt="image" src="https://github.com/user-attachments/assets/4a968bb4-53ca-4127-b580-cb9c1ca8b17e" />

# Assignment 14:  
# Exercise: 
Create a custom leaf-spine topology in mininet using python which can be scaled 
with increasing switch radix. 
# SS
<img width="805" height="574" alt="image" src="https://github.com/user-attachments/assets/c0c6c968-b3b2-4536-b4ee-a943e9982736" />

1. Understanding mininet API. 
2. Learning to perform experiments in virtual networks.
