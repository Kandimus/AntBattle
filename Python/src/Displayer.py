import os
import sys
import math
import time
import json
import socket
import curses
import random
from collections import deque

sys.path.insert(0, os.getcwd())
import Logger

log = Logger.Logger("Displayer")

class UdpClient:
    """ this class registers and receives frames via UDP socket """
    def __init__(self, host_ip=None, host_port=None, srv_ip=None, srv_port=None):
        try:
            self.sock          = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        except Exception as e:
            log.exception(e)
            self.sock          = None

        self.host_addr         = (host_ip, host_port)
        self.addr              = (srv_ip, srv_port)
        self.buff_size         = 2048
        self.blocking          = False
        # to hold frames if the arrive faster than we process at a time
        self.queue             = deque()

    def do_connect(self)->bool:
        """ connect to the server for communication """
        try:
            if not self.sock:
                # try again
                self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
            self.sock.bind(self.addr)
            self.sock.setblocking(self.blocking)
            log.info("Bound to [{}:{}]".format(self.addr[0], self.addr[1]))

        except Exception as e:
            log.exception(e)
            return False
        finally:
            return True

    def do_get_ip(self)->str:
        """ tries to retrive ip address. not reliable """
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            sock.settimeout(0)
            sock.connect(('8.8.8.8', 80))
            ip = sock.getsockname()[0]
        except Exception as e:
            ip = '127.0.0.1'
        finally:
            sock.close()
            return ip

    def do_register(self)->bool:
        """ send registration request to Broker for frame receival """
        try:
            req_msg = '{{"register":"{}:{}"}}'.format(self.addr[0],self.addr[1])
            if not self.do_write(req_msg):
                log.error("Couldn't register to [{}]".format(len(req_msg), self.host_addr))
            else:
                log.debug("Sent register request to [{}:{}]".format(self.host_addr[0], self.host_addr[1]))

        except Exception as e:
            log.exception(e)
            return False
        finally:
            return True

    def do_write(self, msg)->bool:
        try:
            #TODO
            # case when msg.__len__ > 1024
            self.sock.sendto(bytes(msg, encoding="utf-8"), self.host_addr)
            log.debug("[{}] bytes sent to [{}]".format(len(msg), self.host_addr))

        except Exception as e:
            log.exception(e)
            return False
        finally:
            return True

    def do_read(self)->bytes:
        """ read bytes from the socket and return bytes object """
        buff = b''

        while True:

            try:
                rcv, addr = self.sock.recvfrom(self.buff_size)
                if rcv:
                    buff += rcv
                    log.debug("Packet [{}] bytes received".format(len(rcv)))
                else:
                    break

            except BlockingIOError :
                log.debug("Caught BlockingIOError. Ignoring. ")
                break
            except Exception as e:
                log.exception(e)
                break
        if len(buff) > 0:
            log.debug("Received [{}] bytes from [{}]".format(len(buff), addr))
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
                        # write in FIFO for further processing
                        log.debug(">>>>{}".format(msg[j:i+1]))
                        self.queue.append(json.loads(msg[j:i + 1]))
                    except Exception as e:
                        log.exception(e)
                        invalid += 1
                    finally:
                        j = i + 1
            else:
                continue

        log.debug("Validate: Total [{}] failed[{}]".format(total, invalid))
        return self.queue

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

