from subprocess import call

class RunError(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)

def run(program):
    call("rm kami.out", shell=True)
    if call("./" + program, shell=True) != 0:
        raise RunError("Problem with program " + program)
    OUTPUT = [line.strip() for line in open("kami.out")]
    return OUTPUT

def generate(N, M, MAXVALUES, PROBABILITY, SMALL_VALUES):
    line = "./generator " + str(N) + " " + str(M) + " " + str(MAXVALUES) + " " + str(PROBABILITY) + " " + str(SMALL_VALUES) + " > kami.in"
    if call(line, shell=True) != 0:
        raise RunError("Problem generating with " + line)

def copyTest(number):
    call("cp kami.in grader_test" + str(number) + ".in", shell=True)
    call("cp kami.out grader_test" + str(number) + ".ok", shell=True)

