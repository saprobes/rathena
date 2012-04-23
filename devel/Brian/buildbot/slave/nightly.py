from buildslave.bot import BuildSlave
from twisted.internet.task import LoopingCall

class NightlyBuildSlave(BuildSlave):
    """Slave that builds all pending jobs and then shuts down.

    It checks every minute if a command is running. If not active in 2 
    consecutive checks, it assumes everything is done and tells the master
    that it wants to shutdown.

    You must pass allow_shutdown='nightly' in the contructor to enable this
    behaviour.

    Requires buildbot 0.8.3+ in the master and in the slave."""

    def startService(self):
        BuildSlave.startService(self)
        if self.allow_shutdown == 'nightly':
            self.shutdown_loop = l = LoopingCall(self._checkShutdownNightly)
            l.start(interval=60,now=False) # check every minute
            self.last_active = False

    def _checkShutdownNightly(self):
        if self._active():
            self.last_active = True # active
        elif self.last_active:
            self.last_active = False # no longer active
        else: # not active for a minute
            self.gracefulShutdown()
            self.shutdown_loop.stop()
            self.shutdown_loop = None
            self.last_active = None

    def _active(self):
        try:
            bot = self.getServiceNamed("bot")
            for builder in bot.builders.values():
                if builder.command:
                    return True # running a command, active
        except KeyError:
            pass
        return False
