#!/usr/bin/python
import os, sys
sys.path.append(os.path.dirname(os.path.realpath(__file__)) + "/lib")

from datagram import *
from mdconnection import *

HELP = [("help", "get these help commands"),
        ("exit | quit", "Quit the query session.")]

class Queryer(object):
    def __init__(self):
        self._prompt_symbol = "otp>"
        self._location = ""

    def ask_command(self):
        try:
            command = raw_input(self._prompt_string())
        except EOFError:
            print "\nRecieved EOF"
            sys.exit(0)

        if command == "exit" or command == "quit":
            sys.exit(0)
        if command == "help":
            for entry in HELP:
                print "  %s - %s" % (entry[0], entry[1])

    def _prompt_string(self):
        loc = ""
        if len(self._location) > 0:
            loc += self._location + " "
        return loc + self._prompt_symbol + " "

if __name__ == "__main__":
    queryer = Queryer()
    while True:
        queryer.ask_command()