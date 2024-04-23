# AllSafe-Chat-Application

*Introduction*

AllSafe is a specialized client-server chat application designed specifically for internal communications within organizations. This application facilitates up to 200 employees to engage securely with IT administrators or other designated personnel in real-time.
Core functionality of AllSafe is to provide a private and secure platform where organizational communication can occur without the risk of breaking confidentiality or access to unauthorized personnel.
Developed with a focus on security and scalability, AllSafe incorporates Vigenère encryption standards and secure authentication processes to ensure that every message transmitted remains confidential and secure.
Application is structured to handle high volumes of simultaneous connections, making it exceptionally suitable for large organizations where continuous and multiple communications are common.
One of the key features of AllSafe is its ability to seamlessly integrate new clients into ongoing chats without compromising the integrity and privacy of historical communications. This ensures that sensitive information remains protected even as the network of users grows.

========================================================================================================================================================================================================


*Usage Instructions*


To begin using the AllSafe chat application, both IT administrators and employees must follow a series of steps to ensure the application is set up and functioning correctly. Here is a detailed guide:
1.	Download the Source Code
•	Navigate to the GitHub repository linked at the end of this report.
•	Download the source files for both the client and server applications.
2.	 Compile the Source Code
•	Open a terminal window.
•	For the client application, compile the source using the following command:
 g++ -o Client-CLI AllSafe(Client-Side.cpp)
•	For the server application, used by IT administrators, compile the source using:
g++ -o Server-CLI AllSafe(Server-Side.cpp)
3.	Launch the Server
•	IT administrators should launch the server application by running:
./Server-CLI
This will start the server, which will then listen for incoming client connections.
4.	Run the Client Application
•	Employees should open another terminal window and start the client application with:
./Client-CLI
•	Upon launching, the client console will display a welcome message along with an options menu, as displayed below: 


![image](https://github.com/YS2100344/AllSafe-Chat-Application/assets/115540431/d418da32-dc1d-4e9c-85fd-ec5044e99f70)





5.	Account Management
•	Sign Up:
Select option 1 for sign-up.
Enter the desired username and password when prompted.
After signing up, a message confirming successful registration will be displayed.
Snippet:

![image](https://github.com/YS2100344/AllSafe-Chat-Application/assets/115540431/3a18d39f-3267-4c7f-91bb-032f2fe0a533)


•	Log In:
Run ./Client-CLI again to log in.
Select option 2 and provide your username and password.

Upon successful authentication, the client will display:



![image](https://github.com/YS2100344/AllSafe-Chat-Application/assets/115540431/d2bef26a-3d1e-4ce8-844c-729ff347a6c2)






•	Concurrently, the server console will log:


![image](https://github.com/YS2100344/AllSafe-Chat-Application/assets/115540431/a6ad3961-0b46-488e-929d-403524110225)





6.	 Starting a Chat Session

•	After logging in, users can immediately begin chatting with the server.
•	Messages typed in the client console are sent to the server and responses are displayed in real-time.


This sequence of steps ensures that both IT administrators and employees can effectively use AllSafe for secure communications within the organization. The application is designed to be user-friendly and requires minimal technical knowledge to operate, facilitating efficient and secure interactions.