class Window:
    """ main window class to display ongoing game """
    def __init__(self):

        # get main window
        self.stdscr = curses.initscr()
        # clear all in main window first
        self.stdscr.clear()
        self.stdscr.keypad(True)
        # windows related variables
        self.game_win_y = 0
        self.game_win_x = 0
        self.game_win_hight = curses.LINES - 2
        self.game_win_width = curses.COLS - 32
        self.count  = 0
        # latest state of the map
        self.map    = {}
        if curses.has_colors():
           curses.start_color()
        curses.use_default_colors()
        # set color pairs
        curses.init_pair(1, curses.COLOR_RED, curses.COLOR_BLACK)
        curses.init_pair(2, curses.COLOR_GREEN, curses.COLOR_BLACK)
        curses.init_pair(3, curses.COLOR_WHITE, curses.COLOR_BLACK)
        curses.init_pair(4, curses.COLOR_YELLOW, curses.COLOR_BLACK)
        curses.init_pair(5, curses.COLOR_MAGENTA, curses.COLOR_BLACK)
        #properly set up terminal
        curses.noecho()
        curses.cbreak()
        curses.curs_set(0)

        self.game_info = {'worker':'W', 'soldier':'S', 'queen':'Q', 'turn':0, 'food':'@', 'stone':'#'}
        self.color_type = {'stone':3, 'food':4}

        self.state_ok = True
        self.game_win = None
        self.info_win = None

        try:
            # game playground area
            self.game_win = curses.newwin(curses.LINES-2,curses.COLS-2, 1, 1)
            # set getch() non-blocking
            self.game_win.nodelay(True)
        except Exception as e:
            log.exception(e)
            self.state_ok = False

    def get_usr_key(self):
        """ read user input for exit key """
        c = self.game_win.getch()
        if c == ord('q') or c == ord('Q'):
            log.debug("User requested to quit the game [{}]".format(c))
            return True

        return False

    def do_init_frame(self):
        """ set borders and other info on the window """
        #self.stdscr.border('|','|','-','-','@','@','@','@')
        self.stdscr.addstr(curses.LINES - 1, 0, " Press [Q|q] to exit")
        self.stdscr.chgat(curses.LINES - 1, 8, 1, curses.A_BOLD | curses.color_pair(5))
        self.stdscr.chgat(curses.LINES - 1, 10, 1, curses.A_BOLD | curses.color_pair(5))

        # set main window boarders
        self.game_win.border('#','#','#','#','#','#','#','#')
        # set game info
        self.game_win.addstr(4, self.info_win_x, " Player A: ", curses.color_pair(1))
        self.game_win.addstr(6, self.info_win_x, "  Ants total: ")
        self.game_win.addstr(8, self.info_win_x, "  Soldier: ")
        self.game_win.addstr(10, self.info_win_x, "  Worker: ")
        self.game_win.addstr(12, self.info_win_x, "  Food: ")

        self.game_win.addstr(14, self.info_win_x, " Player B: ", curses.color_pair(2))
        self.game_win.addstr(16, self.info_win_x, "  Ants total: ")
        self.game_win.addstr(18, self.info_win_x, "  Soldier: ")
        self.game_win.addstr(20, self.info_win_x, "  Worker: ")
        self.game_win.addstr(22, self.info_win_x, "  Food: ")

        self.game_win.addstr(24, self.info_win_x, "LINES {}".format(curses.LINES))
        self.game_win.addstr(25, self.info_win_x, "COLOUMNS {}".format(curses.COLS))


    def do_show_win(self)->bool:
        """ refresh window """
        if self.state_ok:

            self.game_win.addstr(3, self.info_win_x, " Turn: # {}".format(self.game_info['turn']))
            # refresh windows in order. This says put window in to the line for refresh
            self.stdscr.noutrefresh()
            self.game_win.noutrefresh()
            # actual refresh
            curses.doupdate()
            return True
        else:
            log.debug("State of window is [{}]".format(self.state_ok))
            return False

    def do_process_frame(self, data):
        """ extract coordinate level data """
        log.info("Enter do_process_frame() ")
        try:
            # extract turn number from the queue
            self.game_info['turn'] = int(data.popleft()['turn'])
            self.count += 1
        except IndexError:
            log.error("data queue is empty")
            return False
        except KeyError:
            log.error("no such key: turn ")
            return False
        except Exception as e:
            log.exception(e)
            return False
        log.debug("queue length [{}]".format(len(data)))

        while len(data) > 0:

            try:
                # process cells now
                entry = data.popleft()['cell']
                log.debug(">>>{}".format(entry))
                # get the coordinates of a cell
                point = (int(entry['y']), int(entry['x']))
                # reinitiate cell for information update
                self.map[point] = {}

                self.map[point]['type'] = entry['type']

                # extract cell specific data
                if entry['type'] == 'food':
                    self.map[point]['amount'] = entry['amount']
                elif entry['type'] == 'ant':
                    self.map[point]['ant'] = entry['ant']
                else:
                    continue

            except IndexError:
                log.error("data queue is empty")
                break
            except KeyError:
                log.error("no such key: ")
                break
            except Exception as e:
                log.exception(e)
                break

        log.debug("Exit do_process_frame(). [{}] records proccessed".format(len(self.map)))
        return True

    def do_set_frame(self):
        """ set map's coordinate info and refresh window """
        log.info("Enter do_set_frame() ")
        # iterate through the map and set cells
        for key, val in self.map.items():
            log.debug("[{}] [{}]".format(key, val))
            try:
                # check out of boundery coordinates
                if key[0] >= self.game_win_hight or key[1] >= self.game_win_width:
                    log.error("[{}] out of window coordinates".format(key))
                    continue

                if val['type'] == 'ant':
                    # to be used for highlighting a cell
                    health      =  int(val['ant']['health'])
                    satiety     =  int(val['ant']['satiety'])
                    ant_type    =  self.game_info[val['ant']['type']]
                    color_pair  =  int(val['ant']['player'])

                    self.game_win.addch(key[0],key[1], ant_type, curses.color_pair(color_pair))
                else:
                    what = self.game_info[val['type']]
                    self.game_win.addch(key[0], key[1], what, curses.color_pair(self.color_type[val['type']]))

            except IndexError:
                log.error("data queue is empty")
                break
            except KeyError:
                log.error("Key error ")
            except Exception as e:
                log.exception(e)
                break

        log.info("Map [{}] for [{}] turn set".format(len(self.map), self.game_info['turn']))
        return True

    def do_refresh(self):

        self.game_win.addstr(2, self.info_win_x, " {}".format(time.asctime()))

    def do_done(self)->bool:
        """ set back terminal settings and close window """
        try:
            self.stdscr.keypad(False)
            curses.nocbreak()
            curses.echo()
            curses.curs_set(1)
            curses.endwin()
        except Exception as e:
            log.exception(e)
            return False
        finally:
            return True


