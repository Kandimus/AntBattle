import os
import sys
import json
import time
import socket
import select
from collections import deque

import Logger


try:
    from configparser import ConfigParser
except:
    from ConfigParser import ConfigParser

# to load Logger as module
sys.path.insert(0, os.getcwd())

log = Logger.Logger("Broker")

class ParamParser:
    """ this class handles parameter file reading and parsing """
    def __init__(self):
        self.host_sock_family_type      = socket.AF_INET # defaults to TCP\IP
        self.host_sock_type             = socket.SOCK_DGRAM
        self.host_ip                    = ""
        self.host_port                  = 0
        self.host_verify               = False
        self.srv_sock_family_type       = socket.AF_INET
        self.srv_sock_type              = socket.SOCK_DGRAM
        self.srv_ip                     = ""
        self.srv_port                   = 0
        self.engine_ip                  = ""
        self.engine_port                = 0
        self.client_num                 = 0
        self.ini_file_name              = "broker.ini"
        self.sentinal                   = 0x0000
        self.config                     = ConfigParser()

    def get_srv_port(self):
        return self.srv_port
    def get_srv_ip(self):
        return self.srv_ip
    def get_host_ip(self):
        return self.host_ip
    def get_host_port(self):
        return self.host_port
    def get_host_sock_type(self):
        return self.host_sock_type
    def get_host_sock_family_type(self):
        return self.host_sock_family_type
    def get_host_verify(self):
        return self.verify_message
    def get_srv_sock_family_type(self):
        return self.srv_sock_family_type
    def get_srv_sock_type(self):
        return self.srv_sock_type
    def get_client_num(self):
        return self.client_num
    def get_sentinal(self):
        return self.sentinal
    def get_engine_ip(self):
        return self.engine_ip
    def get_engine_port(self):
        return self.engine_port

    def get_params(self):
        """ Read ini file and populate parameters """
        try:
            if os.path.exists(self.ini_file_name):
                self.config.read(self.ini_file_name)
                self.host_sock_family_type = (socket.AF_INET if self.config.get("HOST", "socket_family") == "INET" \
                                       else socket.AF_UNIX)
                self.host_sock_type      = (socket.SOCK_DGRAM if self.config.get("HOST", "socket_type") == "UDP" \
                                           else socket.SOCK_STREAM)
                self.srv_sock_family_type = (socket.AF_INET if self.config.get("SERVER", "socket_family") == "INET" \
                                                  else socket.AF_UNIX)
                self.srv_sock_type = (socket.SOCK_DGRAM if self.config.get("SERVER", "socket_type") == "UDP" \
                                           else socket.SOCK_STREAM)

                self.host_ip        = self.config.get("HOST", "host_ip")
                self.host_port      = self.config.getint("HOST", "host_port")
                self.host_verify    = self.config.getboolean("HOST", "verify_message")
                self.srv_ip         = self.config.get("SERVER", "server_ip")
                self.srv_port       = self.config.getint("SERVER","server_port")
                self.clinet_num     = self.config.getint("SERVER", "client_number")
                self.engine_ip      = self.config.get("ENGINE", "engine_ip")
                self.engine_port    = self.config.getint("ENGINE", "engine_port")


                log.info("Reading parameters ...")
                log.info("host_ip = %s" % self.host_ip)
                log.info("host_port = %d" % self.host_port)
                log.info("host_sock_family_type = %s" % self.host_sock_family_type)
                log.info("host_sock_type = %s" % self.host_sock_type)
                log.info("host_verify = %s " % self.host_verify)
                log.info("server_sock_family_type = %s" % self.srv_sock_family_type)
                log.info("server_sock_type = %d" % self.srv_sock_type)
                log.info("server_ip = %s" % self.srv_ip)
                log.info("server_port = %d" % self.srv_port)
                log.info("engine_ip = %s" % self.engine_ip)
                log.info("engine_port = %d" % self.engine_port)

                return True

            else:
                log.error("[%s] file not found" % self.ini_file_name)
                return False
        except Exception as e:
            log.exception(e)
            return False

