#!/usr/bin/env python2
import os, sys
sys.path.append(os.path.dirname(os.path.realpath(__file__)) + "/lib")

from datagram import *
from mdconnection import *

HELP = [("help", "get these help commands"),
        ("exit | quit", "Quit the query session."),
        ("connect", "connect to OpenOTP server at IP address")]

class Queryer(object):
    def __init__(self):
        self._prompt_symbol = "otp>"
        self._location = ""

    def ask_command(self):
        try:
            command = raw_input(self._prompt_string())
        except EOFError:
            print "\nRecieved EOF"
            self._exit()
        self.query(command)

    def query(self, command):
        if command == "exit" or command == "quit":
            self._exit()

        # Help lists help commands
        if command == "help":
            for entry in HELP:
                print "  %s - %s" % (entry[0], entry[1])

        # Connect to localhost by default
        if command == "connect":
            self._connect("localhost")

        # Connect to an IP address or 'localhost'
        if command[:len("connect ")] == "connect ":
            if len(command) > len("connect "):
                self._connect(command[len("connect "):])

    def _connect(self, location):
        if len(self._location) == 0:
            self._location = location
            self._connection = MDConnection()
            message = self._connection.connect(self._location)
            if len(message) > 0:
                print "Connection to " + location + " failed:"
                print " "*8 + message
                self._location = ""
        else:
            print "Not connecting"
            print "Previous connection exists to: " + self._location

    def _exit(self):
        if len(self._location) > 0:
            self._connection.close()
            print "Closed connection to " + self._location
        sys.exit(0)

    def _prompt_string(self):
        loc = ""
        if len(self._location) > 0:
            loc += self._location + " "
        return loc + self._prompt_symbol + " "

if __name__ == "__main__":
    queryer = Queryer()
    while True:
        queryer.ask_command()