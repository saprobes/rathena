from buildbot.process.factory import BuildFactory

class CMakeFactory(BuildFactory):
    sourcedir = "source"

    def __init__ ( self, svnurl, generator, arguments=[]):
        """Factory for CMake builds.
        @cvar svnurl: url of the svn repository
        @cvar generator: CMake generator
        @cvar arguments: extra CMake arguments
        """
        assert svnurl is not None
        assert generator is not None
        assert arguments is not None
        from buildbot.steps import source, slave, shell
        BuildFactory.__init__(self)

        # update svn
        self.addStep(source.SVN(
            workdir=self.sourcedir,
            mode='update',
            svnurl=svnurl))
        # recreate build dir
        self.addStep(slave.RemoveDirectory(
            dir=self.workdir,
            haltOnFailure=False,
            flunkOnFailure=False))
        self.addStep(slave.MakeDirectory(
            dir=self.workdir))
        # configure
        self.addStep(shell.Configure(
            command=["cmake", "../source", "-G%s" % generator] + arguments,
            logEnviron=False))
        # compile - the install target builds and then copies files to the
        # production directory (default is subdirectory 'install') and removes
        # full paths from library dependencies so it works when you copy the
        # production files elsewhere
        self.addStep(shell.Compile(
            command=["cmake", "--build", ".", "--target", "install", "--config", "RelWithDebInfo"],
            logEnviron=False))
        # package - the package target creates an installer/package/archive
        # that contains the production files
        self.addStep(shell.ShellCommand(
            name = "Package",
            description = ["packaging"],
            descriptionDone = ["package"],
            haltOnFailure = True,
            flunkOnFailure = True,
            command=["cmake", "--build", ".", "--target", "package", "--config", "RelWithDebInfo"],
            logEnviron=False))
