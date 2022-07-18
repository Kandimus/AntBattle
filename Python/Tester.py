import os
import re
import sys
import time
import socket
import Logger



try:
    from configparser import ConfigParser
except:
    from ConfigParser import ConfigParser


log = Logger.Logger("Test")

class Tester:
    """ simple tester class to test udp servers on performance and robustness """
    def __init__(self, name="TESTER", read_file=False):
        self.srv_name       = name
        self.host_addr      = None
        self.clients_addr   = None
        self.msg            = b'{"cell":{"ant":{"health":100.0,"satiety":100.0,"player":0,"type":"worker"},"type":"ant","x":0,"y":0}}'
        self.read_file      = read_file
        self.config_file    = "broker.ini"
        self.config         = ConfigParser()

    def get_configs(self):
        """ read Broker.ini for engine and clients server addresses """
        try:
            if os.path.exists(self.config_file):
                self.config.read(self.config_file)
                self.host_addr      = (self.config.get("HOST", "host_ip"), self.config.getint("HOST", "host_port"))
                self.clients_addr   = (self.config.get("SERVER", "server_ip"), self.config.getint("SERVER", "server_port"))
                log.debug("ENGINE and SERVER info loaded ... ")

        except Exception as e:
            log.exception(e)
            return False
        finally:
            return True

    def do_test_engine(self, type=None , num = 10):

        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.sock.bind(('127.0.0.1', 6598))
            log.debug("ENGINE Test. Socket created and bound at [127.0.0.1:6598]")
            # read test cases from a file
            if self.read_file:
                cases = Cases()

                for msg, cnt in cases.get():
                    for _ in range(cnt):
                        try:
                            # verify json object first
                            js = json.loads(msg)
                            # send bytes object to socket
                            self.sock.sendto(bytes(msg), self.host_addr)
                            log.debug("[{}] bytes sent to [{}]".format(len(msg), self.host_addr[0], self.host_addr[1]))

                        except ValueError as ve:
                            log.error("[{}] has invalid format. Skip it".format(msg))
                        except Exception as e:
                            log.exception(e)

            else:
                # do tests num times for slef.msg
                try:
                    for _ in range(num):
                        self.sock.sendto(self.msg, self.host_addr)
                        log.debug("[{}] bytes sent to [{}]".format(len(self.msg), self.host_addr[0], self.host_addr[1]))
                except Exception as e:
                    log.exception(e)
                finally:
                    log.info("Sent [{}] messages to [{}:{}]".format(num, self.host_addr[0], self.host_addr[1]))

        except Exception as e:
            log.exception(e)
            return False
        finally:
            return True

    def do_test_clients(self, num=10):
        """ test Clients server for registration acceptance """
        log.debug("Test Clients Stated")
        count = 0
        for _ in range(num):
            try:
                sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                sock.connect(self.clients_addr)
                addr_info = sock.getsockname()
                log.debug("Local address [{}]".format(addr_info))
                #sock.bind(addr_info)
                sock.sendto(b'{"register":"Signup for data receival"}', self.clients_addr)
                buff, srv_addr = sock.recvfrom(1024)
                log.debug("[{}:{}] received [{}] from Server [{}]".format(addr_info[0], addr_info[1], buff, srv_addr))
                sock.close()

            except Exception as e:
                count += 1
                log.exception(e)

            time.sleep(0.03)

        log.debug("Succeeded [{}] , Failed [{}] in Clients Test ".format(num-count, count))
        return True

class Cases:
    """ simple class to read test cases from a file and return iterable object [msg, count] """
    def __init__(self, file = "Test_cases.ini"):
        self.engine_cases = []
        self.file_name    = file
        self.re_engine    = re.compile("\[ENGINE\]")


    def get(self):
        """ Get test cases for Engine and Clients servers """
        try:
            with open(self.file_name, "r") as f:
                lines = f.readlines()
                # check section presence
                if not self.re_engine.match(lines[0]):
                    log.error("File format error: [ENGINE] section missing")
                    return False
                # loop through all lines and append non-empty lines
                for line in lines[1:]:
                    # blank line, skip it
                    if line == "\n":
                        continue
                    # get case and its number
                    case, num = line.split(":")
                    self.engine_cases.append([case,int(num)])
                log.debug("[{}] lines read  from [{}]".format(len(lines), self.file_name))

        except Exception as e:
            log.exception(e)
            return None
        finally:
            return self.engine_cases



if __name__ == "__main__":


    log.info("Test main started ... ")
    ts = Tester()

    if not ts.get_configs():
        log.error("Failed to load configs ...")
        sys.exit(-1)

    if not ts.do_test_engine():
        log.error("Failed to test {}".format_map(ts.srv_name))

    if not ts.do_test_clients():
        log.error("Failed to test {}".format_map(ts.srv_name))

    log.info("Test ended ...")
    sys.exit(0)