class UdpToEngine:
    """ this class handles connections with engine over tcp\ip socket """
    def __init__(self, prm):
        self.ip                 = prm.host_ip
        self.port               = prm.host_port
        self.sock_family_type   = prm.host_sock_family_type
        self.sock_type          = prm.host_sock_type
        self.engine_addr        = (prm.engine_ip, prm.engine_port)
        self.sock               = None
        self.msg                = None
        self.addr_info          = None
        self.bound              = False
        self.blocking           = False
        self.validate           = prm.host_verify
        self.buff_size          = 2048
        self.msg_counter        = 0
        self.recv_buff          = ()

    def do_connect(self)->bool:
        try:
            self.sock = socket.socket(self.sock_family_type, self.sock_type) #,socket.IPPROTO_UDP)
            self.sock.setblocking(self.blocking)
            self.sock.bind((self.ip, self.port))
            log.info("Engine Server bound to [%s:%d]\n" % (self.ip, self.port))
            self.bound = True

        except Exception as e:
            self.bound = False
            log.exception(e)
            return False
        finally:
            return True

    def do_read(self)->([],tuple):
        """ read socket for incoming messages and return bytes object """
        buff = []
        i = 0
        while True:
            try:
                rcv, addr_info = self.sock.recvfrom(self.buff_size)
                if rcv:
                    buff.append(rcv)
                    i += 1
                    log.debug("[{}] Received [{}] bytes".format(i, len(rcv)))
                    time.sleep(0.001)
                else:
                    break
            except BlockingIOError:
                log.debug("Exception BlockingIOError caught. No packets to receive.")
                break
            except Exception as e:
                log.exception(e)
                break

        if len(buff) > 0:
            log.debug("Received [{}] bytes from [{}:{}]".format(len(buff), addr_info[0], addr_info[1]))
            self.msg_counter += 1

        return buff, addr_info

    def do_write(self, msg, addr=None)->bool:
        """ write a given message to the engine """
        try:
            if not addr :
                self.sock.sendto(msg, self.engine_addr)
                log.info("Sent [%d] bytes to [%s:%d]" % (slen(msg), self.engine_addr[0], self.engine_addr[1]))
            elif type(addr) == tuple and len(addr) > 0:
                self.sock.sendto(msg, addr)
                log.info("Sent [%d] bytes to [%s:%d]" % (len(msg), addr[0], addr[1]))
            else:
                log.error("Write request with invalid address {}".format(addr))
                return False

        except Exception as e:
            log.exception(e)
            return False
        finally:
            return True

    def do_in_msg(self)->bytes:
        """ pre_process received message prior to sending to registered clients """
        # TODO
        buff, addr =  self.do_read()

        try:
            log.debug("Message with length [{}] in processing ".format(len(buff)))
            if self.validate and not self.do_validate(buff):
                log.info("Unable to verify received message [{}]".format(self.msg_counter))

        except Exception as e:
            log.exception(e)
            return None
        finally:
            return buff

    def do_validate(self, buff):
        """ verify received message on enclosing curly braces and double quotes """
        q = deque()
        msg = buff
        log.debug("{} [{}]".format(msg, len(msg)))
        j = 0
        invalid = 0
        total = 0

        for i, c in enumerate(msg):
            if c == 123:
                q.append(c)
            elif c == 125:
                q.pop()
                # means we read one json
                if len(q) == 0:
                    total += 1
                    try:
                        json.loads(msg[j:i + 1])
                    except Exception as e:
                        log.exception(e)
                        invalid += 1
                    finally:
                        j = i + 1
            else:
                continue

        log.debug("Validate: Total [{}] failed[{}]".format(total, invalid))
        return self.queue

    def do_out_msg(self, msg, addr):
        """ process incoming msgs from Engine """
        #TODO
        pass

    def do_done(self)->bool:
        """ close the socket """
        try:
            if self.sock.fileno() > 0:
                self.sock.close()
                log.info("Socket for Engine closed")
                self.sock = None
        except Exception as e:
            log.exception(e)
            return False
        finally:
            return True

