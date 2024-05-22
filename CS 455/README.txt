To run the code in web-proxy.py, you will need the following:

ENVIRONMENTS:
-- Python 3.12.2

LIBRARIES:  (each library is included as a standard libray in Python and requires no installation aside from normal Python)
-- Python Socket library
-- Python System library


To compile & run the web-proxy, navigate to the directory containing the script and use the following syntax:
    python3.12 web-proxy.py <port_number>

    -- The parameter port_number should be a port number greater than 1024. Any port number below this is reserved
    for specific functions on Linux/MacOS systems. Ports below 1024 are considered "privileged" ports, 
    and binding to these ports requires superuser privileges.

    -- On Windows the distinction between privileged and unprivileged ports is not enforced, 
    thus there may not be an error generated for a port number below 1024, however the port may still be reserved.
