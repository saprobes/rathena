from buildslave.bot import BuildSlave
from twisted.internet.task import LoopingCall
import datetime

class NightlyBuildSlave(BuildSlave):
    """Slave that builds all pending jobs and then shuts down.

    You must pass allow_shutdown={'type':'nightly'} in the contructor to enable this behaviour:

    It checks it if should shutdown every minute.
    Shutdown is done gracefully, by telling the master it wants to shutdown.

    Optional: It shuts down if not connected during allow_shutdown['noconnection'] consecutive checks.
    ex: {'type':'nightly', 'noconnection':5}

    Optional: It shuts down if not active during allow_shutdown['inactive'] consecutive checks (while connected).
    ex: {'type':'nightly', 'inactive':2}

    Optional: It shuts down if it reaches a time outside the schedules in the array allow_shutdown['schedules'].
    A schedule is a tuple with the start time and the end time.
    ex: {'type':'nightly', 'schedules':[(datetime.time(1,1),datetime.time(6))]}

    Requires buildbot 0.8.3+ in the master and in the slave."""

    def startService(self):
        BuildSlave.startService(self)
        if isinstance(self.allow_shutdown, dict) and self.allow_shutdown.has_key('type') and self.allow_shutdown['type'] == 'nightly':
            self.shutdown_loop = l = LoopingCall(self._checkShutdownNightly)
            self.noconnection_count = 0
            self.inactive_count = 0
            l.start(interval=60,now=False) # check every minute

    def _checkShutdownNightly(self):
        if self.allow_shutdown.has_key('schedules'):
            curtime = datetime.datetime.now().time()
            isworktime = any([curtime >= starttime and curtime <= endtime for starttime,endtime in self.allow_shutdown['schedules']])
            if not isworktime:
                self._doShutdownNightly("not work time")
                return
        # work time
        if not self.bf.perspective:# no connection
            self.noconnection_count += 1
            if self.allow_shutdown.has_key('noconnection') and self.noconnection_count >= self.allow_shutdown['noconnection']:
                self._doShutdownNightly("noconnection limit")
            return
        # connected
        self.noconnection_count = 0
        if self._active():# active
            self.inactive_count = 0
        else:# inactive
            self.inactive_count += 1
            if self.allow_shutdown.has_key('inactive') and self.inactive_count >= self.allow_shutdown['inactive']:
                self._doShutdownNightly("inactive limit")

    def _doShutdownNightly(self,reason=None):
        if reason != None:
            print "Nightly shutdown reason:", reason
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
