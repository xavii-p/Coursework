"""
THIS  CODE  IS  MY  OWN  WORK,  IT  WAS  WRITTEN 
WITHOUT  CONSULTING  CODE  WRITTEN  BY  OTHER  STUDENTS.  Xavier_Pierce
"""

import socket
import sys

proxyHost = "localhost"

if len(sys.argv) != 2:
    print("A port number must be specified.")
    sys.exit(1)

proxyPort = int(sys.argv[1])
if proxyPort < 1024:
    print("Port number must be greater than 1024")
    sys.exit(1)
    

# create server-like socket for listening to connections
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# reserve the port number specified in command line
serversocket.bind((proxyHost, proxyPort))

# listen for connections
serversocket.listen()

# loop to get all requests
while True:
        # will block until connection is established
        (clientSocket, clientAddr) = serversocket.accept()
        # new socket to communicate with client
        with clientSocket:
            while True:
                data = clientSocket.recv(1024)
                
                # end while when no more data being sent from client
                if not data:
                    break
                
                # parse data to ensure GET method
                try:
                    """"
                    request = data.decode("utf-8")
                    requestLines = request.split("\r\n")
                    # print(requestLines)
                    method, uri, version = requestLines[0].split(" ")
                    print(f"{method} + {uri}")
                    uri = uri.replace("/", "")
                    # print(f"{uri}")
                    if method != "GET":
                        errorMessage = "Proxy error: Method not implemented"
                        response = (
                            "HTTP/1.0 405 Method Not Allowed\r\n"
                            "Content-Type: text/plain; charset=UTF-8\r\n"
                            f"Content-Length: {len(errorMessage)}\r\n"
                            "\r\n"
                            f"{errorMessage}"
                            )
                        clientSocket.sendall(response.encode("utf-8"))
                        print("Sent")
                    """
                    # transform from bytes to string
                    request = data.decode("utf-8")
                    headers = request.split("\r\n")
                    ## print(f"{headers}\n")
                    ## print("\n")
                    firstLine = headers[0]
                    ## print(f"{firstLine}\n")
                    ## print("\n")
                    method, uri, version = firstLine.split(" ")
                    ## print(f"{version}\n")
                    ## print("\n")
                    # print(f"{uri}")

                    if method != "GET":
                        errorMessage = "Proxy error: Method not implemented"
                        response = (
                            "HTTP/1.0 405 Method Not Allowed\r\n"
                            "Content-Type: text/plain; charset=UTF-8\r\n"
                            f"Content-Length: {len(errorMessage)}\r\n"
                            "\r\n"
                            f"{errorMessage}"
                            )
                        clientSocket.sendall(response.encode("utf-8"))
                        # print("Error sent")

                    else:

                        hostName = uri.split("/")[1]

                        ## print(f"{hostName}\n")
                        ## print("\n")

                        # we want the root of the hostName we are connecting to --> if hostName is example.com and path is /home
                        # we end up requesting for example.com/home
                        path = "/" + "/".join(uri.split("/")[2:])
                        ## print(f"{path}\n")
                        ## print("\n")

                        # use HTTP 1.0 to communicate w/ web server
                        modifiedRequest = f"{method} {path} HTTP/1.0\r\n"
                        modifiedHeaders = f"Host: {hostName}\r\n\r\n"
                        ## print(f"{modifiedRequest}\n")
                        ## print("\n")

                except Exception as e:
                    print(f"Error parsing request: {e}")
                    break

                # create client-like socket to deliver data to destination

                clientLikeSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                # connect to the specified URI on port 80 (HTTP requests)
                try:
                    clientLikeSocket.connect((hostName, 80))
                    clientLikeSocket.sendall((modifiedRequest + modifiedHeaders).encode("utf-8"))

                    response = b""
                    while True:
                        # gather the response
                        data = clientLikeSocket.recv(4096)
                        
                        # end while when no more data being sent from client
                        if not data:
                            break

                        response += data

                        # send data back to browser
                        # print("Response from web received")
                        clientSocket.sendall(response)
                        # print("Response sent to browser")
                except Exception as e:
                    print(f"Error connecting to web server to send request: {e}")
                    break

                finally:
                    # close web server connection
                    clientLikeSocket.close()