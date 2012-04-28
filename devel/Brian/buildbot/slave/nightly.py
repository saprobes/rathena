from buildslave.bot import BuildSlave
from twisted.internet.task import LoopingCall

class NightlyBuildSlave(BuildSlave):
    """Slave that builds all pending jobs and then shuts down.

    It checks every minute if a command is running. If not active in 2 
    consecutive checks, it assumes everything is done and tells the master
    that it wants to shutdown. Checks done while the slave isn't connected
    are ignored.

    You must pass allow_shutdown='nightly' in the contructor to enable this
    behaviour.

    Requires buildbot 0.8.3+ in the master and in the slave."""

    inactive_shutdown = 2 # number of consecutive checks needed to shutdown

    def startService(self):
        BuildSlave.startService(self)
        if self.allow_shutdown == 'nightly':
            self.shutdown_loop = l = LoopingCall(self._checkShutdownNightly)
            l.start(interval=60,now=True) # check every minute
            self.inactive_count = 0

    def _checkShutdownNightly(self):
        if not self.bf.perspective:
            return # no connection
        if self._active():
            self.inactive_count = 0 # active
            return
        self.inactive_count += 1
        if self.inactive_count >= self.inactive_shutdown:
            self.gracefulShutdown()
            self.shutdown_loop.stop()
            self.shutdown_loop = None
            self.inactive_count = None

    def _active(self):
        try:
            bot = self.getServiceNamed("bot")
            for builder in bot.builders.values():
                if builder.command:
                    return True # running a command, active
        except KeyError:
            pass
        return False
