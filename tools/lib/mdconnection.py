import errno
from socket import socket, AF_INET, SOCK_STREAM
from socket import error as socket_error

__all__ = ['MDConnection']

class MDConnection(object):
    def connect(self, location="localhost"):
        if location == "localhost":
            location = '127.0.0.1'
        self._sock = socket(AF_INET, SOCK_STREAM)
        try:
            self._sock.connect((location, 7199))
        except socket_error as serr:
            if serr.errno == errno.ECONNREFUSED:
                return "Connection refused"
        self._sock.settimeout(0.1)
        return "" # Connected successfuly

    def send(self, datagram):
        data = datagram.get_data()
        msg = struct.pack('<H', len(data)) + data
        self.s.send(msg)

    def _read(self):
        length = 2
        result = ''
        while len(result) < length:
            data = self.s.recv(length - len(result))
            if data == '':
                raise EOFError('Remote socket closed connection')
            result += data
        length = struct.unpack('<H', result)[0]

        result = ''
        while len(result) < length:
            data = self.s.recv(length - len(result))
            if data == '':
                    raise EOFError('Remote socket closed connection')
            result += data

        return result

    def close(self):
        self._sock.close()

    def flush(self):
        while self._read(): pass

    def read(self):
        return Datagram(self._read())

    def set_timeout(self, timeout):
        self._sock.settimeout(timeout)
