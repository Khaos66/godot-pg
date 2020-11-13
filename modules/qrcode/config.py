# config.py

def can_build(env, platform):
    return platform == "x11" or platform == "osx" or platform == "windows"

def configure(env):
    pass
