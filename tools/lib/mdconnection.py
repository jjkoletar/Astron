import socket
__all__ = ['MDConnection']

class MDConnection(object):
    def __init__(self, sock):
        self.s = sock
        self.s.settimeout(0.1)

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
        self.s.close()

    def flush(self):
        while self._read(): pass

    def read(self):
        return Datagram(self._read())