class UdpToClients:
    """ this class handles connections with clients via socket """
    def __init__(self, prm):
        self.ip                 = prm.srv_ip
        self.port               = prm.srv_port
        self.sock_family_type   = prm.srv_sock_family_type
        self.sock_type          = prm.srv_sock_type
        self.clients            = []
        self.cmds               = {"register":self.do_register, "payload":self.do_payload}
        self.addr_info          = None
        self.ret                = None
        self.blocking           = False
        self.buff_size          = 2048

    def do_register(self, msg, addr):
        """ register a client for data receival """
        try:
            if addr[0] and addr[1]:
                log.info("Received request for registration from [%s:%d]" % (addr[0], addr[1]))

                # check that client is not already in the list
                if addr not in self.clients:
                    self.clients.append(addr)
                    log.info("Client [%s:%d] registered for data receival" % (addr[0], addr[1]))
                    if not self.do_write(b'{"status":"success", "message":"registration request completed"}', addr):
                        log.error("Unable to send to [{}] notification on successful registration".format(addr))

                else:
                    log.info("Clinet [%s:%d] already in the list" % (addr[0], addr[1]))

            else:
                log.error("Invalid address value received")
                return False

        except Exception as e:
            log.exception(s)
            return False
        finally:
            return True

    def do_payload(self, msg, addr):
        """ process request with payload key """
        #TODO
        pass

    def do_clt_req(self):
        """ process a request from client """
        #TODO
        msg, addr =  self.do_read()

        try:

            js = json.loads(msg)
            for key in js.keys():
                if key in self.cmds:
                    if not self.cmds[key](msg, addr):
                        log.error("Coudn't process request for [%s]" % key)

        except Exception as e:
            log.exception(e)
            return False
        finally:
            return True

    def do_connect(self):
        try:
            self.sock   = socket.socket(self.sock_family_type, self.sock_type)
            self.sock.setblocking(self.blocking)
            self.sock.bind((self.ip, self.port))
            log.info("Multicast server is bound to [%s:%d]" % (self.ip, self.port))

        except Exception as e:
            log.exception(e)
            return False
        finally:
            return True

    def do_read(self):

        ret = b''
        i = 0
        while True:
            try:
                msg, addr_info = self.sock.recvfrom(self.buff_size)
                if msg:
                    i += 1
                    log.debug("[{}] Received [{}] bytes".format(i, len(msg)))
                    ret += msg
                else:
                    break
            except BlockingIOError:
                log.debug("Exception BlockingIOError caught. No packets to receive.")
                break
            except Exception as e:
                log.exception(e)
                break

        if len(ret) > 0:
            log.debug("Received [{}] bytes in total from [{}:{}]".format(len(ret), addr_info[0], addr_info[1]))

        return ret, addr_info

    def do_write(self, msg, addr):

        try:
            # some processing for json ??
            self.sock.sendto(bytes(msg, encoding='utf-8'), addr)
            log.info("Sent [%d] bytes to [%s:%d] clients" % (len(msg), addr[0], addr[1]))

        except Exception as e:
            log.exception(e)
            return False
        finally:
            return True

    def do_send_msg(self, msg):
        """ send given message to all registered clients """
        try:
            failed = 0
            log.info("About to send [{}] messages to clients [{}]".format(len(msg), len(self.clients)))
            for i, clt in enumerate(self.clients):
                for m in msg:
                    log.debug("ABout to Send [{}]".format(m))
                    if not self.do_write(m,clt):
                        failed += 1
                        log.debug("Unable to send [{}] to [{}]".format(len(msg), clt))
            log.debug("[{}] total [{}] succeeded [{}] failed ".format(len(self.clients), len(self.clients)-failed, failed))

        except Exception as e:
            log.exception(e)
            return False
        finally:
            return True

    def do_done(self):
        """ send good bye to clients """
        end_msg = b'{message:"end of comunication"}'
        try:
            for clt in self.clients:
                self.sock.do_write(msg, clt)
            log.info("Shutdown multicast server ")

            self.sock.close()
            self.sock = None

        except Exception as e:
            log.exception(e)
            return False
        finally:
            return True

# global ParamParser instance

if __name__ == "__main__":

    log.info("Broker process started")

    # create instance of PramParser
    prm = ParamParser()
    if not prm.get_params():
        log.error("Error while reading prameters")
        sys.exit(-1)

    log.info("ParamParser instance started")

    # create instance of UdpToEngine
    engine_srv = UdpToEngine(prm)
    if not engine_srv.do_connect():
        log.error("Engine server could not connect")
        sys.exit(-1)
    log.info("Engine server instance started")

    # create instance of UdpToClients
    clients_srv = UdpToClients(prm)
    if not clients_srv.do_connect():
        log.error("Clients server could not connect")
        sys.exit(-1)
    log.info("Clients server instance started")

    # create epoll object
    epoll = select.epoll()

    # register both sockets for incoming events, Edge-triggered
    epoll.register(engine_srv.sock.fileno(), select.EPOLLIN | select.EPOLLET)
    epoll.register(clients_srv.sock.fileno(), select.EPOLLIN | select.EPOLLET)

    try:
        while True:
            # wait for incoming events
            events = epoll.poll(1)
            for fileno, event in events:
                if event & select.EPOLLIN:
                    if fileno == engine_srv.sock.fileno():
                        # do work
                        msg = engine_srv.do_in_msg()
                        if not msg:
                            log.error("Error while processing request ENGINE ...")
                        if not clients_srv.do_send_msg(msg):
                            log.error("Failed in attempt to send msg to clients ")

                    elif fileno == clients_srv.sock.fileno():
                        # do work
                        if not clients_srv.do_clt_req():
                            log.error("Error while processing request CLIENT ...")
                    else:
                        pass

    except Exception as e:
        log.exception(e)
        sys.exit(-1)
    finally:
        epoll.unregister(engine_srv.sock.fileno())
        epoll.unregister(clients_srv.sock.fileno())
        epoll.close()
        engine_srv.do_done()
        clients_srv.do_done()

