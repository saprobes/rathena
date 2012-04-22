from buildslave.bot import BuildSlave
from twisted.internet.task import LoopingCall

class NightlyBuildSlave(BuildSlave):
    """Tells the master, a minute after starting, that it wants to shutdown when all builds are done.
    You must pass allow_shutdown='nightly' in the contructor to enable this behaviour.
    Requires buildbot 0.8.3+ in the master and in the slave."""

    def startService(self):
        BuildSlave.startService(self)
        if self.allow_shutdown == 'nightly':
            self.shutdown_loop = l = LoopingCall(self._callGracefulShutdown)
            l.start(interval=60,now=False) # wait for a minute

    def _callGracefulShutdown(self):
        self.gracefulShutdown()
        self.shutdown_loop.stop()
        self.shutdown_loop = None
