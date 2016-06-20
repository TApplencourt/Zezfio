import signal


class ExitLoop(Exception):
    pass


for s in (signal.SIGQUIT, signal.SIGTERM, signal.SIGINT):

    def handler(x, y):
        raise ExitLoop

    signal.signal(s, handler)
