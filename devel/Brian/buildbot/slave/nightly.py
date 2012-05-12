from buildslave.bot import BuildSlave
from twisted.internet.task import LoopingCall

class NightlyBuildSlave(BuildSlave):
    """Slave that builds all pending jobs and then shuts down.

    You must pass allow_shutdown='nightly' in the contructor to enable this behaviour:

    It checks every minute if it's connected and if a command is running (active).

    If not connected during 'noconnection_shutdown' consecutive checks,
    it automatically shuts down.

    If not active during 'inactive_shutdown' consecutive checks (while connected),
    it assumes everything is done and tells the master that it wants to shutdown.

    Requires buildbot 0.8.3+ in the master and in the slave."""

    noconnection_shutdown = 5 # number of consectutive noconnection checks needed to shutdown
    inactive_shutdown = 2 # number of consectutive inactive checks needed to shutdown (while connected)

    def startService(self):
        BuildSlave.startService(self)
        if self.allow_shutdown == 'nightly':
            self.shutdown_loop = l = LoopingCall(self._checkShutdownNightly)
            self.noconnection_count = 0
            self.inactive_count = 0
            l.start(interval=60,now=True) # check every minute

    def _checkShutdownNightly(self):
        if not self.bf.perspective:# no connection
            self.noconnection_count += 1
            if self.noconnection_count < self.noconnection_shutdown:
                return
        self.noconnection_count = 0
        if self._active():# active
            self.inactive_count = 0
            return
        else:# inactive
            self.inactive_count += 1
            if self.inactive_count < self.inactive_shutdown:
                return
        self.gracefulShutdown()
        self.shutdown_loop.stop()
        self.shutdown_loop = None
        self.noconnection_count = None
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