if __name__ == "__main__":

    try:
        log.info("Main function started ... ")
        # initiate udp client object
        client = UdpClient(host_ip='127.0.0.1',host_port=7788,srv_ip='127.0.0.1', srv_port=9999)
        # main window object
        window = Window()

        # initiate connect and bind to socket
        if not client.do_connect():
            log.error("Unable to connect to Host")
            sys.exit(-1)
        # send registration request for data receival
        if not client.do_register():
            log.error("Unable to send registration request to Host")
            sys.exit(-2)

        window.do_init_frame()
        window.do_show_win()

        # main Loop
        while True:

            try:
                if window.get_usr_key():
                    break
                buff = client.do_read()

                if len(buff) > 0:
                    log.debug(buff)
                    # validate bytes object and return json representaion of it
                    data = client.do_validate(buff)
                    # set frame
                    window.do_init_frame()
                    # process and apply changes
                    window.do_process_frame(data)
                    # update map according to changes
                    window.do_set_frame()
                    # show changes
                    window.do_show_win()
                else:
                    window.do_init_frame()
                    window.do_set_frame()
                    window.do_refresh()
                    window.do_show_win()
                    time.sleep(0.2)

            except Exception as e:
                log.exception(e)
                #sys.exit(-3)


    except Exception as e:
        log.exception(e)
        sys.exit(-4)
    finally:
        if not client.do_done():
            log.error("Error while closing Clients object")
        if not window.do_done():
            log.error("Error while closing window object")
        log.info("Process finished. Shutdown. ")
        sys.exit(0)
