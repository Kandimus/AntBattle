import os
import re
import sys
import time
import json
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
        self.cases          = Cases()

    def do_devide(self, arr):
        """ devide given array into N parts so that each contains at least 1500 bytes """

        if arr and len(arr) > 1500:
            return len(arr) // 1500

        return 0


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
                for msg in self.cases.get_engine_cases_v2():
                    for _ in range(cnt):
                        try:
                            # send bytes object to socket
                            self.sock.sendto(bytes(msg, encoding='utf-8'), self.host_addr)
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

    def do_test_engine_v2(self, type=None, num=10):

        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.sock.bind(('127.0.0.1', 6598))
            log.debug("ENGINE Test. Socket created and bound at [127.0.0.1:6598]")
            # read test cases from a file
            if self.read_file:
                msg = self.cases.get_engine_cases_v2()
                # send bytes object to socket
                self.sock.sendto(bytes(msg[:1500], encoding='utf-8'), self.host_addr)
                self.sock.sendto(bytes(msg[1500:3000], encoding='utf-8'), self.host_addr)
                self.sock.sendto(bytes(msg[3000:4500], encoding='utf-8'), self.host_addr)
                self.sock.sendto(bytes(msg[4500:], encoding='utf-8'), self.host_addr)
                log.debug("[{}] bytes sent to [{}]".format(len(msg), self.host_addr[0], self.host_addr[1]))


        except Exception as e:
            log.exception(e)
            return False
        finally:
            return True

    def do_test_client_registraion(self, num=10):
        """ test Clients server for registration acceptance """
        log.debug("Test Clients Stated")
        count = 0
        msg = b'{"register":"Signup for data receival", "code":52}'

        for _ in range(num):
            try:
                sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                sock.connect(self.clients_addr)
                addr_info = sock.getsockname()
                log.debug("Local address [{}]".format(addr_info))

                sock.sendto(msg, self.clients_addr)
                buff, srv_addr = sock.recvfrom(4096)
                log.debug("[{}] bytes sent >>> [{}:{}] received [{}] from Server [{}]".format(len(msg), addr_info[0], addr_info[1], buff, srv_addr))
                sock.close()

            except Exception as e:
                count += 1
                log.exception(e)

            time.sleep(0.03)

        log.debug("Succeeded [{}] , Failed [{}] in Clients Test ".format(num-count, count))
        return True

    def do_test_engine_v3(self):
        """ get clinet cases from a file """

        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            sock.bind(('127.0.0.1', 6598))

            addr_info = sock.getsockname()
            log.debug("Local addr [{}]".format(addr_info))
            self.limit = 1500

            if self.read_file:
                cases =  self.cases.get_engine_cases_v3()

                # extract cases and send one by one each as # len(case)/1500 udp packets
                for j, case in enumerate(cases):
                    msg = []
                    log.debug("case [{}] ".format(len(case)) )
                    i = 0

                    while i < len(case):
                        msg.append(case[i:self.limit+i])
                        i += self.limit

                    log.debug("[{}] [{}]".format(len(msg), type(msg)))

                    for m in msg:
                        log.debug("about to send [{}]".format(m))
                        sock.sendto(bytes(m, encoding='utf-8'), self.host_addr)
                        log.debug("[{}] bytes sent to [{}]".format(len(m), self.clients_addr))
                    log.debug("Sent [{}]  [{}]".format(j, len(msg)))

                    time.sleep(2)

            sock.close()
            log.debug("Finished testing clients v2 [step 2]")

        except Exception as e:
            log.exception(e)
            return False
        finally:

            return True


class Cases:
    """ simple class to read test cases from a file and return iterable object [msg, count] """
    def __init__(self, file = "Test_cases.ini"):
        self.engine_cases = []
        self.client_cases = []
        self.file_name    = file
        self.re_engine    = re.compile("\[ENGINE\]")
        self.re_client    = re.compile("\[CLIENTS\]")


    def get_engine_cases(self):
        """ Get test cases for Engine and Clients servers """
        try:
            with open(self.file_name, "r") as f:
                line = f.readline().strip('\n')
                # check section presence
                if not self.re_engine.match(line):
                    log.error("File format error: [ENGINE] section missing")
                    return False
                # loop through all lines and append non-empty lines
                while line != '' and not self.re_client.match(line):
                    # blank line, skip it
                    line = f.readline().strip('\n')

                    # get case and its number
                    case =  line.split(' ')
                    log.debug("<<{}>>".format(case))
                    if len(case) == 2:
                        self.engine_cases.append([case[0],int(case[1])])
                log.debug("Data read from [{}]".format(self.file_name))

        except Exception as e:
            log.exception(e)
            return None
        finally:
            return self.engine_cases

    def get_engine_cases_v2(self):
        """ Get test cases for Engine and Clients servers """
        try:
            engine_cases = ''
            with open(self.file_name, "r") as f:
                line = f.readline().strip('\n')
                # check section presence
                if not self.re_engine.match(line):
                    log.error("File format error: [ENGINE] section missing")
                    return False
                # loop through all lines and append non-empty lines
                while line != '' and not self.re_client.match(line):
                    # blank line, skip it
                    line = f.readline().strip('\n')

                    if line == '\n':
                        continue

                    engine_cases += line

                log.debug("Data read from [{}]".format(self.file_name))

        except Exception as e:
            log.exception(e)
            return None
        finally:
            return engine_cases

    def get_engine_cases_v3(self):

        try:
            with open(self.file_name, 'r') as f:
                lines = f.readlines()
                msg = ''

                for line in lines:
                    if line == '\n' or line == '':
                        self.client_cases.append(msg)
                        msg = ''

                    msg += line.strip('\n')

                log.debug("[{}] added frames to cases".format(len(self.client_cases)))

        except Exception as e:
            log.exception(e)
            return []
        finally:
            return self.client_cases

if __name__ == "__main__":


    log.info("Test main started ... ")
    ts = Tester(read_file=True)

    if not ts.get_configs():
        log.error("Failed to load configs ...")
        sys.exit(-1)

    if not ts.do_test_engine_v3():
        log.error("Failed to test {}".format(ts.srv_name))

    #if not ts.do_test_client_v2():
    #    log.error("Failed to test {}".format(ts.srv_name))

    log.info("Test ended ...")
    sys.exit(0)