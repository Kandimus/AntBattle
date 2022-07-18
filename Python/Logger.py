import os
import sys
import logging
from datetime import datetime

class Logger:
    """ basic logging class with file handler """
    def __init__(self, name="Logger"):
        self.logger = logging.getLogger(__name__)
        self.logger.setLevel(logging.DEBUG)
        self.format = logging.Formatter('[%(asctime)s %(name)s %(levelname)s: %(message)s] ')
        self.fh  = logging.FileHandler("{}_{}.log".format(datetime.now().strftime("%Y%m%d%H%M%S"), name), encoding="utf-8")
        self.fh.setLevel(logging.DEBUG)
        self.fh.setFormatter(self.format)
        self.logger.addHandler(self.fh)

    def info(self, msg):
        return self.logger.info(msg)

    def debug(self, msg):
        return self.logger.debug(msg)

    def error(self, msg):
        return self.logger.error(msg)

    def warning(self, msg):
        return self.logger.warning(msg)

    def exception(self, msg):
        return self.logger.exception(msg)


